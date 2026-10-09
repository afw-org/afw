#!/usr/bin/env python3
"""
Writes from inside an LMDB retrieve callback (#508).

The scan runs in an LMDB read transaction. A write from its callback
begins the session's write transaction next to it on the same thread
(the write takes no reader slot), and the scan keeps reading its own
snapshot. Each case runs in a new afw process, after data written by a
separate process, so the process has no write transaction of its own when
the scan starts (in one process, the first write's transaction lasts
until the process ends, and later scans run inside it). A third process
checks what was kept.
"""

import os
import subprocess

from _afwdev.test import context as test_context


SEED = """
    index_create("lmdb", "k", undefined, ["WsIdx"], undefined, undefined, false, false);
    for (let i = 0; i < 3; i = i + 1) {
        const id = string(i);
        add_object("lmdb", "WsAdd", { id: "a" + id }, "a" + id);
        add_object("lmdb", "WsMod", { id: "m" + id, n: i }, "m" + id);
        add_object("lmdb", "WsSame", { id: "s" + id }, "s" + id);
        add_object("lmdb", "WsDel", { id: "d" + id }, "d" + id);
        add_object("lmdb", "WsIdx", { id: "i" + id, k: "v" }, "i" + id);
    }
    return 0;
"""

# Each scan returns the ids its callback saw, after asserting how many.
CASES = [
    (
        "write_other_type_during_scan",
        "A callback adds an object of another type for each object scanned; all are kept",
        """
        let seen = "";
        retrieve_objects_to_callback(function (o, u) {
            if (o === undefined) return true;
            seen = seen + o.id + " ";
            add_object("lmdb", "WsAudit", { of: o.id });
            return false;
        }, undefined, "lmdb", "WsAdd");
        return seen;
        """,
        3,
        """return length(retrieve_objects("lmdb", "WsAudit"));""",
        "3",
    ),
    (
        "modify_scanned_object_during_scan",
        "A callback modifies the object it was given; the changes are kept",
        """
        let seen = "";
        retrieve_objects_to_callback(function (o, u) {
            if (o === undefined) return true;
            seen = seen + o.id + " ";
            modify_object("lmdb", "WsMod", o.id, [["set_property", "n", 100]]);
            return false;
        }, undefined, "lmdb", "WsMod");
        return seen;
        """,
        3,
        """return length(retrieve_objects("lmdb", "WsMod",
            { filter: { op: "eq", property: "n", value: 100 } }));""",
        "3",
    ),
    (
        "add_scanned_type_during_scan",
        "A callback adds objects of the type being scanned; the scan does not see them, and they are kept",
        """
        let seen = "";
        retrieve_objects_to_callback(function (o, u) {
            if (o === undefined) return true;
            seen = seen + o.id + " ";
            add_object("lmdb", "WsSame", { id: "new" });
            return false;
        }, undefined, "lmdb", "WsSame");
        return seen;
        """,
        3,
        """return length(retrieve_objects("lmdb", "WsSame"));""",
        "6",
    ),
    (
        "delete_later_objects_during_scan",
        "A callback deletes the objects not scanned yet; the scan still delivers them from its snapshot, and the deletes are kept",
        """
        let seen = "";
        retrieve_objects_to_callback(function (o, u) {
            if (o === undefined) return true;
            if (seen === "") {
                for (let i = 0; i < 3; i = i + 1) {
                    if ("d" + string(i) !== o.id) {
                        delete_object("lmdb", "WsDel", "d" + string(i));
                    }
                }
            }
            seen = seen + o.id + " ";
            return false;
        }, undefined, "lmdb", "WsDel");
        return seen;
        """,
        3,
        """return length(retrieve_objects("lmdb", "WsDel"));""",
        "1",
    ),
    (
        "change_indexed_value_during_index_query",
        "During an index query, a callback changes the indexed value; the index is right afterwards",
        """
        let seen = "";
        retrieve_objects_to_callback(function (o, u) {
            if (o === undefined) return true;
            seen = seen + o.id + " ";
            modify_object("lmdb", "WsIdx", o.id, [["set_property", "k", "w"]]);
            return false;
        }, undefined, "lmdb", "WsIdx",
            { filter: { op: "eq", property: "k", value: "v" } });
        return seen;
        """,
        3,
        """return string(length(retrieve_objects("lmdb", "WsIdx",
                { filter: { op: "eq", property: "k", value: "w" } })))
            + "/" + string(length(retrieve_objects("lmdb", "WsIdx",
                { filter: { op: "eq", property: "k", value: "v" } })));""",
        "3/0",
    ),
]


def _run_script(work_dir, afw_conf, script_body):
    # --expression prints the script's result; a script file's result is
    # its exit status.
    cmd = ["afw"]
    if afw_conf:
        cmd += ["--conf", "afw.conf"]
    cmd += ["-s", "script", "-x", script_body]
    return subprocess.run(
        cmd, cwd=work_dir, capture_output=True, text=True, timeout=60
    )


def _remove_data(work_dir):
    for name in ("data.mdb", "lock.mdb"):
        path = os.path.join(work_dir, name)
        if os.path.exists(path):
            os.remove(path)


def _last_line(r):
    lines = [l for l in (r.stdout or "").splitlines() if l.strip()]
    return lines[-1].strip().strip('"') if lines else ""


def run():
    description = "Writes from inside an LMDB retrieve callback are kept, and the scan reads its own snapshot (#508)"

    ctx = test_context.current()
    testEnvironment = ctx.get("testEnvironment") or {}
    work_dir = testEnvironment.get("work_dir")
    afw_conf = testEnvironment.get("afw_conf")

    if not work_dir:
        return {
            "description": description,
            "tests": [
                {
                    "test": "environment_available",
                    "description": "lmdb-adapter test environment is available",
                    "passed": False,
                    "skip": False,
                    "error": "No testEnvironment/work_dir in test context",
                }
            ],
        }

    tests = []

    for name, desc, scan, scan_count, check, expect in CASES:
        _remove_data(work_dir)
        seed = _run_script(work_dir, afw_conf, SEED)

        scan_r = _run_script(work_dir, afw_conf,
            'flag_set(["trace:adapterId:lmdb"], true);\n' + scan)
        out = (scan_r.stdout or "") + (scan_r.stderr or "")
        seen = _last_line(scan_r).split()

        # The case only means something if the write began while the
        # scan's read transaction was open.
        read_at = out.find("LMDB Begin read transaction")
        write_at = out.find("LMDB Begin write transaction")
        nested = 0 <= read_at < write_at

        check_r = _run_script(work_dir, afw_conf, check)
        kept = _last_line(check_r)

        passed = (
            seed.returncode == 0
            and scan_r.returncode == 0
            and len(seen) == scan_count
            and nested
            and check_r.returncode == 0
            and kept == expect
        )
        tests.append({
            "test": name,
            "description": desc,
            "passed": bool(passed),
            "skip": False,
            "error": None if passed else (
                "seen=%r (want %d) write_inside_read=%s kept=%r (want %r)"
                % (seen, scan_count, nested, kept, expect)),
            "stdout": scan_r.stdout + "\n--- check ---\n" + check_r.stdout,
            "stderr": scan_r.stderr + "\n--- check ---\n" + check_r.stderr,
            "returncode": scan_r.returncode or check_r.returncode,
        })

    _remove_data(work_dir)
    return {"description": description, "tests": tests}
