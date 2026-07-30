#!/usr/bin/env python3
"""
Issue #15 external checks for conf path templates:

- Conf with environment:: path templates starts and scripts run when the env root
  is set correctly.
- open_file fails when the env root points at a missing host directory, and
  the error path shows the expanded absolute location (template worked).
"""

import os
import shutil
import subprocess
import tempfile
import textwrap


def _package_root():
    d = os.path.dirname(os.path.abspath(__file__))
    while d != "/" and not os.path.isfile(os.path.join(d, "afw-package.json")):
        d = os.path.dirname(d)
    return d


def _write(path, content):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w", encoding="utf-8") as f:
        f.write(content)


def _run_afw(work, conf_name, script_body, env):
    script_path = os.path.join(work, "t.as")
    with open(script_path, "w", encoding="utf-8") as f:
        f.write(script_body)
    conf_path = os.path.join(work, conf_name)
    r = subprocess.run(
        ["afw", "--syntax", "script", "--conf", conf_path, script_path],
        capture_output=True,
        text=True,
        timeout=60,
        cwd=work,
        env=env,
    )
    return r


def run():
    description = "Issue #15 conf path templates (external env / failure cases)"
    tests = []
    work = tempfile.mkdtemp(prefix="afw_issue15_")
    host_root = os.path.join(work, "host")
    files_dir = os.path.join(host_root, "files")
    os.makedirs(files_dir)
    _write(os.path.join(files_dir, "marker.txt"), "marker-ok\n")

    conf_ok = textwrap.dedent(
        """\
        [
            {
                type: "application",
                applicationId: "issue15-ext",
                rootFilePaths: {
                    "data": "${environment::AFW_ISSUE15_EXT_ROOT}/files"
                }
            }
        ]
        """
    )
    conf_path = os.path.join(work, "ok.conf")
    with open(conf_path, "w", encoding="utf-8") as f:
        f.write(conf_ok)

    # --- success with correct env root ---
    env = os.environ.copy()
    env["AFW_ISSUE15_EXT_ROOT"] = host_root
    script = textwrap.dedent(
        """\
        const sn = open_file("m", "data/marker.txt", "r");
        const s = read(sn, 100);
        close(sn);
        if (s !== "marker-ok\\n" && s !== "marker-ok") {
            print("FAIL content=", s);
            return 1;
        }
        print("PASS");
        return 0;
        """
    )
    try:
        r = _run_afw(work, "ok.conf", script, env)
        out = (r.stdout or "") + (r.stderr or "")
        ok = r.returncode == 0 and "PASS" in out
        tests.append(
            {
                "test": "template-rootFilePaths-success",
                "description": "environment:: rootFilePaths opens host file",
                "passed": ok,
                "skip": False,
                "error": None if ok else (
                    f"rc={r.returncode} out={out[:800]}"
                ),
            }
        )
    except Exception as e:
        tests.append(
            {
                "test": "template-rootFilePaths-success",
                "description": "environment:: rootFilePaths opens host file",
                "passed": False,
                "skip": False,
                "error": str(e),
            }
        )

    # --- missing host directory: template still expands; open_file fails ---
    # (apr filepath TRUENAME may not require the dir at conf create)
    env_bad = os.environ.copy()
    missing_root = os.path.join(work, "does-not-exist")
    env_bad["AFW_ISSUE15_EXT_ROOT"] = missing_root
    # Avoid embedding the success marker in the script body: error
    # backtraces reprint source and would false-trigger a "not in out" check.
    script_open = textwrap.dedent(
        """\
        const sn = open_file("m", "data/marker.txt", "r");
        print("opened-ok");
        return 0;
        """
    )
    try:
        r = _run_afw(work, "ok.conf", script_open, env_bad)
        out = (r.stdout or "") + (r.stderr or "")
        # Template expanded to absolute missing path; open must fail.
        expanded_seen = missing_root in out
        open_failed = (
            "failed to open" in out or "No such file" in out
        )
        # "opened-ok" may appear in source reprint; require open failure instead.
        ok = expanded_seen and open_failed
        tests.append(
            {
                "test": "template-rootFilePaths-missing-dir-open-fails",
                "description": (
                    "missing host root: template expands; open_file fails"
                ),
                "passed": ok,
                "skip": False,
                "error": None if ok else (
                    f"rc={r.returncode} out={out[:1000]}"
                ),
            }
        )
    except Exception as e:
        tests.append(
            {
                "test": "template-rootFilePaths-missing-dir-open-fails",
                "description": (
                    "missing host root: template expands; open_file fails"
                ),
                "passed": False,
                "skip": False,
                "error": str(e),
            }
        )

    # --- file adapter root template ---
    objects_dir = os.path.join(host_root, "objects", "T")
    os.makedirs(objects_dir)
    _write(
        os.path.join(objects_dir, "a.json"),
        '{\n  "id": "a"\n}\n',
    )
    conf_file = textwrap.dedent(
        """\
        [
            {
                type: "application",
                applicationId: "issue15-file"
            },
            {
                type: "adapter",
                adapterType: "file",
                adapterId: "file",
                root: "${environment::AFW_ISSUE15_EXT_ROOT}/objects/",
                contentType: "json",
                filenameSuffix: ".json"
            }
        ]
        """
    )
    with open(os.path.join(work, "file.conf"), "w", encoding="utf-8") as f:
        f.write(conf_file)
    env_file = os.environ.copy()
    env_file["AFW_ISSUE15_EXT_ROOT"] = host_root
    script_file = textwrap.dedent(
        """\
        const o = get_object("file", "T", "a");
        if (o === null || o === undefined || o.id !== "a") {
            print("FAIL");
            return 1;
        }
        print("PASS");
        return 0;
        """
    )
    try:
        r = _run_afw(work, "file.conf", script_file, env_file)
        out = (r.stdout or "") + (r.stderr or "")
        ok = r.returncode == 0 and "PASS" in out
        tests.append(
            {
                "test": "template-file-adapter-root-success",
                "description": "environment:: file adapter root get_object",
                "passed": ok,
                "skip": False,
                "error": None if ok else (
                    f"rc={r.returncode} out={out[:800]}"
                ),
            }
        )
    except Exception as e:
        tests.append(
            {
                "test": "template-file-adapter-root-success",
                "description": "environment:: file adapter root get_object",
                "passed": False,
                "skip": False,
                "error": str(e),
            }
        )

    try:
        shutil.rmtree(work, ignore_errors=True)
    except Exception:
        pass

    return {"description": description, "tests": tests}
