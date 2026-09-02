#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""summarize_valgrind_xml — kind + top frames for afwdev test fails."""

from _afwdev.test.valgrind_report import (
    summarize_valgrind_xml,
    valgrind_error_message,
)
from _afwdev.test.runner import _with_buffered_stdio
from _afwdev.test.common import want_error_detail

_SAMPLE = """<?xml version="1.0"?>
<valgrindoutput>
<error>
  <unique>0x0</unique>
  <tid>1</tid>
  <kind>InvalidWrite</kind>
  <what>Invalid write of size 8</what>
  <stack>
    <frame>
      <ip>0x1</ip>
      <obj>/usr/local/lib/afw/libafwcurl.so</obj>
      <fn>afw_curl_internal_request_cb</fn>
      <dir>/workspaces/afw/src/afw_curl</dir>
      <file>afw_curl_internal.c</file>
      <line>262</line>
    </frame>
    <frame>
      <ip>0x2</ip>
      <fn>curl_easy_perform</fn>
      <obj>/usr/lib/x86_64-linux-gnu/libcurl.so.4</obj>
    </frame>
  </stack>
</error>
</valgrindoutput>
"""


def _case(name, description, passed, error=None):
    return {
        "test": name,
        "description": description,
        "passed": bool(passed),
        "skip": False,
        "error": error,
    }


def run():
    tests = []
    summary = summarize_valgrind_xml(_SAMPLE)
    tests.append(_case(
        "kind-and-frames",
        "summary names InvalidWrite and the curl READFUNCTION frame",
        passed=(
            summary is not None
            and "InvalidWrite" in summary
            and "Invalid write of size 8" in summary
            and "afw_curl_internal_request_cb" in summary
            and "afw_curl_internal.c:262" in summary
        ),
        error=None if summary and "InvalidWrite" in summary else summary,
    ))
    tests.append(_case(
        "no-error-is-none",
        "XML without <error> yields None",
        passed=summarize_valgrind_xml("<valgrindoutput/>") is None,
    ))
    tests.append(_case(
        "generic-fallback",
        "valgrind_error_message always names valgrind when XML has <error>",
        passed="Valgrind" in valgrind_error_message(_SAMPLE),
    ))

    def _captured_print():
        print("group-a")
        print("group-b")
        return 7

    captured_result, captured_text = _with_buffered_stdio(_captured_print)
    tests.append(_case(
        "buffer-group-stdio",
        "-j worker capture keeps a group's prints as one block",
        passed=(
            captured_result == 7
            and "group-a" in captured_text
            and "group-b" in captured_text
            and captured_text.find("group-a") < captured_text.find("group-b")
        ),
    ))
    tests.append(_case(
        "error-detail-flag",
        "--error-detail is fail extras; empty options is off",
        passed=(
            want_error_detail({"error_detail": True}) is True
            and want_error_detail({}) is False
        ),
    ))
    return {
        "description": "valgrind XML fail text for afwdev test",
        "tests": tests,
    }
