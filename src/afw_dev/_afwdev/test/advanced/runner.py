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
        return _fail_response(str(marker_path), str(e)), None, None

    work_dir = None
    if testEnvironment and testEnvironment.get("work_dir"):
        work_dir = testEnvironment["work_dir"]
    else:
        work_dir = os.path.dirname(marker_path)

    conf_path = os.path.join(work_dir, "afw.conf")
    if not os.path.isfile(conf_path):
        err = "advanced-test requires afw.conf in leaf/work dir: " + work_dir
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

        for step in steps:
            if time.time() - t0 > timeout_s:
                err = "advanced-test timed out after {}s (step {!r})".format(
                    timeout_s, step.get("name"))
                return (
                    _fail_response(description, err),
                    None,
                    _debug_blob(debug_parts, handle),
                )

            step_name = step["name"]
            msg.debug("advanced-test step: " + step_name)
            remaining = max(1.0, timeout_s - (time.time() - t0))
            try:
                _run_step(
                    step,
                    work_dir=work_dir,
                    socket_path=handle["socket_path"],
                    timeout=remaining,
                    debug_parts=debug_parts,
                )
            except Exception as e:
                err = "step {!r} failed: {}".format(step_name, e)
                debug_parts.append(err)
                return (
                    _fail_response(description, err),
                    None,
                    _debug_blob(debug_parts, handle),
                )

        if under_valgrind and afwfcgi_host.valgrind_errors_in_log(
                handle.get("log_path")):
            err = "Valgrind error(s) detected in afwfcgi stderr"
            return (
                _fail_response(description, err),
                None,
                _debug_blob(debug_parts, handle),
            )

        return (
            _pass_response(description),
            None,
            _debug_blob(debug_parts, handle),
        )

    except afwfcgi_host.AfwfcgiHostError as e:
        # Host spawn failures: surface as error for digest path identity
        return (
            _fail_response(description, str(e)),
            str(e),
            _debug_blob(debug_parts, handle),
        )
    except Exception as e:
        return (
            _fail_response(description, str(e)),
            str(e),
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
            raise RuntimeError("script not found: " + script_path)
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
    except FcgiClientError as e:
        raise RuntimeError(str(e)) from e

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
        raise RuntimeError(
            "non-JSON response from afwfcgi (HTTP-ish status {}): {}".format(
                result.get("status_code"), body_text[:500])) from e

    status = response.get("status")
    if status and status != "success":
        raise RuntimeError(
            "perform status {!r}: {}".format(
                status, body_text[:1500]))

    if response.get("status") == "error":
        raise RuntimeError(
            "action error: {}".format(
                nfc.json_dumps(response.get("error") or response)[:1500]))

    actions = response.get("actions")
    if "result" in response and isinstance(response["result"], dict):
        inner = response["result"]
        if inner.get("status") == "error":
            raise RuntimeError(
                "action error: {}".format(
                    nfc.json_dumps(inner.get("error") or inner)[:1500]))
        if inner.get("actions") is not None:
            actions = inner.get("actions")

    # Collect test_script result objects from action results
    def _fail_if_test_script_failures(obj):
        if not isinstance(obj, dict) or "tests" not in obj:
            return
        for tc in obj.get("tests") or []:
            if tc.get("skip"):
                continue
            if tc.get("passed", False) is False:
                raise RuntimeError(
                    "test_script failure: {}".format(
                        tc.get("test") or tc.get("description") or tc))

    if actions:
        for act in actions:
            if not isinstance(act, dict):
                continue
            if act.get("status") == "error":
                raise RuntimeError(
                    "action error: {}".format(
                        nfc.json_dumps(act.get("error") or act)[:1500]))
            ar = act.get("result")
            # Always inspect for tests[] (eval of test_script source)
            _fail_if_test_script_failures(ar)
            if syntax == "test_script" and ar is not None:
                _fail_if_test_script_failures(ar)


def _pass_response(description):
    return {
        "description": description,
        "tests": [
            {
                "test": description,
                "description": description,
                "passed": True,
            }
        ],
    }


def _fail_response(description, detail):
    return {
        "description": description or "advanced-test",
        "tests": [
            {
                "test": description or "advanced-test",
                "description": detail,
                "passed": False,
                "error": detail,
            }
        ],
    }


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
