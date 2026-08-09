# -*- coding: utf-8 -*-
"""
On-demand blast: randomly fire Adaptive test_script sources at afwfcgi.

Attach (--url) or managed spawn (--conf). Continues on Adaptive failures;
stops if the server dies or duration / max-requests is hit.
"""

from __future__ import print_function

import fnmatch
import json
import os
import random
import sys
import threading
import time
from concurrent.futures import FIRST_COMPLETED, ThreadPoolExecutor, wait

import requests

from _afwdev.common import msg, package
from _afwdev.test.common import (
    find_test_groups,
    load_test_group_config,
    test_group_matches_tags,
)


# Default attach target: docker/dev nginx front door (modes/afwfcgi.py)
_DEFAULT_URL = "http://localhost:8080/afw"
_DEFAULT_DURATION = "5m"


def _cpu_count():
    n = os.cpu_count()
    if n is None or n < 1:
        return 4
    return n


def run(options):
    """Entry point for the blast subcommand."""
    url = options.get("url") or options.get("server_url")
    conf = options.get("conf")
    if url and conf:
        msg.error_exit(
            "afwdev blast: use only one of --url/-u (attach) or "
            "--conf/-f (managed spawn)")
    if not url and not conf:
        # Typical afw docker/dev: nginx always up, afwfcgi when you need it
        url = _DEFAULT_URL

    duration_raw = options.get("duration")
    if duration_raw is None or duration_raw == "":
        duration_raw = _DEFAULT_DURATION
    # duration 0 / 0s / 0m means "no time limit" (use max-requests only)
    duration_s = _parse_duration(duration_raw)
    if duration_s is not None and duration_s <= 0:
        duration_s = None

    max_requests = options.get("max_requests")
    if max_requests is not None and max_requests != "":
        try:
            max_requests = int(max_requests)
        except (TypeError, ValueError):
            msg.error_exit("--max-requests must be an integer")
        if max_requests < 1:
            msg.error_exit("--max-requests must be >= 1")
    else:
        max_requests = None

    if duration_s is None and max_requests is None:
        msg.error_exit(
            "afwdev blast needs a stop condition: --duration/-d "
            "(default 5m) and/or --max-requests/-m")

    cpus = _cpu_count()
    # Historical gobench-style load: threads ≈ CPUs, in-flight ≈ 2×CPUs
    try:
        concurrency = int(options.get("concurrency") or 0)
    except (TypeError, ValueError):
        msg.error_exit("--concurrency must be an integer")
    if concurrency <= 0:
        concurrency = max(1, 2 * cpus)
    if concurrency < 1:
        msg.error_exit("--concurrency must be >= 1")

    try:
        threads = int(options.get("threads") or 0)
    except (TypeError, ValueError):
        msg.error_exit("--threads must be an integer")
    if threads <= 0:
        threads = cpus
    if threads < 1:
        msg.error_exit("--threads must be >= 1")

    timeout_s = float(options.get("request_timeout") or 30.0)
    progress_every_s = float(options.get("progress_every") or 2.0)

    attach = bool(url)
    tests_paths = _normalize_tests_paths(options.get("tests_path"))
    include_fixtures = bool(options.get("include_fixtures"))

    if tests_paths:
        # Explicit roots only — not package tests/, not test -j discovery
        corpus, skipped_fixture = _collect_corpus_from_tests_paths(
            options, tests_paths)
        corpus_mode = "tests-path"
    else:
        srcdirs = _collect_srcdirs(options)
        # Default: skip fixture-heavy groups so fail≈0 unless something is wrong
        corpus, skipped_fixture = _collect_corpus(
            options, srcdirs, skip_fixtures=not include_fixtures)
        corpus_mode = "package-tests"

    if not corpus:
        if tests_paths:
            msg.error_exit(
                "No .as tests under --tests-path "
                "(check paths and --test-pattern): " +
                ", ".join(tests_paths))
        msg.error_exit(
            "No .as tests in blast corpus "
            "(check --srcdir-pattern / --test-pattern / --tags; "
            "fixture groups skipped by default — see --include-fixtures; "
            "or use --tests-path/-T for private corpora)")

    msg.highlighted_info(
        "*** Experimental *** afwdev blast — not part of test -j")
    dur_show = duration_raw if duration_s is not None else "-"
    msg.highlighted_info(
        "corpus={}  mode={}  skipped_fixture={}  target={}  "
        "concurrency={} (cpus={})  duration={}  max_requests={}".format(
            len(corpus),
            corpus_mode,
            skipped_fixture,
            "attach " + url if attach else "managed conf=" + conf,
            concurrency,
            cpus,
            dur_show if duration_s is not None else "-",
            max_requests if max_requests is not None else "-",
        ))
    if tests_paths:
        msg.highlighted_info(
            "tests-path: " + ", ".join(tests_paths))
    if attach:
        msg.highlighted_info(
            "attach: ensure afwfcgi is up behind that URL "
            "(nginx often already running in dev containers)")
    if not tests_paths and not include_fixtures and skipped_fixture:
        msg.highlighted_info(
            "skipping groups with Environment= / afw.conf "
            "(use --include-fixtures to blast those too)")

    handle = None
    socket_path = None
    session = None
    stats = _Stats()
    stop = threading.Event()

    try:
        if attach:
            session = requests.Session()
            session.headers.update({
                "Content-Type": "application/json",
                "Accept": "application/json",
            })
            transport = _HttpTransport(session, url, timeout_s)
            # cheap health: empty eval
            try:
                transport.send("return true;")
            except Exception as e:
                msg.error_exit(
                    "Cannot reach afwfcgi at {!r}: {}".format(url, e))
        else:
            from _afwdev.test.advanced.hosts import afwfcgi as afwfcgi_host
            from _afwdev.test.advanced.fcgi_client import fcgi_request

            work_dir = os.path.dirname(os.path.abspath(conf))
            conf_name = os.path.basename(conf)
            if conf_name != "afw.conf":
                # host helper expects afw.conf in work_dir
                import shutil
                tmp = os.path.join(work_dir, ".afwdev_blast_work")
                if os.path.isdir(tmp):
                    shutil.rmtree(tmp)
                os.makedirs(tmp)
                shutil.copy2(conf, os.path.join(tmp, "afw.conf"))
                work_dir = tmp
            elif not os.path.isfile(os.path.join(work_dir, "afw.conf")):
                msg.error_exit("afw.conf not found: " + conf)

            under_valgrind = (options.get("mode") == "valgrind")
            handle = afwfcgi_host.start_afwfcgi(
                work_dir,
                threads=threads,
                under_valgrind=under_valgrind,
                options=options,
                ready_timeout_s=30.0,
            )
            socket_path = handle["socket_path"]
            transport = _FcgiTransport(socket_path, timeout_s)

        t0 = time.time()
        deadline = (t0 + duration_s) if duration_s is not None else None
        last_progress = t0

        def one_shot():
            if stop.is_set():
                return
            if deadline is not None and time.time() >= deadline:
                stop.set()
                return
            if max_requests is not None and stats.total >= max_requests:
                stop.set()
                return

            path = random.choice(corpus)
            try:
                with open(path, "r", encoding="utf-8", errors="replace") as fd:
                    source = fd.read()
            except OSError as e:
                stats.record_fail(path, "read: " + str(e))
                return

            t_req = time.time()
            try:
                transport.send(source)
                stats.record_ok(path, (time.time() - t_req) * 1000.0)
            except _ServerDead as e:
                stats.record_err(path, str(e))
                stop.set()
                stats.server_dead = True
            except _Timeout as e:
                stats.record_timeout(path, str(e))
            except Exception as e:
                stats.record_fail(path, str(e))

        try:
            with ThreadPoolExecutor(max_workers=concurrency) as pool:
                futures = set()
                while not stop.is_set():
                    if deadline is not None and time.time() >= deadline:
                        break
                    if max_requests is not None and stats.total >= max_requests:
                        break

                    while (len(futures) < concurrency and not stop.is_set()):
                        if max_requests is not None and (
                                stats.total + len(futures) >= max_requests):
                            break
                        if deadline is not None and time.time() >= deadline:
                            break
                        futures.add(pool.submit(one_shot))

                    if not futures:
                        break

                    done, futures = wait(
                        futures, timeout=0.5, return_when=FIRST_COMPLETED)
                    for fut in done:
                        try:
                            fut.result()
                        except Exception as e:
                            msg.debug("worker: " + str(e))

                    now = time.time()
                    if now - last_progress >= progress_every_s:
                        _print_progress(stats, t0)
                        last_progress = now

                if futures:
                    done, _ = wait(futures)
                    for fut in done:
                        try:
                            fut.result()
                        except Exception:
                            pass
        except KeyboardInterrupt:
            msg.highlighted_info("\nInterrupted — stopping blast.")
            stop.set()

        _print_progress(stats, t0, final=True)
        elapsed = time.time() - t0
        msg.highlighted_info("")
        msg.highlighted_info(
            "blast done in {:.1f}s  ok={}  fail={}  timeout={}  err={}  "
            "total={}  latency_ms avg={:.0f} max={:.0f}".format(
                elapsed,
                stats.ok,
                stats.fail,
                stats.timeout,
                stats.err,
                stats.total,
                stats.avg_latency_ms(),
                stats.max_latency_ms,
            ))
        if stats.timeout and not stats.fail and not stats.err:
            msg.highlighted_info(
                "note: only client timeouts (no expect fails) — often load/"
                "queue; try lower -c or higher --request-timeout")
        if stats.recent_fails:
            msg.error("Recent problems:")
            for kind, path, detail in stats.recent_fails[-10:]:
                msg.error("  [{}] {}  {}".format(
                    kind,
                    os.path.relpath(path) if os.path.exists(path) else path,
                    detail[:120]))

        if stats.server_dead:
            msg.error("Server died or became unreachable during blast.")
            sys.exit(2)
        if stats.fail or stats.err or stats.timeout:
            sys.exit(1)
        sys.exit(0)

    finally:
        if handle is not None:
            from _afwdev.test.advanced.hosts import afwfcgi as afwfcgi_host
            afwfcgi_host.stop_afwfcgi(handle)
        if session is not None:
            try:
                session.close()
            except Exception:
                pass


