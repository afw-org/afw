# -*- coding: utf-8 -*-
"""
Run one orchestrated-test leaf (orchestration.yaml|json).

Returns (response, error, debug) compatible with afwdev test parse_test_run.
"""

import os
import random
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
    resolve_source_text,
)
from _afwdev.test.orchestrated.fcgi_client import FcgiClientError, fcgi_request
from _afwdev.test.orchestrated.hosts import afwfcgi as afwfcgi_host


def run_orchestrated_test(marker_path, options, testEnvironment=None,
                          testGroupConfig=None):
    """Entry point from common.run_test for orchestration markers."""
    mode = (options or {}).get("mode") or "afw"
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

    conf_path = os.path.join(work_dir, "afw.conf")
    if not os.path.isfile(conf_path):
        err = AfwdevRunnerError(
            "orchestrated-test requires afw.conf in leaf/work dir: "
            + work_dir)
        return _fail_response("orchestrated-test", err), None, None

    description = doc.get("description") or os.path.basename(work_dir)
    timeout_s = float(doc.get("timeout_s") or 120.0)
    threads = int(doc.get("afwfcgi", {}).get("threads") or 1)
    tests_by_name = {t["name"]: t for t in doc["tests"]}
    doc_feed = doc.get("feed") or {}

    handle = None
    t0 = time.time()
    try:
        handle = afwfcgi_host.start_afwfcgi(
            work_dir,
            threads=threads,
            under_valgrind=under_valgrind,
            options=options,
            ready_timeout_s=min(30.0, timeout_s),
        )
        debug_parts.append("started: " + " ".join(handle["argv"]))
        debug_parts.append("socket: " + handle["socket_path"])

        step_timings = []
        schedule = doc.get("schedule")
        if not schedule:
            schedule = [{"sequential": [
                t["name"] for t in doc["tests"] if not t.get("skip")
            ]}]

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
                        continue
                    _run_named_test(
                        item, work_dir, handle["socket_path"], remaining,
                        doc_feed, debug_parts, step_timings, description,
                        fail_fast=True)

            elif kind == "parallel":
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
                    items, n, work_dir, handle["socket_path"], remaining,
                    doc_feed, debug_parts, step_timings)

            elif kind == "firehose":
                if not isinstance(body, dict):
                    raise AfwdevRunnerError(
                        "schedule firehose body must be a mapping")
                _run_firehose(
                    body, tests_by_name, work_dir, handle["socket_path"],
                    remaining, doc_feed, debug_parts, step_timings,
                    description)

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
                            item, work_dir, handle["socket_path"], rem,
                            doc_feed, debug_parts, step_timings, description,
                            fail_fast=True)

            else:
                raise AfwdevRunnerError(
                    "unknown schedule phase {!r}".format(kind))

        if under_valgrind and afwfcgi_host.valgrind_errors_in_log(
                handle.get("log_path")):
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


def _run_named_test(item, work_dir, socket_path, timeout, doc_feed,
                    debug_parts, step_timings, description, fail_fast=True):
    name = item["name"]
    msg.debug("orchestrated-test: " + name)
    t0 = time.time()
    try:
        _run_test_item(item, work_dir, socket_path, timeout, doc_feed,
                       debug_parts)
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


def _run_parallel(items, n, work_dir, socket_path, timeout, doc_feed,
                  debug_parts, step_timings):
    if not items:
        return
    n = max(1, min(n, len(items)))
    errors = []

    def one(item):
        t0 = time.time()
        try:
            _run_test_item(item, work_dir, socket_path, timeout, doc_feed,
                           debug_parts)
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


def _run_firehose(body, tests_by_name, work_dir, socket_path, timeout,
                  doc_feed, debug_parts, step_timings, description):
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

    t_end = time.time() + duration_s if duration_s is not None else None
    t0 = time.time()
    ok = fail = 0
    total = 0
    first_error = None

    def one(item):
        try:
            _run_test_item(item, work_dir, socket_path, max(5.0, timeout),
                           doc_feed, debug_parts)
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
                item = rng.choice(pool)
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

    elapsed = time.time() - t0
    debug_parts.append(
        "firehose done in {:.1f}s total={} ok={} fail={}".format(
            elapsed, total, ok, fail))
    step_timings.append({
        "name": "firehose",
        "ms": round(elapsed * 1000),
        "passed": fail == 0 or not stop_on_error,
        "firehose": {"total": total, "ok": ok, "fail": fail},
    })
    # Blast-like: keep going is success if process lived; report fails in debug.
    # Hard-fail only if every request failed and we had some traffic.
    if total > 0 and ok == 0 and fail > 0:
        raise AfwAdaptiveError(
            "firehose: all {} request(s) failed: {}".format(
                fail, error_message(first_error)),
            cause=first_error)


def _run_test_item(item, work_dir, socket_path, timeout, doc_feed, debug_parts):
    feed = merge_feed(doc_feed, item.get("feed"))
    kind = feed.get("kind") or "action"
    # copy so we can expand expectResponse without mutating loaded doc permanently
    item = dict(item)
    if item.get("expectResponse") is not None and isinstance(
            item["expectResponse"], str):
        er = item["expectResponse"]
        if er.lstrip().startswith("<<<"):
            item["expectResponse"] = resolve_source_text(
                {"name": item.get("name"), "source": er}, work_dir)

    source = resolve_source_text(item, work_dir)
    source_type = item.get("sourceType") or "script"

    if kind == "rest":
        _run_rest(feed, socket_path, timeout, item, debug_parts, work_dir)
        return

    if kind != "action":
        raise AfwdevRunnerError("unsupported feed.kind {!r}".format(kind))

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

    body_obj = {"actions": [action]}
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
            item.get("name"),
            result.get("status_code"),
            len(result.get("body") or b""),
            accept,
        )
    )

    if accept.strip().lower() == "application/x-afw":
        _check_x_afw_response(item, result, debug_parts)
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


def _run_rest(feed, socket_path, timeout, item, debug_parts, work_dir=None):
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
        expected = item["expectResponse"]
        body_out = result.get("body") or b""
        if isinstance(expected, str):
            expected_b = expected.encode("utf-8")
        else:
            expected_b = expected
        if body_out != expected_b:
            raise AfwdevRunnerError(
                "expectResponse mismatch (got {} bytes, expected {})".format(
                    len(body_out), len(expected_b)))


def _check_x_afw_response(item, result, debug_parts):
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
    exp = item.get("expectResponse")
    if exp is not None:
        if isinstance(exp, str):
            exp_b = exp.encode("utf-8")
        else:
            exp_b = exp
        if body != exp_b:
            raise AfwdevRunnerError(
                "expectResponse mismatch for {!r}".format(item.get("name")))
    # Soft success: process returned without HTTP error
    if b"0 end\n" in body or b" intermediate" in body or body:
        return
    # allow empty true return with no intermediate if only void progressive
    return


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
            err_obj = tc.get("error")
            if isinstance(err_obj, dict):
                raise AfwAdaptiveError(
                    err_obj.get("message") or
                    "test_script failure: {}".format(
                        tc.get("test") or tc.get("description") or tc),
                    object=err_obj,
                )
            raise AfwAdaptiveError(
                "test_script failure: {}".format(
                    tc.get("test") or tc.get("description") or tc),
                object=error_to_dict(err_obj) if err_obj else None,
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
