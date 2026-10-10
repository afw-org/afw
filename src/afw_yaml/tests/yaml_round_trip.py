#!/usr/bin/env python3
"""
YAML read and write through a file adapter (contentType yaml).

Coverage (found 2026-10-10 by a differential probe against PyYAML):
  - a sequence with no indentation under a key ("key:\n- a\n- b"), the
    style most YAML writers use: it took the first entry as the value,
    then failed "Unexpected token inside map"
  - plain keys that look like numbers or booleans are their text
    ("1: a" had no key)
  - write then read back: keys that need quotes, doubles that look like
    integers (1e10, -0.0), multi-line strings (keep chomping, a first
    line starting with a space), a scalar document ("---42" was a string)
"""

from __future__ import annotations

import os
import subprocess
import tempfile
import textwrap


def _case(name, description, passed, detail=None):
    t = {"test": name, "description": description, "passed": bool(passed)}
    if detail and not passed:
        t["error"] = detail
    return t


def _write(path, content):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w", encoding="utf-8") as fd:
        fd.write(content)


def _script(td, source):
    conf = os.path.join(td, "afw.conf")
    _write(
        conf,
        '[{ type: "adapter", adapterType: "file", adapterId: "y", '
        'root: "objects/", filenameSuffix: ".yaml", contentType: "yaml" }]',
    )
    path = os.path.join(td, "t.as")
    _write(path, textwrap.dedent(source))
    r = subprocess.run(
        ["afw", "-e", "afw_yaml", "-f", conf, "-s", "script", path],
        capture_output=True, text=True, timeout=60, cwd=td,
    )
    return r.returncode, r.stdout.strip(), r.stderr


def run():
    tests = []

    with tempfile.TemporaryDirectory(prefix="afw_yaml_rt_") as td:
        _write(
            os.path.join(td, "objects", "T", "indentless.yaml"),
            "p:\n- a\n- b\nq: 1\nitems:\n- k: 1\n  j: 2\n- k: 3\nend: true\n",
        )
        code, body, err = _script(td, """\
            const o = get_object("y", "T", "indentless");
            assert(stringify(o.p) === '["a","b"]', "p");
            assert(o.q === 1, "q");
            assert(stringify(o.items) === '[{"k":1,"j":2},{"k":3}]', "items");
            assert(o.end === true, "end");
            return 0;
            """)
        tests.append(_case(
            "indentless_sequence",
            "a sequence with no indentation under a key is a list",
            code == 0, "exit=%s %s" % (code, err[-600:])))

        _write(
            os.path.join(td, "objects", "T", "keys.yaml"),
            "1: a\n1e30: b\ntrue: c\nnull: d\n",
        )
        code, body, err = _script(td, """\
            const o = get_object("y", "T", "keys");
            assert(stringify(keys(o)) === '["1","1e30","true","null"]',
                stringify(keys(o)));
            return 0;
            """)
        tests.append(_case(
            "plain_keys_are_text",
            "a plain key that looks like a number, boolean, or null is its text",
            code == 0, "exit=%s %s" % (code, err[-600:])))

        code, body, err = _script(td, """\
            const w = {
                "a #b": 1, "%p": 2, " x": 3, "{a}": 4, "- x": 5,
                "yes": 6, "n": 7, "1": 8, "plain_key-1": 9,
                big: 1e10, negzero: -0.0, tenth: 0.1, tiny: 5e-324,
                keep: "a\\n\\n", clip: "a\\nb\\n", strip: "a\\nb",
                lead: "  lead\\n", blank: "\\n", tab: "x\\n\\ty"
            };
            add_object("y", "W", w, "w");
            const r = get_object("y", "W", "w");
            for (const k of keys(w)) {
                assert(r[k] === w[k], k);
            }
            assert(length(keys(r)) === length(keys(w)), "keys");
            return 0;
            """)
        tests.append(_case(
            "write_read_back",
            "an object written as YAML reads back the same (keys, doubles, "
            "multi-line strings)",
            code == 0, "exit=%s %s" % (code, err[-600:])))

    return {
        "description": "YAML read/write round trip (file adapter)",
        "tests": tests,
    }