class _ServerDead(Exception):
    pass


class _Timeout(Exception):
    """Client-side request timeout (load/queue), not an Adaptive expect fail."""
    pass


class _HttpTransport(object):
    def __init__(self, session, url, timeout_s):
        self._session = session
        self._url = url
        self._timeout = timeout_s

    def send(self, source):
        payload = {
            "actions": [
                {"function": "eval<script>", "source": source}
            ]
        }
        try:
            r = self._session.post(
                self._url,
                data=json.dumps(payload),
                timeout=self._timeout,
            )
        except requests.exceptions.ConnectionError as e:
            raise _ServerDead("connection failed: " + str(e)) from e
        except requests.exceptions.Timeout as e:
            raise _Timeout("request timeout: " + str(e)) from e

        if r.status_code >= 500:
            raise _ServerDead(
                "HTTP {} from server".format(r.status_code))

        try:
            body = r.json()
        except ValueError as e:
            raise RuntimeError(
                "non-JSON response HTTP {}: {}".format(
                    r.status_code, r.text[:200])) from e

        status = body.get("status")
        if status and status != "success":
            raise RuntimeError(
                "status={!r}: {}".format(status, json.dumps(body)[:300]))

        actions = body.get("actions") or []
        for act in actions:
            if isinstance(act, dict) and act.get("status") == "error":
                raise RuntimeError(
                    "action error: {}".format(
                        json.dumps(act.get("error") or act)[:300]))
            _raise_if_test_script_failures(act.get("result") if isinstance(act, dict) else None)
        return body


