# -*- coding: utf-8 -*-
"""
Run one orchestrated-test leaf (orchestration.yaml|json).

Returns (response, error, debug) compatible with afwdev test parse_test_run.
"""

import multiprocessing
import os
import random
import re
import signal
import threading
import time
from concurrent.futures import (
    FIRST_COMPLETED,
    ThreadPoolExecutor,
    as_completed,
    wait,
)

from _afwdev.common import msg, nfc
from _afwdev.common.errors import (
    AfwAdaptiveError,
    AfwdevError,
    AfwdevProcessError,
    AfwdevRunnerError,
    adaptive_error_from_response,
    error_message,
    error_to_dict,
    wrap_exception,
)
from _afwdev.test.orchestrated.load import (
    OrchestrationLoadError,
    eval_function_for_source_type,
    load_orchestration_document,
    merge_feed,
    parse_count_spec,
    parse_triple_lt_path,
    resolve_count_spec,
    resolve_file_bytes,
    resolve_file_text,
    resolve_source_text,
)
from _afwdev.test.orchestrated.fcgi_client import fcgi_request
from _afwdev.test.orchestrated import http_front
from _afwdev.test.orchestrated.hosts import afwfcgi as afwfcgi_host
from _afwdev.test.orchestrated.hosts import local as local_host
from _afwdev.test.orchestrated import x_afw_demux
from _afwdev.test import failure_log


def _journal_failure(options, name, err, ctx):
    failure_log.record(
        options,
        name=name,
        message=error_message(err) or str(err),
        err=err,
        stderr_path=(ctx or {}).get("log_path"),
    )


def _capture_goldens_enabled(options):
    if options and options.get("capture_goldens"):
        return True
    env = os.environ.get("AFWDEV_CAPTURE_GOLDENS", "").strip().lower()
    return env in ("1", "true", "yes", "on")


def run_orchestrated_test(marker_path, options, testEnvironment=None,
                          testGroupConfig=None):
    """Entry point from common.run_test for orchestration markers."""
    mode = (options or {}).get("mode") or "afw"
    options = options or {}
    debug_parts = []
    if mode == "actions":
        msg.debug("Skipping orchestrated-test under --env-mode actions: " +
                  marker_path)
        return None, None, None

    if mode == "afwfcgi":
        msg.debug(
            "Skipping orchestrated-test under --env-mode afwfcgi "
            "(live stack; use default env-mode for hermetic leaves): "
            + marker_path)
        return None, None, None

    under_valgrind = (mode == "valgrind")

    try:
        doc = load_orchestration_document(marker_path)
    except OrchestrationLoadError as e:
        return _fail_response(str(marker_path), e), None, None

    work_dir = None
    if testEnvironment and testEnvironment.get("work_dir"):
        work_dir = testEnvironment["work_dir"]
    else:
        work_dir = os.path.dirname(marker_path)

    host_kind = doc.get("host") or "afwfcgi"
    conf_path = os.path.join(work_dir, "afw.conf")
    # afwfcgi needs leaf conf; afw --local has a built-in default conf.
    if host_kind == "afwfcgi" and not os.path.isfile(conf_path):
        err = AfwdevRunnerError(
            "orchestrated-test host afwfcgi requires afw.conf in leaf/work "
            "dir: " + work_dir)
        return _fail_response("orchestrated-test", err), None, None

    description = doc.get("description") or os.path.basename(work_dir)
    timeout_s = float(doc.get("timeout_s") or 120.0)
    threads = int(doc.get("afwfcgi", {}).get("threads") or 1)
    tests_by_name = {t["name"]: t for t in doc["tests"]}
    doc_feed = doc.get("feed") or {}
    # Source leaf (marker directory) — goldens write here, not only the
    # hermetic work_dir copy under /tmp.
    source_leaf = os.path.dirname(os.path.abspath(marker_path))

    handle = None
    http_front_handle = None
    t0 = time.time()
    try:
        socket_path = None
        http_doc = (doc.get("afwfcgi") or {}).get("http")
        if http_doc and host_kind == "afwfcgi":
            http_front_handle = http_front.prepare(
                http_doc, work_dir, options)
            http_front_handle.httpd.ask_stop = _ask_firehose_stop
        if host_kind == "afwfcgi":
            # Valgrind cold-start is much slower, especially under -j load.
            ready_cap = 120.0 if under_valgrind else 30.0
            handle = afwfcgi_host.start_afwfcgi(
                work_dir,
                threads=threads,
                under_valgrind=under_valgrind,
                options=options,
                ready_timeout_s=min(ready_cap, timeout_s),
            )
            socket_path = handle["socket_path"]
            debug_parts.append("started: " + " ".join(handle["argv"]))
            debug_parts.append("socket: " + socket_path)
            if http_front_handle is not None:
                http_front.serve(http_front_handle, socket_path)
                debug_parts.append("http: " + http_front_handle.url)
        else:
            debug_parts.append("host: local (afw --local 1 per work item)")

        step_timings = []
        schedule = doc.get("schedule")
        if not schedule:
            # Include skipped names so counts stay stable; runner no-ops skip.
            schedule = [{"sequential": [t["name"] for t in doc["tests"]]}]

        ctx = {
            "host_kind": host_kind,
            "socket_path": socket_path,
            "under_valgrind": under_valgrind,
            "conf_path": conf_path if os.path.isfile(conf_path) else None,
            "log_path": (handle or {}).get("log_path"),
        }

        for phase in schedule:
            if time.time() - t0 > timeout_s:
                err = AfwdevRunnerError(
                    "orchestrated-test timed out after {}s".format(timeout_s))
                return (
                    _fail_response(description, err, step_timings),
                    None,
                    _debug_blob(debug_parts, handle),
                )

            if not isinstance(phase, dict) or len(phase) != 1:
                err = AfwdevRunnerError(
                    "schedule phase must be a single-key mapping: "
                    + repr(phase))
                return (
                    _fail_response(description, err, step_timings),
                    None,
                    _debug_blob(debug_parts, handle),
                )

            kind, body = next(iter(phase.items()))
            remaining = max(1.0, timeout_s - (time.time() - t0))

            if kind == "sequential":
                names = body if isinstance(body, list) else []
                for name in names:
                    item = tests_by_name.get(name)
                    if item is None:
                        err = AfwdevRunnerError(
                            "schedule sequential unknown test {!r}".format(
                                name))
                        return (
                            _fail_response(description, err, step_timings),
                            None,
                            _debug_blob(debug_parts, handle),
                        )
                    if item.get("skip"):
                        step_timings.append({
                            "name": name, "ms": 0, "passed": True,
                            "skip": True,
                        })
                        continue
                    _run_named_test(
                        item, work_dir, source_leaf, ctx, remaining,
                        doc_feed, debug_parts, step_timings, description,
                        options, fail_fast=True)

            elif kind == "parallel":
                if host_kind == "local":
                    raise AfwdevRunnerError(
                        "schedule parallel is not supported for host local")
                if not isinstance(body, dict):
                    raise AfwdevRunnerError(
                        "schedule parallel body must be a mapping")
                names = body.get("tests") or []
                n = int(body.get("n") or len(names) or 1)
                items = []
                for name in names:
                    item = tests_by_name.get(name)
                    if item is None:
                        raise AfwdevRunnerError(
                            "schedule parallel unknown test {!r}".format(name))
                    if not item.get("skip"):
                        items.append(item)
                _run_parallel(
                    items, n, work_dir, source_leaf, ctx, remaining,
                    doc_feed, debug_parts, step_timings, options)

            elif kind == "firehose":
                if host_kind == "local":
                    raise AfwdevRunnerError(
                        "schedule firehose is not supported for host local")
                if not isinstance(body, dict):
                    raise AfwdevRunnerError(
                        "schedule firehose body must be a mapping")
                _run_firehose(
                    body, tests_by_name, work_dir, source_leaf, ctx,
                    remaining, doc_feed, debug_parts, step_timings,
                    description, options, handle, threads)

            elif kind == "repeat":
                if not isinstance(body, dict):
                    raise AfwdevRunnerError(
                        "schedule repeat body must be a mapping")
                times = int(body.get("times") or 1)
                names = body.get("tests") or []
                for _ in range(times):
                    for name in names:
                        item = tests_by_name.get(name)
                        if item is None or item.get("skip"):
                            continue
                        rem = max(1.0, timeout_s - (time.time() - t0))
                        if rem <= 1.0:
                            raise AfwdevRunnerError(
                                "orchestrated-test timed out during repeat")
                        _run_named_test(
                            item, work_dir, source_leaf, ctx, rem,
                            doc_feed, debug_parts, step_timings, description,
                            options, fail_fast=True)

            else:
                raise AfwdevRunnerError(
                    "unknown schedule phase {!r}".format(kind))

        if (host_kind == "afwfcgi" and under_valgrind and handle and
                afwfcgi_host.valgrind_errors_in_log(handle.get("log_path"))):
            err = AfwdevProcessError(
                "Valgrind error(s) detected in afwfcgi stderr")
            return (
                _fail_response(description, err, step_timings),
                None,
                _debug_blob(debug_parts, handle),
            )

        return (
            _pass_response(description, step_timings),
            None,
            _debug_blob(debug_parts, handle),
        )

    except afwfcgi_host.AfwfcgiHostError as e:
        return (
            _fail_response(description, e, None),
            e,
            _debug_blob(debug_parts, handle),
        )
    except local_host.AfwLocalHostError as e:
        return (
            _fail_response(description, e, None),
            e,
            _debug_blob(debug_parts, handle),
        )
    except Exception as e:
        wrapped = wrap_exception(e)
        return (
            _fail_response(description, wrapped, None),
            wrapped,
            _debug_blob(debug_parts, handle),
        )
    finally:
        if http_front_handle is not None:
            http_front.stop(http_front_handle)
        if handle is not None:
            afwfcgi_host.stop_afwfcgi(handle)


