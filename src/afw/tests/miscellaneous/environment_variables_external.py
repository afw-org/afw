#!/usr/bin/env python3
"""
Tests for process environment boundary: valid UTF-8 → string, invalid → hexBinary,
non-UTF-8 names → _NONUTF8_ + hex. Uses a small C helper to setenv raw bytes then
exec afw (issue #71 related external-octets handling).
"""

import os
import subprocess
import tempfile
import textwrap


def _package_root():
    d = os.path.dirname(os.path.abspath(__file__))
    while d != "/" and not os.path.isfile(os.path.join(d, "afw-package.json")):
        d = os.path.dirname(d)
    return d


def _compile_helper(path):
    src = textwrap.dedent(
        r"""
        #include <stdio.h>
        #include <stdlib.h>
        #include <unistd.h>
        #include <string.h>
        int main(int argc, char **argv) {
            /* Invalid UTF-8 value */
            char bad_val[] = {'h','e','l','l','o',(char)0xff,(char)0xfe,0};
            setenv("AFW_PYTEST_BAD_VAL", bad_val, 1);
            /* Valid UTF-8 non-ASCII value (café) */
            char good_val[] = {'c','a','f',(char)0xc3,(char)0xa9,0};
            setenv("AFW_PYTEST_GOOD_VAL", good_val, 1);
            /* Invalid UTF-8 name */
            char bad_name[] = {'B','A','D',(char)0xff,'N',0};
            setenv(bad_name, "from_bad_name", 1);
            if (argc < 2) return 2;
            execvp(argv[1], argv + 1);
            perror("execvp");
            return 1;
        }
        """
    )
    c_path = path + ".c"
    with open(c_path, "w", encoding="utf-8") as f:
        f.write(src)
    subprocess.check_call(["cc", "-O0", "-o", path, c_path])


def _run_script(helper, script_body):
    with tempfile.NamedTemporaryFile("w", suffix=".as", delete=False) as tf:
        tf.write(script_body)
        script_path = tf.name
    try:
        r = subprocess.run(
            [helper, "afw", "-s", "script", script_path],
            capture_output=True,
            text=True,
            timeout=60,
        )
        return r
    finally:
        os.unlink(script_path)


def run():
    description = (
        "Process env external octets: string | hexBinary and _NONUTF8_ names"
    )
    tests = []
    work = tempfile.mkdtemp(prefix="afw_env_ext_")
    helper = os.path.join(work, "setenv_exec")

    try:
        _compile_helper(helper)
    except Exception as e:
        return {
            "description": description,
            "tests": [
                {
                    "test": "compile_helper",
                    "description": "Compile setenv helper",
                    "passed": False,
                    "skip": False,
                    "error": str(e),
                }
            ],
        }

    cases = [
        (
            "retrieve_one_current",
            "retrieve_objects returns one current with bad env present",
            textwrap.dedent(
                """\
                const objs = retrieve_objects("afw", "_AdaptiveEnvironmentVariables_");
                if (length(objs) !== 1) {
                    print("FAIL count=", length(objs));
                    return 1;
                }
                print("PASS");
                return 0;
                """
            ),
            "PASS",
        ),
        (
            "bad_value_hexBinary",
            "Invalid UTF-8 env value is hexBinary",
            textwrap.dedent(
                """\
                const e = get_object("afw", "_AdaptiveEnvironmentVariables_", "current");
                const v = e.AFW_PYTEST_BAD_VAL;
                if (meta(v).dataType !== "hexBinary") {
                    print("FAIL type=", meta(v).dataType);
                    return 1;
                }
                print("PASS");
                return 0;
                """
            ),
            "PASS",
        ),
        (
            "good_value_string",
            "Valid UTF-8 env value is string café",
            textwrap.dedent(
                """\
                const e = get_object("afw", "_AdaptiveEnvironmentVariables_", "current");
                const v = e.AFW_PYTEST_GOOD_VAL;
                if (meta(v).dataType !== "string") {
                    print("FAIL type=", meta(v).dataType);
                    return 1;
                }
                if (v !== "café") {
                    print("FAIL value=", v);
                    return 1;
                }
                print("PASS");
                return 0;
                """
            ),
            "PASS",
        ),
        (
            "bad_name_nonutf8_prefix",
            "Invalid UTF-8 env name exposed as _NONUTF8_ + hex",
            textwrap.dedent(
                """\
                const e = get_object("afw", "_AdaptiveEnvironmentVariables_", "current");
                /* Force full environ load (lazy get only does getenv by UTF-8 name). */
                string(e);
                /* BAD\\xffN → uppercase hex 424144FF4E */
                const k = "_NONUTF8_424144FF4E";
                const v = e[k];
                if (v === null || v === undefined) {
                    print("FAIL missing key");
                    return 1;
                }
                if (v !== "from_bad_name") {
                    print("FAIL value=", v);
                    return 1;
                }
                print("PASS");
                return 0;
                """
            ),
            "PASS",
        ),
    ]

    for name, desc, body, expect_token in cases:
        r = _run_script(helper, body)
        out = (r.stdout or "") + (r.stderr or "")
        passed = r.returncode == 0 and expect_token in out
        tests.append(
            {
                "test": name,
                "description": desc,
                "passed": passed,
                "skip": False,
                "stdout": r.stdout,
                "stderr": r.stderr,
                "returncode": r.returncode,
            }
        )

    return {"description": description, "tests": tests}