class _FcgiTransport(object):
    def __init__(self, socket_path, timeout_s):
        self._socket_path = socket_path
        self._timeout = timeout_s

    def send(self, source):
        from _afwdev.common import nfc
        from _afwdev.test.advanced.fcgi_client import (
            FcgiClientError,
            fcgi_request,
        )

        payload = nfc.json_dumps({
            "actions": [
                {"function": "eval<script>", "source": source}
            ]
        })
        try:
            result = fcgi_request(
                self._socket_path,
                path="/afw",
                method="POST",
                body=payload,
                timeout=self._timeout,
            )
        except FcgiClientError as e:
            err = str(e).lower()
            if "timeout" in err:
                raise _Timeout(str(e)) from e
            if "connect" in err or "closed" in err:
                raise _ServerDead(str(e)) from e
            raise RuntimeError(str(e)) from e

        body_text = (result.get("body") or b"").decode(
            "utf-8", errors="replace")
        try:
            body = nfc.json_loads(body_text) if body_text.strip() else {}
        except Exception as e:
            raise RuntimeError(
                "non-JSON FCGI body: {}".format(body_text[:200])) from e

        status = body.get("status")
        if status and status != "success":
            raise RuntimeError(
                "status={!r}: {}".format(status, body_text[:300]))
        actions = body.get("actions") or []
        for act in actions:
            if isinstance(act, dict) and act.get("status") == "error":
                raise RuntimeError(
                    "action error: {}".format(
                        nfc.json_dumps(act.get("error") or act)[:300]))
            _raise_if_test_script_failures(
                act.get("result") if isinstance(act, dict) else None)
        return body