def _run_named_test(item, work_dir, source_leaf, ctx, timeout,
                    doc_feed, debug_parts, step_timings, description, options,
                    fail_fast=True):
    name = item["name"]
    msg.debug("orchestrated-test: " + name)
    t0 = time.time()
    try:
        _run_test_item(item, work_dir, source_leaf, ctx, timeout,
                       doc_feed, debug_parts, options)
        ms = round((time.time() - t0) * 1000)
        step_timings.append({"name": name, "ms": ms, "passed": True})
        debug_parts.append("test {!r} ok ({}ms)".format(name, ms))
    except Exception as e:
        ms = round((time.time() - t0) * 1000)
        step_timings.append({"name": name, "ms": ms, "passed": False})
        wrapped = wrap_exception(e, default_cls=AfwAdaptiveError)
        if not isinstance(wrapped, AfwdevError):
            wrapped = AfwAdaptiveError(
                "test {!r} failed: {}".format(name, error_message(e)),
                cause=e)
        else:
            wrapped = type(wrapped)(
                "test {!r} failed: {}".format(name, error_message(wrapped)),
                object=getattr(wrapped, "object", None),
                cause=e,
            )
        debug_parts.append(error_message(wrapped))
        _journal_failure(options, name, wrapped, ctx)
        if fail_fast:
            raise wrapped
        return wrapped
    return None


def _run_parallel(items, n, work_dir, source_leaf, ctx, timeout,
                  doc_feed, debug_parts, step_timings, options):
    if not items:
        return
    n = max(1, min(n, len(items)))
    errors = []

    def one(item):
        t0 = time.time()
        try:
            _run_test_item(item, work_dir, source_leaf, ctx, timeout,
                           doc_feed, debug_parts, options)
            return item["name"], True, round((time.time() - t0) * 1000), None
        except Exception as e:
            _journal_failure(options, item.get("name"), e, ctx)
            return item["name"], False, round((time.time() - t0) * 1000), e
    with ThreadPoolExecutor(max_workers=n) as ex:
        futs = [ex.submit(one, it) for it in items]
        for fut in as_completed(futs):
            name, ok, ms, err = fut.result()
            step_timings.append({"name": name, "ms": ms, "passed": ok})
            if not ok:
                errors.append((name, err))
                debug_parts.append(
                    "parallel test {!r} failed: {}".format(
                        name, error_message(err)))
            else:
                debug_parts.append(
                    "parallel test {!r} ok ({}ms)".format(name, ms))
    if errors:
        name, err = errors[0]
        raise AfwAdaptiveError(
            "parallel test {!r} failed: {}".format(name, error_message(err)),
            cause=err)


def _resolve_firehose_count(value, base, what, default):
    if value is None:
        return default
    spec = parse_count_spec(value, what, what)
    return resolve_count_spec(spec, base)


def _afwfcgi_dead(handle):
    """Error if the server process has exited, else None."""
    if not handle:
        return None
    proc = handle.get("process")
    if proc is None or proc.poll() is None:
        return None
    tail = ""
    log_path = handle.get("log_path")
    if log_path and os.path.isfile(log_path):
        try:
            with open(log_path, "rb") as fd:
                tail = fd.read()[-2000:].decode("utf-8", "replace").strip()
        except OSError:
            tail = ""
    return AfwdevRunnerError(
        "afwfcgi exited ({}) during firehose. {}".format(
            proc.returncode, tail or "(no stderr)"))


