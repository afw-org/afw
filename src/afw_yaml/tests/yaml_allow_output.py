#!/usr/bin/env python3
"""
YAML content-type output via `afw --allow` / `-a` (issue #14).

Why Python (not .as)?
  YAML is a content-type extension with no Adaptive Script `yaml()` converter.
  Issue #14 is the CLI path that prints evaluated values through
  `afw_content_type_value_to_raw` using `--allow`. Spawning `afw` is the
  natural regression surface.

Coverage:
  - `-a yaml` and `-a application/x-yaml` media type aliases
  - literal block scalar (`|`) for strings containing newlines (chomping,
    indent indicator for leading spaces) — core #14 fix
  - integer (and other primitive) YAML encoding from the same work
  - objects combining multiline strings + integers
  - invalid `--allow` rejected
  - default JSON path still works (sanity)
  - `-t yaml` can parse a YAML conf list (input side of the content type)
"""

from __future__ import annotations

import os
import re
import subprocess
import tempfile


def _case(name, description, passed, detail=None, skip=False):
    t = {
        "test": name,
        "description": description,
        "passed": bool(passed),
        "skip": bool(skip),
    }
    if detail and not passed:
        t["error"] = detail
    return t


def _run_afw(args, timeout=30):
    """Run afw; return (returncode, stdout_text, stderr_text)."""
    r = subprocess.run(
        ["afw", *args],
        capture_output=True,
        text=True,
        timeout=timeout,
    )
    return r.returncode, r.stdout, r.stderr


def _yaml_expr(expression, allow="yaml", extra=None):
    """
    Evaluate an Adaptive expression and return YAML (or other) stdout body.

    `afw` prints the content-type encoding via impl_print_result, which always
    appends one trailing newline. That outer newline is stripped here so
    comparisons match the content-type payload.
    """
    args = ["-e", "afw_yaml", "-a", allow, "-s", "expression", "-x", expression]
    if extra:
        args = list(extra) + args
    code, out, err = _run_afw(args)
    body = out[:-1] if out.endswith("\n") else out
    return code, body, err


def _expect_equal(name, description, expression, expected, allow="yaml"):
    code, body, err = _yaml_expr(expression, allow=allow)
    if code != 0:
        return _case(
            name,
            description,
            False,
            detail="exit %s; stderr tail: %s"
            % (code, err[-400:].replace("\n", " | ")),
        )
    if body != expected:
        return _case(
            name,
            description,
            False,
            detail="expected %r got %r" % (expected, body),
        )
    return _case(name, description, True)


def _expect_match(name, description, expression, pattern, allow="yaml"):
    code, body, err = _yaml_expr(expression, allow=allow)
    if code != 0:
        return _case(
            name,
            description,
            False,
            detail="exit %s; stderr tail: %s"
            % (code, err[-400:].replace("\n", " | ")),
        )
    if not re.search(pattern, body, re.DOTALL):
        return _case(
            name,
            description,
            False,
            detail="pattern %r not in %r" % (pattern, body),
        )
    return _case(name, description, True)