def _raise_if_test_script_failures(result_obj):
    """If eval returned a test_script result with failed cases, raise."""
    if not isinstance(result_obj, dict) or "tests" not in result_obj:
        return
    for tc in result_obj.get("tests") or []:
        if tc.get("skip"):
            continue
        if tc.get("passed", False) is False:
            raise RuntimeError(
                "test_script failure: {}".format(
                    tc.get("test") or tc.get("description") or tc))


class _Stats(object):
    def __init__(self):
        self._lock = threading.Lock()
        self.ok = 0
        self.fail = 0          # Adaptive expect / action errors
        self.timeout = 0       # client read/connect timeout (load)
        self.err = 0           # server dead / 5xx class
        self.total = 0
        self.last_path = ""
        self.recent_fails = []  # (kind, path, detail)
        self.server_dead = False
        self.latency_sum_ms = 0.0
        self.latency_n = 0
        self.max_latency_ms = 0.0

    def record_ok(self, path, latency_ms=0.0):
        with self._lock:
            self.ok += 1
            self.total += 1
            self.last_path = path
            if latency_ms > 0:
                self.latency_sum_ms += latency_ms
                self.latency_n += 1
                if latency_ms > self.max_latency_ms:
                    self.max_latency_ms = latency_ms

    def record_fail(self, path, detail):
        with self._lock:
            self.fail += 1
            self.total += 1
            self.last_path = path
            self.recent_fails.append(("fail", path, detail))
            if len(self.recent_fails) > 50:
                self.recent_fails = self.recent_fails[-50:]

    def record_timeout(self, path, detail):
        with self._lock:
            self.timeout += 1
            self.total += 1
            self.last_path = path
            self.recent_fails.append(("timeout", path, detail))
            if len(self.recent_fails) > 50:
                self.recent_fails = self.recent_fails[-50:]

    def record_err(self, path, detail):
        with self._lock:
            self.err += 1
            self.total += 1
            self.last_path = path
            self.recent_fails.append(("err", path, detail))

    def avg_latency_ms(self):
        if self.latency_n < 1:
            return 0.0
        return self.latency_sum_ms / float(self.latency_n)


def _print_progress(stats, t0, final=False):
    elapsed = max(0.001, time.time() - t0)
    rps = stats.total / elapsed
    last = stats.last_path
    if last:
        try:
            last = os.path.relpath(last)
        except ValueError:
            pass
    line = (
        "blast  {elapsed}  req={total}  ok={ok}  fail={fail}  "
        "timeout={timeout}  err={err}  rps≈{rps:.1f}  "
        "lat_ms≈{lat:.0f}/{mx:.0f}  last={last}"
    ).format(
        elapsed=_fmt_hms(elapsed),
        total=stats.total,
        ok=stats.ok,
        fail=stats.fail,
        timeout=stats.timeout,
        err=stats.err,
        rps=rps,
        lat=stats.avg_latency_ms(),
        mx=stats.max_latency_ms,
        last=last or "-",
    )
    if final:
        msg.highlighted_info(line)
    else:
        print(line, flush=True)


def _fmt_hms(seconds):
    seconds = int(seconds)
    h = seconds // 3600
    m = (seconds % 3600) // 60
    s = seconds % 60
    if h:
        return "{:d}:{:02d}:{:02d}".format(h, m, s)
    return "{:d}:{:02d}".format(m, s)


def _parse_duration(text):
    if text is None or text == "":
        return None
    text = str(text).strip().lower()
    if not text:
        return None
    mult = 1.0
    if text.endswith("ms"):
        mult = 0.001
        text = text[:-2]
    elif text.endswith("s"):
        mult = 1.0
        text = text[:-1]
    elif text.endswith("m"):
        mult = 60.0
        text = text[:-1]
    elif text.endswith("h"):
        mult = 3600.0
        text = text[:-1]
    try:
        return float(text) * mult
    except ValueError:
        msg.error_exit(
            "Invalid --duration {!r} (use 30s, 5m, 1h, or seconds)".format(
                text))