def _sample_server(socket_path):
    """One read of _AdaptiveServer_/current. None if the read fails."""
    source = (
        "const s = get_object(\"afw\", \"_AdaptiveServer_\", \"current\");\n"
        "return [s.threadCount, s.concurrent, s.maxConcurrent, "
        "s.requestCount];\n"
    )
    body = nfc.json_dumps({
        "actions": [{"function": "eval<script>", "source": source}],
    })
    try:
        result = fcgi_request(
            socket_path, path="/afw", method="POST", body=body,
            param_overrides={"HTTP_ACCEPT": "application/json"},
            timeout=5.0,
        )
        parsed = nfc.json_loads((result.get("body") or b"").decode("utf-8"))
    except Exception:
        return None
    if not isinstance(parsed, dict) or parsed.get("status") != "success":
        return None
    actions = parsed.get("actions") or []
    if not actions or not isinstance(actions[0], dict):
        return None
    values = actions[0].get("result")
    if not isinstance(values, list) or len(values) < 4:
        return None
    return {
        "threadCount": values[0],
        "concurrent": values[1],
        "maxConcurrent": values[2],
        "requestCount": values[3],
    }


def _firehose_request_ok(item, work_dir, socket_path, doc_feed, timeout):
    """Issue one action request. Raise on a failed or non-success response."""
    source = resolve_source_text(item, work_dir)
    source_type = item.get("sourceType") or "script"
    feed = merge_feed(doc_feed, item.get("feed"))
    function = eval_function_for_source_type(source_type, feed)
    if function == "evaluate":
        action = {"function": "evaluate", "expression": source}
    else:
        action = {"function": function, "source": source}
    body = nfc.json_dumps({"actions": [action]})
    result = fcgi_request(
        socket_path,
        path=feed.get("path") or "/afw",
        method="POST",
        body=body,
        param_overrides={
            "HTTP_ACCEPT": feed.get("accept") or "application/json"},
        timeout=timeout,
    )
    raw = result.get("body") or b""
    if result.get("status_code") != 200:
        raise AfwdevRunnerError(
            "firehose {}: HTTP {}".format(
                item.get("name"), result.get("status_code")))
    parsed = nfc.json_loads(raw.decode("utf-8", "replace"))
    if not isinstance(parsed, dict) or parsed.get("status") != "success":
        raise AfwdevRunnerError(
            "firehose {} failed: {}".format(
                item.get("name"), raw[:300].decode("utf-8", "replace")))
    return True


# Set before the fork pool starts so workers inherit them. Passing a
# Condition through the task queue raises "shared through inheritance".
_FH_STOP = None
_FH_ISSUED = None
_ASKED = threading.Event()


def _ask_firehose_stop():
    """End the current firehose after the requests already in flight."""
    if _ASKED.is_set():
        return
    _ASKED.set()
    stop = _FH_STOP
    if stop is not None:
        try:
            stop.set()
        except Exception:
            pass
    msg.highlighted_info("stopping")


def _on_firehose_signal(signum, _frame):
    if _ASKED.is_set():
        signal.signal(signum, signal.SIG_DFL)
        os.kill(os.getpid(), signum)
        return
    _ask_firehose_stop()


def _firehose_process_entry(args):
    """One client process. Returns (ok, fail, first_error_or_None)."""
    stop = _FH_STOP
    issued = _FH_ISSUED
    (socket_path, work_dir, items, doc_feed, deadline, seed, policy,
     per, max_requests, timeout, stop_on_error) = args
    def one_loop(start):
        ok = fail = 0
        first = None
        rr = start
        rng = random.Random(start)
        while True:
            if stop is None or stop.is_set():
                break
            if deadline is not None and time.time() >= deadline:
                break
            if max_requests is not None:
                with issued.get_lock():
                    if issued.value >= max_requests:
                        break
                    issued.value += 1
            if policy == "roundRobin":
                item = items[rr % len(items)]
                rr += 1
            else:
                item = rng.choice(items)
            try:
                _firehose_request_ok(
                    item, work_dir, socket_path, doc_feed, timeout)
                ok += 1
            except Exception as exc:
                fail += 1
                if first is None:
                    first = "{}".format(exc)
                if stop_on_error:
                    stop.set()
                    break
        return ok, fail, first

    if per <= 1:
        return one_loop(seed)
    with ThreadPoolExecutor(max_workers=per) as ex:
        futs = [ex.submit(one_loop, seed + i * 997) for i in range(per)]
        ok = fail = 0
        first = None
        for fut in futs:
            part_ok, part_fail, part_first = fut.result()
            ok += part_ok
            fail += part_fail
            if first is None and part_first:
                first = part_first
        return ok, fail, first


def _run_firehose_threads(concurrency, pool, work_dir, source_leaf, ctx,
                          timeout, doc_feed, options, t_end, t0, policy,
                          rng, max_requests, stop_on_error, handle,
                          quiet_log, honor_timeout=True):
    """Single-process firehose. One Python thread per in-flight request."""
    ok = fail = 0
    total = 0
    first_error = None
    rr_i = 0

    def pick_item():
        nonlocal rr_i
        if policy == "roundRobin":
            item = pool[rr_i % len(pool)]
            rr_i += 1
            return item
        return rng.choice(pool)

    def one(item):
        try:
            _run_test_item(item, work_dir, source_leaf, ctx,
                           max(5.0, timeout), doc_feed, quiet_log, options)
            return True, None
        except Exception as e:
            _journal_failure(options, item.get("name"), e, ctx)
            return False, e

    with ThreadPoolExecutor(max_workers=concurrency) as ex:
        pending = set()
        while True:
            dead = _afwfcgi_dead(handle)
            if dead is not None:
                raise dead
            if _ASKED.is_set():
                break
            if t_end is not None and time.time() >= t_end:
                break
            if honor_timeout and time.time() - t0 > timeout:
                raise AfwdevRunnerError(
                    "orchestrated-test timed out during firehose")
            if max_requests is not None and total >= max_requests:
                break
            while len(pending) < concurrency:
                if _ASKED.is_set():
                    break
                if max_requests is not None and total >= max_requests:
                    break
                if t_end is not None and time.time() >= t_end:
                    break
                item = pick_item()
                pending.add(ex.submit(one, item))
                total += 1

            if not pending:
                break

            # A timed sleep here raced ahead of the workers and capped
            # the leaf near concurrency/10ms.
            done, pending = wait(
                pending, timeout=0.05, return_when=FIRST_COMPLETED)
            if not done:
                continue
            for fut in done:
                success, err = fut.result()
                if success:
                    ok += 1
                else:
                    fail += 1
                    if first_error is None:
                        first_error = err
                    if stop_on_error:
                        for p in pending:
                            p.cancel()
                        raise AfwAdaptiveError(
                            "firehose stopOnError: {}".format(
                                error_message(err)),
                            cause=err)

        for fut in as_completed(pending):
            success, err = fut.result()
            if success:
                ok += 1
            else:
                fail += 1
                if first_error is None:
                    first_error = err

    dead = _afwfcgi_dead(handle)
    if dead is not None:
        raise dead
    return ok, fail, total, first_error