def run():
    description = (
        "afw --allow YAML output (issue #14: block scalars, integers, media types)"
    )
    tests = []

    # --- media type / --allow plumbing ------------------------------------

    tests.append(
        _expect_equal(
            "allow_short_yaml_integer",
            "issue #14: -a yaml encodes integer without quotes",
            "integer(42)",
            "---42",
            allow="yaml",
        )
    )

    tests.append(
        _expect_equal(
            "allow_media_type_application_x_yaml",
            "issue #14: -a application/x-yaml is accepted (media type)",
            "integer(99)",
            "---99",
            allow="application/x-yaml",
        )
    )

    # invalid content type
    code, out, err = _run_afw(
        ["-e", "afw_yaml", "-a", "not_a_content_type", "-s", "expression", "-x", "1"]
    )
    tests.append(
        _case(
            "allow_invalid_rejected",
            "issue #14: invalid --allow content type exits with error",
            code != 0 and "Invalid --allow content-type." in err,
            detail="exit=%s stderr=%r" % (code, err[-300:]),
        )
    )

    # default json path still works (no yaml extension needed)
    code, out, err = _run_afw(["-a", "json", "-s", "expression", "-x", "integer(7)"])
    body = out[:-1] if out.endswith("\n") else out
    tests.append(
        _case(
            "allow_json_default_path",
            "issue #14: -a json still prints plain JSON numbers",
            code == 0 and body == "7",
            detail="exit=%s body=%r" % (code, body),
        )
    )

    # --- integer / numeric / boolean / null (issue #14 follow-ons) --------

    tests.append(
        _expect_equal(
            "integer_zero",
            "YAML integer 0",
            "0",
            "---0",
        )
    )

    # Document start "---" + signed integer yields "----42" (current encoding).
    tests.append(
        _expect_equal(
            "integer_negative",
            "YAML negative integer (--- document start + signed value)",
            "integer(-42)",
            "----42",
        )
    )

    tests.append(
        _expect_equal(
            "boolean_true",
            "YAML boolean true",
            "true",
            "---true",
        )
    )

    tests.append(
        _expect_equal(
            "boolean_false",
            "YAML boolean false",
            "false",
            "---false",
        )
    )

    tests.append(
        _expect_equal(
            "null_value",
            "YAML null",
            "null",
            "---null",
        )
    )

    # doubles use JSON-like formatting; just require unquoted finite form
    tests.append(
        _expect_match(
            "double_finite",
            "YAML double is emitted as an unquoted number",
            "3.5",
            r"^---3\.5",
        )
    )

    # --- strings: simple vs literal block scalar (core #14) ----------------

    tests.append(
        _expect_equal(
            "string_no_newline_json_quoted",
            "issue #14: string without newline uses JSON-quoted style",
            'string("hello")',
            '---"hello"',
        )
    )

    tests.append(
        _expect_equal(
            "string_empty",
            "empty string is JSON-quoted",
            'string("")',
            '---""',
        )
    )

    # Strip chomping (`|-`): value does not end with newline
    tests.append(
        _expect_equal(
            "string_multiline_strip_chomp",
            "issue #14: multiline string without trailing NL uses |- block",
            'string("a\\nb")',
            "---|-\n  a\n  b",
        )
    )

    # Keep chomping (`|+`): value ends with newline
    tests.append(
        _expect_equal(
            "string_multiline_keep_chomp",
            "issue #14: multiline string with trailing NL uses |+ block",
            'string("a\\nb\\n")',
            "---|+\n  a\n  b\n  ",
        )
    )

    # Leading spaces → indentation indicator after `|`
    tests.append(
        _expect_equal(
            "string_multiline_indent_indicator",
            "issue #14: leading space sets literal indent indicator",
            'string("  lead\\n")',
            "---|1+\n    lead\n  ",
        )
    )

    # Must not fall back to escaped JSON for multiline
    code, body, err = _yaml_expr('string("line1\\nline2")')
    looks_like_block = body.startswith("---|") and "\n" in body
    has_json_escape = r"\n" in body and not body.startswith("---|")
    tests.append(
        _case(
            "string_multiline_not_json_escaped",
            "issue #14: multiline must use block style, not JSON \\n escapes",
            code == 0 and looks_like_block and not has_json_escape,
            detail="body=%r" % (body,),
        )
    )

    # --- composite values -------------------------------------------------

    tests.append(
        _expect_equal(
            "object_multiline_and_integer",
            "issue #14: object with multiline string + integer + boolean",
            'object({msg: string("line1\\nline2"), n: integer(7), ok: true})',
            "---\n  msg: |-\n    line1\n    line2\n  n: 7\n  ok: true",
        )
    )

    tests.append(
        _expect_match(
            "array_of_integers",
            "YAML array of integers uses list style",
            "array(1, 2, 3)",
            r"^---\n  - 1\n  - 2\n  - 3",
        )
    )

    tests.append(
        _expect_match(
            "array_with_multiline_string",
            "YAML array item with multiline string uses block scalar",
            'array(string("x\\ny"))',
            r"\|-",
        )
    )

    # --- conf input: -t yaml ---------------------------------------------

    with tempfile.TemporaryDirectory(prefix="afw_yaml_conf_") as td:
        conf_path = os.path.join(td, "afw.conf")
        # conf must be a list of conf objects; empty list is valid
        with open(conf_path, "w", encoding="utf-8") as fd:
            fd.write("# yaml conf (issue #14 content type input path)\n[]\n")
        code, out, err = _run_afw(
            [
                "-e",
                "afw_yaml",
                "-t",
                "yaml",
                "-f",
                conf_path,
                "-s",
                "expression",
                "-x",
                "integer(3)",
            ]
        )
        body = out[:-1] if out.endswith("\n") else out
        tests.append(
            _case(
                "conf_type_yaml_empty_list",
                "content type input: -t yaml parses YAML conf list",
                code == 0 and body == "3",
                detail="exit=%s body=%r stderr=%r" % (code, body, err[-200:]),
            )
        )

        conf2 = os.path.join(td, "app.yaml")
        with open(conf2, "w", encoding="utf-8") as fd:
            fd.write("- type: application\n")
        code, out, err = _run_afw(
            [
                "-e",
                "afw_yaml",
                "-t",
                "yaml",
                "-f",
                conf2,
                "-a",
                "yaml",
                "-s",
                "expression",
                "-x",
                'string("ok\\npath")',
            ]
        )
        body = out[:-1] if out.endswith("\n") else out
        tests.append(
            _case(
                "conf_type_yaml_with_allow_yaml",
                "combine -t yaml conf load with -a yaml block-scalar output",
                code == 0 and body == "---|-\n  ok\n  path",
                detail="exit=%s body=%r" % (code, body),
            )
        )

    # extension load should be visible when using -e
    code, body, err = _yaml_expr("1")
    tests.append(
        _case(
            "extension_loads",
            "afw_yaml extension loads when -e afw_yaml is given",
            code == 0 and "Extension 'afw_yaml' loaded." in err,
            detail="exit=%s err has load note=%s"
            % (code, "Extension 'afw_yaml' loaded." in err),
        )
    )

    return {"description": description, "tests": tests}