def _normalize_tests_paths(raw):
    """Return list of absolute existing directories from --tests-path (or [])."""
    if not raw:
        return []
    if isinstance(raw, str):
        raw = [raw]
    paths = []
    for p in raw:
        if p is None or p == "":
            continue
        ap = os.path.abspath(os.path.expanduser(str(p)))
        if not os.path.isdir(ap):
            msg.error_exit(
                "afwdev blast --tests-path is not a directory: " + ap)
        paths.append(ap)
    return paths


def _collect_srcdirs(options):
    pattern = options.get("srcdir_pattern") or "*"
    pattern = pattern.replace("\\", "")
    srcdirs = []
    for srcdir in package.get_afw_package(options)["srcdirs"]:
        if not fnmatch.fnmatch(srcdir, pattern):
            continue
        package.set_options_from_existing_package_srcdir(
            options, srcdir, set_all=True)
        srcdir_path = options["srcdir_path"]
        manual_tests = srcdir_path + "tests"
        srcdirs.append((srcdir, srcdir_path, None, manual_tests))
    return srcdirs


def _collect_corpus_from_tests_paths(options, tests_paths):
    """
    Build corpus from explicit directory roots (--tests-path).

    Recursively finds *.as (not _*). Applies --test-pattern when set.
    Does not use package srcdir discovery or fixture skip (roots are intentional).
    """
    from _afwdev.test.common import _test_pattern_matches

    pattern = options.get("test_pattern") or ".*"
    corpus = []
    seen = set()
    for root in tests_paths:
        for dirpath, _dirnames, filenames in os.walk(root):
            # skip hidden / underscore dir segments
            parts = os.path.relpath(dirpath, root).split(os.sep)
            if any(p.startswith("_") or p.startswith(".") for p in parts
                   if p not in (".", "")):
                continue
            for name in filenames:
                if not name.endswith(".as") or name.startswith("_"):
                    continue
                path = os.path.join(dirpath, name)
                ap = os.path.abspath(path)
                if ap in seen:
                    continue
                if not _test_pattern_matches(pattern, ap) and \
                        not _test_pattern_matches(pattern, name):
                    continue
                seen.add(ap)
                corpus.append(ap)
    corpus.sort()
    return corpus, 0


def _group_needs_fixture(root, config):
    """
    True if this test group needs a private conf, shared environment, or
    work-dir setup that a bare live /afw (or single --conf) does not provide.

    Matches Jeremy's live afwfcgi mode spirit: skip custom stacks; default
    blast should fail only when something is actually wrong.
    """
    if config and config.get("environment"):
        return True, "Environment=" + str(config.get("environment"))
    if os.path.isfile(os.path.join(root, "afw.conf")):
        return True, "group afw.conf"
    return False, None


def _path_has_afw_conf_nearby(path, stop_dir_name="tests"):
    """Walk parents of the test file for afw.conf (subdir tests under a conf)."""
    d = os.path.dirname(os.path.abspath(path))
    for _ in range(12):
        if os.path.isfile(os.path.join(d, "afw.conf")):
            return True
        base = os.path.basename(d)
        if base == stop_dir_name or d == os.path.dirname(d):
            break
        d = os.path.dirname(d)
    return False


def _collect_corpus(options, srcdirs, skip_fixtures=True):
    """
    Build list of .as paths for blasting.

    When skip_fixtures (default): omit groups with config.py Environment,
    group-level afw.conf, or afw.conf next to / above the script. Those need
    afwdev test work dirs / environments, not a random eval on /afw.
    """
    corpus = []
    skipped_fixture = 0
    for srcdir, srcdir_path, _, manual_tests in srcdirs:
        if not os.path.isdir(manual_tests):
            continue
        groups = find_test_groups(options, srcdir, manual_tests)
        for _sd, root, tests in groups:
            config = load_test_group_config(root)
            if not test_group_matches_tags(options, config):
                continue

            if skip_fixtures:
                needs, reason = _group_needs_fixture(root, config)
                if needs:
                    n_as = sum(1 for p in tests if p.endswith(".as"))
                    skipped_fixture += n_as
                    msg.debug(
                        "blast skip group ({}) {}: {} script(s)".format(
                            reason, root, n_as))
                    continue

            for path in tests:
                if not path.endswith(".as"):
                    continue
                base = os.path.basename(path)
                if base.startswith("_"):
                    continue
                if skip_fixtures and _path_has_afw_conf_nearby(path):
                    skipped_fixture += 1
                    msg.debug(
                        "blast skip (afw.conf nearby): " + path)
                    continue
                corpus.append(path)
    return corpus, skipped_fixture