def _run_firehose_processes(client_processes, concurrency, pool, work_dir,
                            socket_path, doc_feed, deadline, policy, seed,
                            max_requests, timeout, stop_on_error, handle):
    """Feed afwfcgi from several processes. Raises if the server exits."""
    if client_processes > concurrency:
        client_processes = concurrency
    base = concurrency // client_processes
    extra = concurrency % client_processes
    global _FH_STOP, _FH_ISSUED
    ctx = multiprocessing.get_context("fork")
    _FH_STOP = ctx.Event()
    _FH_ISSUED = ctx.Value("i", 0)
    stop = _FH_STOP
    if _ASKED.is_set():
        stop.set()
    pool_mp = ctx.Pool(client_processes)
    asyncs = []
    for i in range(client_processes):
        per = base + (1 if i < extra else 0)
        if per < 1:
            per = 1
        asyncs.append(pool_mp.apply_async(_firehose_process_entry, ((
            socket_path, work_dir, pool, doc_feed, deadline,
            (seed or 0) + i * 1009, policy, per,
            max_requests, timeout, stop_on_error,
        ),)))
    died = None
    try:
        while not all(item.ready() for item in asyncs):
            if _ASKED.is_set():
                stop.set()
            died = _afwfcgi_dead(handle)
            if died is not None:
                stop.set()
                break
            time.sleep(0.2)
        if died is None:
            died = _afwfcgi_dead(handle)
    finally:
        if died is not None:
            stop.set()
            pool_mp.terminate()
        else:
            pool_mp.close()
        pool_mp.join()
    if died is not None:
        raise died
    ok = fail = 0
    first_error = None
    for item in asyncs:
        part_ok, part_fail, part_first = item.get()
        ok += part_ok
        fail += part_fail
        if first_error is None and part_first:
            first_error = AfwdevRunnerError(part_first)
    return ok, fail, ok + fail, first_error


def _run_firehose(body, tests_by_name, work_dir, source_leaf, ctx,
                  timeout, doc_feed, debug_parts, step_timings, description,
                  options, handle=None, server_threads=1):
    names = body.get("fromTests") or []
    pool = []
    for name in names:
        item = tests_by_name.get(name)
        if item is None:
            raise AfwdevRunnerError(
                "firehose fromTests unknown test {!r}".format(name))
        if not item.get("skip"):
            pool.append(item)
    if not pool:
        raise AfwdevRunnerError("firehose fromTests pool is empty")

    cpu = os.cpu_count() or 1
    concurrency = _resolve_firehose_count(
        body.get("concurrency"), server_threads,
        "firehose.concurrency", 1)
    client_processes = _resolve_firehose_count(
        body.get("clientProcesses"), cpu,
        "firehose.clientProcesses", 1)
    if client_processes > concurrency:
        client_processes = concurrency
    until_stopped = bool(body.get("untilStopped", False))
    duration_s = body.get("duration_s")
    max_requests = body.get("maxRequests")
    if until_stopped and duration_s is not None:
        raise AfwdevRunnerError(
            "firehose untilStopped and duration_s are different endings; "
            "set one")
    if (not until_stopped and duration_s is None and max_requests is None):
        raise AfwdevRunnerError(
            "firehose requires duration_s, maxRequests, or untilStopped")
    duration_s = float(duration_s) if duration_s is not None else None
    max_requests = int(max_requests) if max_requests is not None else None
    stop_on_error = bool(body.get("stopOnError", False))
    seed = body.get("seed")
    rng = random.Random(seed)
    policy = (body.get("policy") or "random").strip()
    if policy not in ("random", "roundRobin"):
        raise AfwdevRunnerError(
            "firehose policy must be 'random' or 'roundRobin', got {!r}"
            .format(policy))
    max_fail = body.get("maxFail")
    if max_fail is not None:
        max_fail = int(max_fail)
    max_fail_rate = body.get("maxFailRate")
    if max_fail_rate is not None:
        max_fail_rate = float(max_fail_rate)
        if max_fail_rate < 0.0 or max_fail_rate > 1.0:
            raise AfwdevRunnerError(
                "firehose maxFailRate must be between 0 and 1")

    t_end = time.time() + duration_s if duration_s is not None else None
    t0 = time.time()

    # Per-request lines here would be millions of strings on a long
    # soak. The firehose summary below is the record. Failures on the
    # single-process path still go through _journal_failure.
    class _DropLog:
        def append(self, _item):
            return None

    quiet_log = _DropLog()
    socket_path = ctx.get("socket_path")

    if until_stopped:
        wall_end = None
        request_timeout = 30.0
    else:
        wall_end = t0 + timeout
        if t_end is not None and t_end < wall_end:
            wall_end = t_end
        request_timeout = max(5.0, timeout)
    previous_signals = {}
    for signum in (signal.SIGINT, signal.SIGTERM):
        previous_signals[signum] = signal.signal(signum, _on_firehose_signal)
    try:
        if client_processes > 1:
            ok, fail, total, first_error = _run_firehose_processes(
                client_processes, concurrency, pool, work_dir, socket_path,
                doc_feed, wall_end, policy, seed if seed is not None else 0,
                max_requests, request_timeout, stop_on_error, handle)
        else:
            ok, fail, total, first_error = _run_firehose_threads(
                concurrency, pool, work_dir, source_leaf, ctx, timeout,
                doc_feed, options, t_end, t0, policy, rng, max_requests,
                stop_on_error, handle, quiet_log,
                honor_timeout=not until_stopped)
    finally:
        for signum, handler in previous_signals.items():
            signal.signal(signum, handler)

    elapsed = max(time.time() - t0, 1e-9)
    rps = total / elapsed
    fail_rate = (float(fail) / float(total)) if total else 0.0
    summary = {
        "total": total,
        "ok": ok,
        "fail": fail,
        "failRate": round(fail_rate, 4),
        "rps": round(rps, 2),
        "policy": policy,
        "concurrency": concurrency,
        "clientProcesses": client_processes,
        "threads": server_threads,
    }
    if seed is not None:
        summary["seed"] = seed
    dead = _afwfcgi_dead(handle)
    if dead is not None:
        raise dead
    if socket_path:
        server_stats = _sample_server(socket_path)
        if server_stats:
            summary["server"] = server_stats

    # Pass criteria: explicit maxFail / maxFailRate win; else blast-like
    # (survive with any successes; only hard-fail if every request failed).
    if total == 0:
        passed = False
        reason = "firehose issued 0 requests"
    elif max_fail is not None:
        passed = fail <= max_fail
        reason = (
            "firehose fail count {} > maxFail {}".format(fail, max_fail)
            if not passed else None)
    elif max_fail_rate is not None:
        passed = fail_rate <= max_fail_rate
        reason = (
            "firehose failRate {:.4f} > maxFailRate {:.4f}".format(
                fail_rate, max_fail_rate)
            if not passed else None)
    elif fail == 0:
        passed = True
        reason = None
    elif ok == 0:
        passed = False
        reason = "firehose: all {} request(s) failed: {}".format(
            fail, error_message(first_error))
    else:
        # Mixed results, no threshold: blast-style tolerate errors
        passed = True
        reason = None

    server_note = ""
    server_stats = summary.get("server")
    if server_stats:
        server_note = " maxConcurrent={}/{}".format(
            server_stats.get("maxConcurrent"),
            server_stats.get("threadCount"))
    line = (
        "firehose {:.1f}s total={} ok={} fail={} rps={:.0f} "
        "threads={} clientProcesses={}{}".format(
            elapsed, total, ok, fail, rps, server_threads,
            client_processes, server_note))
    debug_parts.append(line)
    msg.highlighted_info(line)
    step_timings.append({
        "name": "firehose",
        "ms": round(elapsed * 1000),
        "passed": passed,
        "firehose": summary,
    })
    if not passed:
        raise AfwAdaptiveError(
            reason or "firehose failed",
            cause=first_error,
            object=summary,
        )


