# -*- coding: utf-8 -*-
"""
Run one advanced-test leaf (marker path).

Returns (response, error, debug) compatible with afwdev test parse_test_run:
  - one synthetic test case for one leaf pass/fail
  - response None + error None => skipped
"""

import os
import time

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
from _afwdev.test.advanced.load import (
    AdvancedTestLoadError,
    load_advanced_test_document,
)
from _afwdev.test.advanced.fcgi_client import FcgiClientError, fcgi_request
from _afwdev.test.advanced.hosts import afwfcgi as afwfcgi_host


def run_advanced_test(marker_path, options, testEnvironment=None,
                      testGroupConfig=None):
    """
    Entry point from common.run_test for advanced-test markers.
    """
    mode = (options or {}).get("mode") or "afw"
    debug_parts = []

    if mode == "actions":
        msg.debug("Skipping advanced-test under --env-mode actions: " +
                  marker_path)
        return None, None, None

    if mode == "afwfcgi":
        # Private conf / hermetic leaf — skip (same spirit as .as + afw.conf)
        msg.debug(
            "Skipping advanced-test under --env-mode afwfcgi "
            "(live stack; use default env-mode for hermetic leaves): "
            + marker_path)
        return None, None, None

    under_valgrind = (mode == "valgrind")

    try:
        doc = load_advanced_test_document(marker_path)
    except AdvancedTestLoadError as e:
        # Assertion-style fail (no separate error) so counts stay 1:1
        return _fail_response(str(marker_path), e), None, None

    work_dir = None
    if testEnvironment and testEnvironment.get("work_dir"):
        work_dir = testEnvironment["work_dir"]
    else:
        work_dir = os.path.dirname(marker_path)

    conf_path = os.path.join(work_dir, "afw.conf")
    if not os.path.isfile(conf_path):
        err = AfwdevRunnerError(
            "advanced-test requires afw.conf in leaf/work dir: " + work_dir)
        return _fail_response("advanced-test", err), None, None

    description = doc.get("description") or os.path.basename(work_dir)
    timeout_s = float(doc.get("timeout_s") or 120.0)
    threads = int(doc.get("afwfcgi", {}).get("threads") or 1)
    steps = doc["steps"]

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
        for step in steps:
            if time.time() - t0 > timeout_s:
                err = AfwdevRunnerError(
                    "advanced-test timed out after {}s (step {!r})".format(
                        timeout_s, step.get("name")))
                return (
                    _fail_response(description, err, step_timings),
                    None,
                    _debug_blob(debug_parts, handle),
                )

            step_name = step["name"]
            msg.debug("advanced-test step: " + step_name)
            remaining = max(1.0, timeout_s - (time.time() - t0))
            step_t0 = time.time()
            try:
                _run_step(
                    step,
                    work_dir=work_dir,
                    socket_path=handle["socket_path"],
                    timeout=remaining,
                    debug_parts=debug_parts,
                )
                step_ms = round((time.time() - step_t0) * 1000)
                step_timings.append({
                    "name": step_name,
                    "ms": step_ms,
                    "passed": True,
                })
                debug_parts.append(
                    "step {!r} ok ({}ms)".format(step_name, step_ms))
            except Exception as e:
                step_ms = round((time.time() - step_t0) * 1000)
                step_timings.append({
                    "name": step_name,
                    "ms": step_ms,
                    "passed": False,
                })
                wrapped = wrap_exception(e, default_cls=AfwAdaptiveError)
                if not isinstance(wrapped, AfwdevError):
                    wrapped = AfwAdaptiveError(
                        "step {!r} failed: {}".format(
                            step_name, error_message(e)),
                        cause=e)
                else:
                    # Prefix step name for digests while keeping Adaptive object
                    wrapped = type(wrapped)(
                        "step {!r} failed: {}".format(
                            step_name, error_message(wrapped)),
                        object=getattr(wrapped, "object", None),
                        cause=e,
                    )
                debug_parts.append(error_message(wrapped))
                return (
                    _fail_response(description, wrapped, step_timings),
                    None,
                    _debug_blob(debug_parts, handle),
                )

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
        # Host spawn failures: surface as error for digest path identity
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


def _run_step(step, work_dir, socket_path, timeout, debug_parts):
    source = step.get("eval")
    if source is None:
        script_rel = step["script"]
        script_path = os.path.join(work_dir, script_rel)
        if not os.path.isfile(script_path):
            raise AfwdevRunnerError("script not found: " + script_path)
        with nfc.open(script_path, "r") as fd:
            source = fd.read()

    # Keep shebang in source: eval<script> honors --syntax test_script when
    # present (same as modes/afwfcgi.py). Still parse syntax for checks.
    syntax = None
    lines = source.splitlines()
    if lines and lines[0].startswith("#!") and "--syntax" in lines[0]:
        parts = lines[0].split()
        try:
            syntax = parts[parts.index("--syntax") + 1]
        except (ValueError, IndexError):
            syntax = None

    body_obj = {
        "actions": [
            {
                "function": "eval<script>",
                "source": source,
            }
        ]
    }
    body = nfc.json_dumps(body_obj)

    try:
        result = fcgi_request(
            socket_path,
            path="/afw",
            method="POST",
            body=body,
            timeout=timeout,
        )
    except FcgiClientError:
        raise

    debug_parts.append(
        "step {!r} status={} body_len={}".format(
            step.get("name"),
            result.get("status_code"),
            len(result.get("body") or b""),
        )
    )
    if result.get("stderr"):
        debug_parts.append(
            "fcgi stderr: " +
            result["stderr"].decode("utf-8", errors="replace")[:2000])

    body_text = (result.get("body") or b"").decode("utf-8", errors="replace")
    try:
        response = nfc.json_loads(body_text) if body_text.strip() else {}
    except Exception as e:
        raise AfwdevRunnerError(
            "non-JSON response from afwfcgi (HTTP-ish status {}): {}".format(
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
                    "action error: {}".format(
                        nfc.json_dumps(act)[:1500]),
                    object=error_to_dict(err_obj) if err_obj else None,
                )
            ar = act.get("result")
            _fail_if_test_script_failures(ar)
            if syntax == "test_script" and ar is not None:
                _fail_if_test_script_failures(ar)


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
    """
    Build a test_script-shaped failure response.
    ``detail`` may be AfwdevError, Exception, or str.
    """
    err_dict = error_to_dict(detail)
    detail_s = error_message(detail) or "advanced-test failure"
    body = {
        "description": description or "advanced-test",
        "error": err_dict,
        "tests": [
            {
                "test": description or "advanced-test",
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
