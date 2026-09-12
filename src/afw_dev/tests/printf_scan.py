#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Unit tests for AFW printf format slot parsing and type matching."""

from _afwdev.build.printf_scan import parse_format, type_ok


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

    def check_slots(fmt, expect):
        try:
            got = parse_format(fmt)
        except ValueError as e:
            tests.append(_case(
                "fmt-" + fmt, "parse " + fmt, False, str(e)))
            return
        tests.append(_case(
            "fmt-" + fmt,
            "parse " + fmt,
            got == expect,
            None if got == expect else "got %r" % (got,)))

    check_slots("hello", [])
    check_slots("%%", [])
    check_slots("%ku", ["utf8_ptr"])
    check_slots("Adapter '%ku' is not available", ["utf8_ptr"])
    check_slots("/%ku/%ku/%ku", ["utf8_ptr", "utf8_ptr", "utf8_ptr"])
    check_slots("%km", ["memory_ptr"])
    check_slots("%ks", ["cstr"])
    check_slots("%s", ["cstr"])
    check_slots("%.*s", ["int", "cstr"])
    check_slots("%d %s", ["int", "cstr"])
    check_slots("%*d", ["int", "int"])
    check_slots("%*.*f", ["int", "int", "float"])
    check_slots("%8ku", ["utf8_ptr"])
    check_slots("%*ku", ["int", "utf8_ptr"])
    check_slots("%.2ku", ["utf8_ptr"])

    try:
        parse_format("%n")
        tests.append(_case("fmt-%n", "%n rejected", False, "did not throw"))
    except ValueError:
        tests.append(_case("fmt-%n", "%n rejected", True))

    tests.append(_case(
        "type-ku-utf8",
        "%ku accepts const afw_utf8_t *",
        type_ok("utf8_ptr", "const afw_utf8_t *")))
    tests.append(_case(
        "type-ku-null",
        "%ku accepts NULL void *",
        type_ok("utf8_ptr", "void *")))
    tests.append(_case(
        "type-ku-rejects-int",
        "%ku rejects int",
        not type_ok("utf8_ptr", "int")))
    tests.append(_case(
        "type-ku-rejects-char",
        "%ku rejects char *",
        not type_ok("utf8_ptr", "const char *")))
    tests.append(_case(
        "type-km",
        "%km accepts afw_memory_t *",
        type_ok("memory_ptr", "const afw_memory_t *")))
    tests.append(_case(
        "type-ks",
        "%ks accepts char *",
        type_ok("cstr", "const char *")))

    return tests