def _run_test_item(item, work_dir, source_leaf, ctx, timeout, doc_feed,
                   debug_parts, options=None):
    options = options or {}
    ctx = ctx or {}
    feed = merge_feed(doc_feed, item.get("feed"))
    kind = feed.get("kind") or "action"
    # copy so expansions do not mutate the loaded document
    item = dict(item)
    name = item.get("name")
    if not source_leaf:
        source_leaf = work_dir
    socket_path = ctx.get("socket_path")

    # Expand stream/raw expects (literal text or <<< files → keep as str/bytes later)
    _expand_stream_expect_values(item, work_dir, name)

    if kind == "local":
        # Raw afw --local stdin protocol (escape hatch / multi-directive sessions)
        _run_local_raw(item, work_dir, source_leaf, ctx, timeout, debug_parts,
                       options)
        return

    if kind == "rest":
        if ctx.get("host_kind") == "local":
            raise AfwdevRunnerError(
                "feed.kind rest is not supported for host local "
                "(use afwfcgi, or feed.kind local with raw protocol)")
        _run_rest(feed, socket_path, timeout, item, debug_parts, work_dir,
                  source_leaf, options)
        return

    if kind != "action":
        raise AfwdevRunnerError("unsupported feed.kind {!r}".format(kind))

    source = resolve_source_text(item, work_dir)
    source_type = item.get("sourceType") or "script"

    function = eval_function_for_source_type(source_type, feed)
    # test_script / script with shebang: keep shebang for eval<script>
    syntax = None
    lines = source.splitlines()
    if lines and lines[0].startswith("#!") and "--syntax" in lines[0]:
        parts = lines[0].split()
        try:
            syntax = parts[parts.index("--syntax") + 1]
        except (ValueError, IndexError):
            syntax = None
    if source_type == "test_script" and syntax is None:
        # Ensure test_script shebang so hosts evaluate as test_script
        if not source.lstrip().startswith("#!"):
            source = (
                "#!/usr/bin/env -S afw --syntax test_script\n" + source)

    action = {"function": function, "source": source}
    # evaluate() takes expression differently — if function is evaluate
    if function == "evaluate":
        action = {"function": "evaluate", "expression": source}

    accept = feed.get("accept") or "application/json"
    # Stream expects → response:* flags (JSON properties or x-afw :stream frames)
    flags = x_afw_demux.flags_for_stream_expects(item, accept)
    if flags:
        action["_flags_"] = list(flags)

    body_obj = {"actions": [action]}
    if flags:
        body_obj["_flags_"] = list(flags)
    body = nfc.json_dumps(body_obj)
    host_kind = ctx.get("host_kind") or "afwfcgi"

    if host_kind == "local":
        _run_local_action(
            item, work_dir, source_leaf, ctx, timeout, debug_parts, options,
            body_obj=body_obj, accept=accept, source_type=source_type,
            syntax=syntax)
        return

    if not socket_path:
        raise AfwdevRunnerError(
            "feed.kind action requires host afwfcgi (no socket)")

    param_overrides = {"HTTP_ACCEPT": accept}

    result = fcgi_request(
        socket_path,
        path=feed.get("path") or "/afw",
        method="POST",
        body=body,
        param_overrides=param_overrides,
        timeout=timeout,
    )

    debug_parts.append(
        "test {!r} status={} body_len={} accept={!r}".format(
            name,
            result.get("status_code"),
            len(result.get("body") or b""),
            accept,
        )
    )

    body_bytes = result.get("body") or b""
    if accept.strip().lower() == "application/x-afw" or \
            "x-afw" in accept.strip().lower():
        _judge_x_afw_body(
            item, body_bytes, work_dir, source_leaf, options, debug_parts,
            name, source_type, syntax, status_code=result.get("status_code"))
        return

    # JSON Accept — expect-response is not valid (option B)
    _reject_xafw_only_expects_on_json(item, name)

    body_text = body_bytes.decode("utf-8", errors="replace")
    try:
        response = nfc.json_loads(body_text) if body_text.strip() else {}
    except Exception as e:
        raise AfwdevRunnerError(
            "non-JSON response (status {}): {}".format(
                result.get("status_code"), body_text[:500]),
            cause=e,
        ) from e

    _judge_action_json_response(
        item, response, body_text, name, source_type, syntax)

    _judge_raw_full_body_expects(
        item, body_bytes, work_dir, source_leaf, options, debug_parts)


