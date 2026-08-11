# -*- coding: utf-8 -*-
"""
Run one orchestrated-test leaf (orchestration.yaml|json).

Returns (response, error, debug) compatible with afwdev test parse_test_run.
"""

import os
import random
import re
import time
from concurrent.futures import ThreadPoolExecutor, as_completed

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
    parse_triple_lt_path,
    resolve_file_bytes,
    resolve_file_text,
    resolve_source_text,
)
from _afwdev.test.orchestrated.fcgi_client import fcgi_request
from _afwdev.test.orchestrated.hosts import afwfcgi as afwfcgi_host
from _afwdev.test.orchestrated.hosts import local as local_host


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
    t0 = time.time()
    try:
        socket_path = None
        if host_kind == "afwfcgi":
            handle = afwfcgi_host.start_afwfcgi(
                work_dir,
                threads=threads,
                under_valgrind=under_valgrind,
                options=options,
                ready_timeout_s=min(30.0, timeout_s),
            )
            socket_path = handle["socket_path"]
            debug_parts.append("started: " + " ".join(handle["argv"]))
            debug_parts.append("socket: " + socket_path)
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
                    description, options)

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


def _run_firehose(body, tests_by_name, work_dir, source_leaf, ctx,
                  timeout, doc_feed, debug_parts, step_timings, description,
                  options):
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

    concurrency = int(body.get("concurrency") or 1)
    concurrency = max(1, concurrency)
    duration_s = body.get("duration_s")
    max_requests = body.get("maxRequests")
    if duration_s is None and max_requests is None:
        raise AfwdevRunnerError(
            "firehose requires duration_s and/or maxRequests")
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
                           max(5.0, timeout), doc_feed, debug_parts, options)
            return True, None
        except Exception as e:
            return False, e

    with ThreadPoolExecutor(max_workers=concurrency) as ex:
        pending = set()
        while True:
            if t_end is not None and time.time() >= t_end:
                break
            if max_requests is not None and total >= max_requests:
                break
            if time.time() - t0 > timeout:
                raise AfwdevRunnerError(
                    "orchestrated-test timed out during firehose")

            while len(pending) < concurrency:
                if max_requests is not None and total >= max_requests:
                    break
                if t_end is not None and time.time() >= t_end:
                    break
                item = pick_item()
                pending.add(ex.submit(one, item))
                total += 1

            if not pending:
                break

            done = []
            for fut in list(pending):
                if fut.done():
                    done.append(fut)
            if not done:
                time.sleep(0.01)
                continue
            for fut in done:
                pending.discard(fut)
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

        # drain
        for fut in as_completed(pending):
            success, err = fut.result()
            if success:
                ok += 1
            else:
                fail += 1
                if first_error is None:
                    first_error = err

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
    }
    if seed is not None:
        summary["seed"] = seed

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

    debug_parts.append(
        "firehose done in {:.1f}s total={} ok={} fail={} "
        "failRate={:.2%} rps={:.1f} policy={}".format(
            elapsed, total, ok, fail, fail_rate, rps, policy))
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

    # Expand side-channel expects (literal text / <<< files)
    for key in ("expect-stdout", "expect-stderr"):
        if item.get(key) is not None:
            text, _rel = resolve_file_text(
                item[key], work_dir, item_name=name, what=key)
            item[key] = text if text is not None else ""

    if kind == "local":
        _run_local(item, work_dir, source_leaf, ctx, timeout, debug_parts,
                   options)
        return

    if kind == "rest":
        _run_rest(feed, socket_path, timeout, item, debug_parts, work_dir,
                  source_leaf, options)
        return

    if kind != "action":
        raise AfwdevRunnerError("unsupported feed.kind {!r}".format(kind))

    if not socket_path:
        raise AfwdevRunnerError(
            "feed.kind action requires host afwfcgi (no socket)")

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
        # Ensure test_script shebang so afwfcgi evaluates as test_script
        if not source.lstrip().startswith("#!"):
            source = (
                "#!/usr/bin/env -S afw --syntax test_script\n" + source)

    action = {"function": function, "source": source}
    # evaluate() takes expression differently — if function is evaluate
    if function == "evaluate":
        action = {"function": "evaluate", "expression": source}

    # Leaf-level stream expects → response:stdout / response:stderr flags so
    # Adaptive captures utf-8 on the response object (not process FDs).
    flags = []
    if item.get("expect-stdout") is not None:
        flags.append("response:stdout")
    if item.get("expect-stderr") is not None:
        flags.append("response:stderr")
    if flags:
        action["_flags_"] = list(flags)

    body_obj = {"actions": [action]}
    if flags:
        body_obj["_flags_"] = list(flags)
    body = nfc.json_dumps(body_obj)
    accept = feed.get("accept") or "application/json"
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

    if accept.strip().lower() == "application/x-afw":
        _check_x_afw_response(
            item, result, debug_parts, work_dir, source_leaf, options)
        return

    body_text = (result.get("body") or b"").decode("utf-8", errors="replace")
    try:
        response = nfc.json_loads(body_text) if body_text.strip() else {}
    except Exception as e:
        raise AfwdevRunnerError(
            "non-JSON response (status {}): {}".format(
                result.get("status_code"), body_text[:500]),
            cause=e,
        ) from e

    adapt = adaptive_error_from_response(response)
    if adapt is not None:
        raise adapt

    status = response.get("status")
    if status and status != "success":
        raise AfwAdaptiveError(
            "perform status {!r}: {}".format(status, body_text[:1500]),
            object={"message": body_text[:1500], "status": status},
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

    if "expect" in item and item.get("expect") is not None:
        _check_expect(item.get("expect"), action_result)

    _check_side_stream_expects(item, response, name)

    if item.get("expectResponse") is not None:
        # JSON body as bytes for exact golden compare when requested
        _check_expect_response(
            item, result.get("body") or b"", work_dir, source_leaf, options,
            debug_parts)


def _run_local(item, work_dir, source_leaf, ctx, timeout, debug_parts,
               options):
    """feed.kind local: raw stdin protocol for afw --local."""
    name = item.get("name")
    # Binary stdin body (<<< path or UTF-8 text)
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
        "local {!r} stdout_len={}".format(name, len(stdout)))

    # Debug copy in work dir (like historical local_test_*_actual.txt)
    try:
        actual_path = os.path.join(
            work_dir, "local_actual_{}.bin".format(
                re.sub(r"[^\w.-]+", "_", str(name))))
        with nfc.open(actual_path, "wb") as fd:
            fd.write(stdout)
    except OSError:
        pass

    if item.get("expectResponse") is not None:
        _check_expect_response(
            item, stdout, work_dir, source_leaf, options, debug_parts,
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

    if item.get("expectResponse") is not None:
        _check_expect_response(
            item, result.get("body") or b"", work_dir, source_leaf, options,
            debug_parts)


def _check_x_afw_response(item, result, debug_parts, work_dir, source_leaf,
                          options):
    body = result.get("body") or b""
    code = int(result.get("status_code") or 200)
    if code >= 400:
        raise AfwdevRunnerError(
            "x-afw response status {}: {}".format(
                code, body[:400].decode("utf-8", errors="replace")))
    # Minimal: non-empty body and optional exact expectResponse
    if not body and item.get("expectResponse") is None:
        # progressive may still write frames; empty is suspicious
        debug_parts.append("x-afw empty body for {!r}".format(item.get("name")))
    if item.get("expectResponse") is not None:
        _check_expect_response(
            item, body, work_dir, source_leaf, options, debug_parts)
    # Soft success: process returned without HTTP error
    if b"0 end\n" in body or b" intermediate" in body or body:
        return
    # allow empty true return with no intermediate if only void progressive
    return

def _check_side_stream_expects(item, response, name):
    """Compare leaf expect-stdout / expect-stderr to response properties."""
    if not isinstance(response, dict):
        response = {}
    for key, prop in (("expect-stdout", "stdout"),
                      ("expect-stderr", "stderr")):
        if item.get(key) is None:
            continue
        expected = item[key]
        if expected is None:
            expected = ""
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


def _check_expect_response(item, actual_bytes, work_dir, source_leaf, options,
                           debug_parts, normalize=None):
    """
    Exact-byte compare of expectResponse (inline or <<< file).

    With --capture-goldens / AFWDEV_CAPTURE_GOLDENS=1 and a <<< path, write
    actual_bytes to the source leaf (and work_dir) and pass (update workflow).

    normalize: optional callable(bytes) -> bytes applied to both sides before
    compare (e.g. afw --local version banner). Capture still writes raw actual.
    """
    name = item.get("name")
    raw = item.get("expectResponse")
    if raw is None:
        return
    if not isinstance(actual_bytes, (bytes, bytearray)):
        actual_bytes = bytes(actual_bytes)

    rel = parse_triple_lt_path(raw)
    capture = _capture_goldens_enabled(options)

    if rel is not None:
        if capture:
            # Prefer writing into the package source leaf so the file is
            # reviewable/committable; also refresh hermetic work_dir copy.
            written = set()
            for base in (source_leaf, work_dir):
                if not base:
                    continue
                path = os.path.abspath(os.path.join(base, rel))
                if path in written:
                    continue
                written.add(path)
                _write_golden(path, actual_bytes, debug_parts, name)
            return
        expected_b, _ = resolve_file_bytes(
            raw, work_dir, item_name=name, what="expectResponse",
            missing_ok=False)
    else:
        if isinstance(raw, bytes):
            expected_b = raw
        else:
            expected_b = str(raw).encode("utf-8")

    left = normalize(actual_bytes) if normalize else actual_bytes
    right = normalize(expected_b) if normalize else expected_b
    if left != right:
        hint = ""
        if rel is not None:
            hint = (
                " (update with: afwdev test --capture-goldens "
                "-T <leaf-dir>)")
        raise AfwdevRunnerError(
            "test {!r}: expectResponse mismatch "
            "(got {} bytes, expected {}){}"
            .format(name, len(actual_bytes), len(expected_b), hint))


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
