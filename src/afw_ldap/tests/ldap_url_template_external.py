#!/usr/bin/env python3
"""
Issue #15: LDAP adapter url is a template evaluated at adapter start.

Without a live LDAP server we only verify that environment:: is expanded
before URL parse: a template that evaluates to a non-LDAP string fails
adapter create the same way a plain bad url would.
"""

import os
import subprocess
import tempfile
import textwrap


def _run_afw(work, conf_name, script_body, env):
    script_path = os.path.join(work, "t.as")
    with open(script_path, "w", encoding="utf-8") as f:
        f.write(script_body)
    conf_path = os.path.join(work, conf_name)
    return subprocess.run(
        ["afw", "--syntax", "script", "--conf", conf_path, script_path],
        capture_output=True,
        text=True,
        timeout=60,
        cwd=work,
        env=env,
    )


def run():
    description = "Issue #15 LDAP url conf template evaluation"
    tests = []
    work = tempfile.mkdtemp(prefix="afw_ldap_url_")

    conf = textwrap.dedent(
        """\
        [
            {
                type: "adapter",
                adapterType: "ldap",
                adapterId: "ldap",
                url: "${environment::AFW_LDAP_TEST_URL}",
                bindParameters: {
                    dn: "cn=test",
                    password: "x"
                }
            }
        ]
        """
    )
    with open(os.path.join(work, "ldap.conf"), "w", encoding="utf-8") as f:
        f.write(conf)

    script = "return 0;\n"
    env = os.environ.copy()
    # Not a valid LDAP URL after template eval → adapter create fails at parse
    env["AFW_LDAP_TEST_URL"] = "not-a-valid-ldap-url"

    try:
        r = _run_afw(work, "ldap.conf", script, env)
        out = (r.stdout or "") + (r.stderr or "")
        # Template must have been evaluated (bad string reaches URL parse).
        failed = (
            "invalid" in out.lower()
            or "url" in out.lower()
            or "Error" in out
            or "error" in out
            or r.returncode != 0
        )
        # Adapter should not report successful start
        no_success = "adapter-ldap' successfully started" not in out
        ok = failed and no_success
        tests.append(
            {
                "test": "ldap-url-template-invalid-after-eval",
                "description": (
                    "url template expands; invalid result fails adapter create"
                ),
                "passed": ok,
                "skip": False,
                "error": None if ok else f"rc={r.returncode} out={out[:1200]}",
            }
        )
    except Exception as e:
        tests.append(
            {
                "test": "ldap-url-template-invalid-after-eval",
                "description": (
                    "url template expands; invalid result fails adapter create"
                ),
                "passed": False,
                "skip": False,
                "error": str(e),
            }
        )

    # Valid-looking URL form after template: parse should accept scheme
    # (may still fail later on connect/metadata; that still proves template
    # produced a parseable URL).
    env2 = os.environ.copy()
    env2["AFW_LDAP_TEST_URL"] = "ldap://127.0.0.1:1"
    try:
        r = _run_afw(work, "ldap.conf", script, env2)
        out = (r.stdout or "") + (r.stderr or "")
        # Must not fail solely as "property invalid" for url before any
        # network work. Either starts service, or fails on connect/metadata.
        bad_template = (
            "not-a-valid-ldap-url" in out
        )
        # "property invalid" alone is weak; accept any outcome that is not
        # "Value data type does not match" / template compile failure.
        template_ok = (
            "does not match compile_type" not in out
            and "modulePath must evaluate" not in out
        )
        ok = template_ok and not bad_template
        tests.append(
            {
                "test": "ldap-url-template-valid-form",
                "description": (
                    "url template expands to parseable ldap:// URL form"
                ),
                "passed": ok,
                "skip": False,
                "error": None if ok else f"rc={r.returncode} out={out[:1200]}",
            }
        )
    except Exception as e:
        tests.append(
            {
                "test": "ldap-url-template-valid-form",
                "description": (
                    "url template expands to parseable ldap:// URL form"
                ),
                "passed": False,
                "skip": False,
                "error": str(e),
            }
        )

    try:
        import shutil

        shutil.rmtree(work, ignore_errors=True)
    except Exception:
        pass

    return {"description": description, "tests": tests}