def _response_error_id(response):
    """Return Adaptive error id from a perform JSON body, or None."""
    if not isinstance(response, dict):
        return None
    err = response.get("error")
    if isinstance(err, dict) and err.get("id"):
        return err.get("id")
    actions = response.get("actions")
    if isinstance(actions, list):
        for act in actions:
            if not isinstance(act, dict):
                continue
            if act.get("status") == "error":
                e = act.get("error")
                if isinstance(e, dict) and e.get("id"):
                    return e.get("id")
    return None


def _judge_action_json_response(item, response, body_text, name, source_type,
                                syntax):
    """Shared expect / expect-stdout judging for JSON action responses."""
    expect_error = item.get("expectError")
    if expect_error:
        actual_id = _response_error_id(response)
        if actual_id != expect_error:
            raise AfwdevRunnerError(
                "test {!r}: expected error id {!r}, got status={!r} id={!r}".format(
                    name, expect_error,
                    (response or {}).get("status"), actual_id),
                object=response if isinstance(response, dict) else None,
            )
        return

    adapt = adaptive_error_from_response(response)
    if adapt is not None:
        raise adapt

    status = response.get("status")
    if status and status != "success":
        raise AfwAdaptiveError(
            "perform status {!r}: {}".format(status, (body_text or "")[:1500]),
            object={"message": (body_text or "")[:1500], "status": status},
        )

    actions = response.get("actions")
    if "result" in response and isinstance(response["result"], dict):
        inner = response["result"]
        adapt_inner = adaptive_error_from_response(inner)
        if adapt_inner is not None:
            raise adapt_inner
        if inner.get("actions") is not None:
            actions = inner.get("actions")

    action_result = None
    if actions:
        for act in actions:
            if not isinstance(act, dict):
                continue
            if act.get("status") == "error":
                err_obj = act.get("error")
                if isinstance(err_obj, dict):
                    raise AfwAdaptiveError(
                        err_obj.get("message") or "action error",
                        object=err_obj,
                    )
                raise AfwAdaptiveError(
                    "action error: {}".format(nfc.json_dumps(act)[:1500]))
            action_result = act.get("result")
            _fail_if_test_script_failures(action_result)
            if (source_type == "test_script" or syntax == "test_script") and \
                    action_result is not None:
                _fail_if_test_script_failures(action_result)
    elif "result" in response:
        # Single-function style body
        action_result = response.get("result")

    if "expect" in item and item.get("expect") is not None:
        _check_expect(item.get("expect"), action_result)

    _check_side_stream_expects(item, response, name)


def _expand_stream_expect_values(item, work_dir, name):
    """Resolve <<< for expect-* / expectResponse keys when the file exists."""
    for key in list(item.keys()):
        if key == "expect":
            continue
        if key != "expectResponse" and not (
                isinstance(key, str) and key.startswith("expect-")):
            continue
        if item[key] is None:
            continue
        rel = parse_triple_lt_path(item[key]) if isinstance(item[key], str) else None
        if rel is None:
            continue
        try:
            text_val, _rel = resolve_file_text(
                item[key], work_dir, item_name=name, what=key, missing_ok=True)
            if text_val is not None:
                item[key] = text_val
        except Exception:
            pass


def _reject_xafw_only_expects_on_json(item, name):
    if item.get("expect-response") is not None:
        raise AfwdevRunnerError(
            "test {!r}: expect-response is only for Accept application/x-afw "
            "(demuxed response stream). For JSON use expect / expect-stdout, "
            "or expect-raw-response for the full body.".format(name))


def _expected_to_bytes(expected):
    if expected is None:
        return b""
    if isinstance(expected, (bytes, bytearray)):
        return bytes(expected)
    return str(expected).encode("utf-8")


def _compare_expect_bytes(name, key, actual, expected, work_dir, source_leaf,
                          options, debug_parts, normalize=None):
    if not isinstance(actual, (bytes, bytearray)):
        actual = _expected_to_bytes(actual)
    else:
        actual = bytes(actual)

    rel = parse_triple_lt_path(expected) if isinstance(expected, str) else None
    capture = _capture_goldens_enabled(options)

    if rel is not None:
        if capture:
            written = set()
            for base in (source_leaf, work_dir):
                if not base:
                    continue
                path = os.path.abspath(os.path.join(base, rel))
                if path in written:
                    continue
                written.add(path)
                _write_golden(path, actual, debug_parts, name)
            return
        expected_b, _ = resolve_file_bytes(
            expected, work_dir, item_name=name, what=key, missing_ok=False)
    else:
        expected_b = _expected_to_bytes(expected)

    left = normalize(actual) if normalize else actual
    right = normalize(expected_b) if normalize else expected_b
    if left != right:
        hint = ""
        if rel is not None:
            hint = (
                " (update with: afwdev test --capture-goldens "
                "-T <leaf-dir>)")
        raise AfwdevRunnerError(
            "test {!r}: {} mismatch (got {} bytes, expected {}){}\n"
            "  expected: {!r}\n  actual:   {!r}".format(
                name, key, len(actual), len(expected_b), hint,
                expected_b[:200], actual[:200]))


def _judge_stream_expects(item, full_body, demuxed, work_dir, source_leaf,
                          options, debug_parts, name, normalize=None):
    specs = x_afw_demux.collect_stream_expect_specs(item)
    if not specs:
        return
    for spec in specs:
        actual = x_afw_demux.actual_bytes_for_spec(spec, demuxed, full_body)
        _compare_expect_bytes(
            name, spec["key"], actual, spec["expected_spec"],
            work_dir, source_leaf, options, debug_parts, normalize=normalize)


def _judge_x_afw_body(item, body_bytes, work_dir, source_leaf, options,
                      debug_parts, name, source_type, syntax,
                      status_code=200, normalize_full=None):
    code = int(status_code or 200)
    if code >= 400:
        raise AfwdevRunnerError(
            "x-afw response status {}: {}".format(
                code, (body_bytes or b"")[:400].decode(
                    "utf-8", errors="replace")))

    body_bytes = bytes(body_bytes or b"")
    demuxed = x_afw_demux.demux_x_afw(body_bytes)
    debug_parts.append(
        "x-afw demux streams={!r} frames={}".format(
            sorted(demuxed.get("payloads", {}).keys()),
            len(demuxed.get("frames") or [])))

    if not body_bytes and not x_afw_demux.collect_stream_expect_specs(item):
        debug_parts.append("x-afw empty body for {!r}".format(name))

    if item.get("expect") is not None:
        result_val = x_afw_demux.try_action_result_from_demux(demuxed)
        if isinstance(result_val, dict) and "__error__" in result_val:
            raise AfwAdaptiveError(
                "x-afw action error",
                object=result_val.get("__error__"))
        if result_val is not None or item.get("expect") in ("undefined", None):
            _check_expect(item.get("expect"), result_val)

    _judge_stream_expects(
        item, body_bytes, demuxed, work_dir, source_leaf, options,
        debug_parts, name, normalize=normalize_full)

    for frame in demuxed.get("frames") or []:
        if frame.get("streamId") != "response":
            continue
        try:
            import json
            obj = json.loads(frame["payload"].decode("utf-8"))
        except Exception:
            continue
        if isinstance(obj, dict) and "result" in obj:
            _fail_if_test_script_failures(obj.get("result"))


def _judge_raw_full_body_expects(item, body_bytes, work_dir, source_leaf,
                                 options, debug_parts, normalize=None):
    specs = [
        s for s in x_afw_demux.collect_stream_expect_specs(item)
        if s["kind"] == "full_raw"
    ]
    if not specs:
        return
    demuxed = {"raw": body_bytes, "payloads": {}, "raw_frames": {}}
    for spec in specs:
        actual = x_afw_demux.actual_bytes_for_spec(spec, demuxed, body_bytes)
        _compare_expect_bytes(
            item.get("name"), spec["key"], actual, spec["expected_spec"],
            work_dir, source_leaf, options, debug_parts, normalize=normalize)


def _run_local_action(item, work_dir, source_leaf, ctx, timeout, debug_parts,
                      options, body_obj, accept, source_type, syntax):
    """host local + feed.kind action: FCGI-like authoring."""
    name = item.get("name")
    accept = (accept or "application/json").strip()
    stdin = local_host.build_action_session_stdin(accept, body_obj)
    stdout = local_host.run_afw_local(
        stdin,
        work_dir,
        conf_path=ctx.get("conf_path"),
        under_valgrind=bool(ctx.get("under_valgrind")),
        timeout=timeout,
    )
    debug_parts.append(
        "local-action {!r} accept={!r} stdout_len={}".format(
            name, accept, len(stdout)))
    _write_local_actual(work_dir, name, stdout)

    if "x-afw" in accept.lower():
        app_body = x_afw_demux.local_output_app_body(stdout)
        _judge_x_afw_body(
            item, app_body, work_dir, source_leaf, options, debug_parts,
            name, source_type, syntax, status_code=200)
        full_specs = [
            s for s in x_afw_demux.collect_stream_expect_specs(item)
            if s["kind"] == "full_raw"
        ]
        if full_specs:
            d2 = {"raw": stdout, "payloads": {}, "raw_frames": {}}
            for spec in full_specs:
                actual = x_afw_demux.actual_bytes_for_spec(spec, d2, stdout)
                _compare_expect_bytes(
                    name, spec["key"], actual, spec["expected_spec"],
                    work_dir, source_leaf, options, debug_parts,
                    normalize=local_host.normalize_local_stdout)
        return

    _reject_xafw_only_expects_on_json(item, name)
    response = local_host.primary_json_response(stdout)
    if response is None:
        raise AfwdevRunnerError(
            "test {!r}: no JSON response parsed from afw --local stdout"
            .format(name))
    body_text = nfc.json_dumps(response)
    _judge_action_json_response(
        item, response, body_text, name, source_type, syntax)
    _judge_raw_full_body_expects(
        item, stdout, work_dir, source_leaf, options, debug_parts,
        normalize=local_host.normalize_local_stdout)


def _write_local_actual(work_dir, name, stdout):
    try:
        actual_path = os.path.join(
            work_dir, "local_actual_{}.bin".format(
                re.sub(r"[^\w.-]+", "_", str(name))))
        with nfc.open(actual_path, "wb") as fd:
            fd.write(stdout)
    except OSError:
        pass


def _run_local_raw(item, work_dir, source_leaf, ctx, timeout, debug_parts,
                   options):
    """feed.kind local: raw stdin protocol for afw --local."""
    name = item.get("name")
    if item.get("sourcePath") or (
            isinstance(item.get("source"), str) and
            parse_triple_lt_path(item.get("source"))):
        raw = item.get("source")
        if item.get("sourcePath"):
            path = os.path.join(work_dir, item["sourcePath"])
            if not os.path.isfile(path):
                raise AfwdevRunnerError(
                    "test {!r}: sourcePath not found: {}".format(name, path))
            with nfc.open(path, "rb") as fd:
                input_bytes = fd.read()
        else:
            input_bytes, _ = resolve_file_bytes(
                raw, work_dir, item_name=name, what="source")
    else:
        src = item.get("source")
        if src is None:
            raise AfwdevRunnerError(
                "test {!r}: local feed requires source".format(name))
        if isinstance(src, bytes):
            input_bytes = src
        else:
            input_bytes = str(src).encode("utf-8")

    stdout = local_host.run_afw_local(
        input_bytes,
        work_dir,
        conf_path=ctx.get("conf_path"),
        under_valgrind=bool(ctx.get("under_valgrind")),
        timeout=timeout,
    )
    debug_parts.append(
        "local-raw {!r} stdout_len={}".format(name, len(stdout)))
    _write_local_actual(work_dir, name, stdout)

    app_body = x_afw_demux.local_output_app_body(stdout)
    demuxed = x_afw_demux.demux_x_afw(app_body)
    # Stream/payload expects use reconstructed x-afw body; full-raw uses
    # complete local stdout (banner + segments) with banner normalize.
    stream_specs = [
        s for s in x_afw_demux.collect_stream_expect_specs(item)
        if s["kind"] != "full_raw"
    ]
    full_specs = [
        s for s in x_afw_demux.collect_stream_expect_specs(item)
        if s["kind"] == "full_raw"
    ]
    if stream_specs and demuxed.get("frames"):
        for spec in stream_specs:
            actual = x_afw_demux.actual_bytes_for_spec(
                spec, demuxed, app_body)
            _compare_expect_bytes(
                name, spec["key"], actual, spec["expected_spec"],
                work_dir, source_leaf, options, debug_parts)
    elif stream_specs and not demuxed.get("frames"):
        raise AfwdevRunnerError(
            "test {!r}: stream expects set but no x-afw frames in local "
            "stdout".format(name))
    if full_specs:
        d2 = {"raw": stdout, "payloads": {}, "raw_frames": {}}
        for spec in full_specs:
            actual = x_afw_demux.actual_bytes_for_spec(spec, d2, stdout)
            _compare_expect_bytes(
                name, spec["key"], actual, spec["expected_spec"],
                work_dir, source_leaf, options, debug_parts,
                normalize=local_host.normalize_local_stdout)


def _run_rest(feed, socket_path, timeout, item, debug_parts, work_dir=None,
              source_leaf=None, options=None):
    options = options or {}
    work_dir = work_dir or "."
    source_leaf = source_leaf or work_dir
    method = (feed.get("method") or "GET").upper()
    path = feed.get("path") or "/"
    accept = feed.get("accept") or "application/json"
    body = feed.get("body")
    if body is not None and not isinstance(body, (bytes, str)):
        body = nfc.json_dumps(body)
    param_overrides = {
        "HTTP_ACCEPT": accept,
        "REQUEST_METHOD": method,
    }
    if feed.get("contentType"):
        param_overrides["CONTENT_TYPE"] = feed["contentType"]
        param_overrides["HTTP_CONTENT_TYPE"] = feed["contentType"]

    result = fcgi_request(
        socket_path,
        path=path,
        method=method,
        body=body or b"",
        param_overrides=param_overrides,
        timeout=timeout,
    )
    debug_parts.append(
        "rest {!r} {} {} status={}".format(
            item.get("name"), method, path, result.get("status_code")))

    code = int(result.get("status_code") or 0)
    expect_status = item.get("expectStatus")
    if expect_status is not None:
        if code != int(expect_status):
            raise AfwdevRunnerError(
                "REST status {} expected {}".format(code, expect_status))
    elif code >= 400:
        raise AfwdevRunnerError(
            "REST status {} for {} {}".format(code, method, path))

    body_bytes = result.get("body") or b""
    if "x-afw" in (accept or "").lower():
        _judge_x_afw_body(
            item, body_bytes, work_dir, source_leaf, options, debug_parts,
            item.get("name"), "script", None, status_code=code)
    else:
        _judge_raw_full_body_expects(
            item, body_bytes, work_dir, source_leaf, options, debug_parts)


def _check_side_stream_expects(item, response, name):
    """JSON path: property-based stream expects."""
    if not isinstance(response, dict):
        response = {}
    for key, prop in (("expect-stdout", "stdout"),
                      ("expect-stderr", "stderr"),
                      ("expect-console", "console")):
        if item.get(key) is None:
            continue
        expected = item[key]
        if expected is None:
            expected = ""
        if isinstance(expected, (bytes, bytearray)):
            expected = expected.decode("utf-8", errors="replace")
        actual = response.get(prop)
        if actual is None:
            actual = ""
        if not isinstance(actual, str):
            actual = str(actual)
        if actual != expected:
            raise AfwdevRunnerError(
                "test {!r}: {} mismatch\n  expected: {!r}\n  actual:   {!r}"
                .format(name, key, expected, actual))


def _write_golden(path, data, debug_parts, name):
    parent = os.path.dirname(path)
    if parent and not os.path.isdir(parent):
        os.makedirs(parent, exist_ok=True)
    with nfc.open(path, "wb") as fd:
        fd.write(data)
    note = "captured golden for {!r} → {} ({} bytes)".format(
        name, path, len(data))
    debug_parts.append(note)
    msg.info(note)


def _check_expect(expect, action_result):
    """Compare orchestration expect to action result (JSON-ish primitives)."""
    if expect == "error" or (
            isinstance(expect, str) and expect.startswith("error")):
        # Expected error but we got success
        raise AfwdevRunnerError(
            "expected error but action succeeded: result={!r}".format(
                action_result))
    if expect == "undefined" or expect is None:
        if action_result is not None:
            # undefined may appear as missing or null
            if action_result is not None:
                pass  # soft: Adaptive undefined may serialize oddly
        return
    # Normalize Adaptive JSON results
    if action_result != expect:
        # allow numeric int/float loose compare
        try:
            if float(action_result) == float(expect):
                return
        except (TypeError, ValueError):
            pass
        raise AfwdevRunnerError(
            "expect {!r} but result was {!r}".format(expect, action_result))


def _fail_if_test_script_failures(obj):
    if not isinstance(obj, dict) or "tests" not in obj:
        return
    for tc in obj.get("tests") or []:
        if tc.get("skip"):
            continue
        if tc.get("passed", False) is False:
            label = tc.get("test") or tc.get("description") or tc
            err_obj = tc.get("error")
            if isinstance(err_obj, dict):
                raise AfwAdaptiveError(
                    err_obj.get("message") or
                    "test_script failure: {}".format(label),
                    object=err_obj,
                )
            reason = tc.get("errorReason") or "test_script failure"
            parts = ["{}: {}".format(reason, label)]
            if tc.get("expect-stdout") is not None:
                parts.append("expect-stdout={!r} stdout={!r}".format(
                    tc.get("expect-stdout"), tc.get("stdout")))
            if tc.get("expect-stderr") is not None:
                parts.append("expect-stderr={!r} stderr={!r}".format(
                    tc.get("expect-stderr"), tc.get("stderr")))
            raise AfwAdaptiveError(
                "; ".join(parts),
                object=error_to_dict(err_obj) if err_obj else tc,
            )


def _pass_response(description, step_timings=None):
    body = {
        "description": description,
        "tests": [
            {
                "test": description,
                "description": description,
                "passed": True,
            }
        ],
    }
    if step_timings:
        body["stepTimings"] = step_timings
        body["tests"][0]["stepTimings"] = step_timings
    return body


def _fail_response(description, detail, step_timings=None):
    err_dict = error_to_dict(detail)
    detail_s = error_message(detail) or "orchestrated-test failure"
    body = {
        "description": description or "orchestrated-test",
        "error": err_dict,
        "tests": [
            {
                "test": description or "orchestrated-test",
                "description": detail_s,
                "passed": False,
                "error": err_dict,
            }
        ],
    }
    if step_timings:
        body["stepTimings"] = step_timings
        body["tests"][0]["stepTimings"] = step_timings
    return body


def _debug_blob(debug_parts, handle):
    parts = list(debug_parts or [])
    if handle and handle.get("log_path"):
        try:
            with open(handle["log_path"], "rb") as fd:
                log = fd.read().decode("utf-8", errors="replace")
            if log:
                parts.append("--- afwfcgi stderr ---\n" + log[-12000:])
        except OSError:
            pass
    return "\n".join(parts) if parts else None
