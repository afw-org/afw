#!/usr/bin/env python3
"""
Index definitions come from each transaction's snapshot, so a process
sees another process's index_create / index_remove from its next
transaction.

Process B is one long-lived `afw -s expression` (a line is evaluated as it
is entered); process A runs index_create / index_remove in between, as
separate `afw` runs, on the same LMDB environment. Before, each process
kept the definitions it loaded at start (and those it changed itself) in
memory: B's writes did not maintain an index A created, so a query that
used it missed them; B kept planning with an index A removed, and its
cleared database gave nothing.
"""

import os
import subprocess
import tempfile

from _afwdev.test import context as test_context


MARK = "@@END@@"


def _run_script(work_dir, afw_conf, script_body):
    with tempfile.NamedTemporaryFile(
        "w", suffix=".as", dir=work_dir, delete=False
    ) as tf:
        tf.write(script_body)
        script_path = tf.name
    try:
        cmd = ["afw"]
        if afw_conf:
            cmd += ["--conf", "afw.conf"]
        cmd += ["-s", "script", script_path]
        return subprocess.run(
            cmd, cwd=work_dir, capture_output=True, text=True, timeout=60
        )
    finally:
        os.unlink(script_path)


class _LongLived:
    """One afw process that evaluates an expression per line."""

    def __init__(self, work_dir, afw_conf):
        cmd = ["afw"]
        if afw_conf:
            cmd += ["--conf", "afw.conf"]
        cmd += ["-s", "expression"]
        self.proc = subprocess.Popen(
            cmd, cwd=work_dir, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
            stderr=subprocess.PIPE, text=True, bufsize=1,
        )
        self.ask("1")  # past the banner

    def ask(self, expression):
        """Evaluate expression; its output lines (empty on an error)."""
        self.proc.stdin.write(expression + "\n" + '"' + MARK + '"\n')
        self.proc.stdin.flush()
        lines = []
        while True:
            line = self.proc.stdout.readline()
            if not line or MARK in line:
                break
            lines.append(line)
        return "".join(lines).strip()

    def close(self):
        try:
            self.proc.stdin.close()
            self.proc.wait(timeout=30)
        except Exception:
            self.proc.kill()
        return self.proc.stderr.read()


def _result(name, desc, passed, detail):
    return {
        "test": name,
        "description": desc,
        "passed": bool(passed),
        "skip": False,
        "stdout": detail,
        "stderr": "",
        "returncode": 0 if passed else 1,
    }


def run():
    description = "Index changes in another process are seen from the next transaction"

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

    # 1. A creates an index while B runs; B's later writes maintain it.
    b = _LongLived(work_dir, afw_conf)
    log = []
    try:
        log.append(b.ask('add_object("lmdb", "TwoProcCreate", {s_tpc: "b0"}, "b0")'))
        a = _run_script(work_dir, afw_conf, """
            index_create("lmdb", "s_tpc", undefined, ["TwoProcCreate"],
                undefined, undefined, false);
            return 0;
            """)
        log.append("A: " + str(a.returncode) + " " + a.stderr[-300:])
        log.append(b.ask('add_object("lmdb", "TwoProcCreate", {s_tpc: "b1"}, "b1")'))
        in_b = b.ask('length(retrieve_objects("lmdb", "TwoProcCreate", '
            '{filter: {op: "eq", property: "s_tpc", value: "b1"}}))')
        log.append("B finds b1: " + in_b)
    finally:
        log.append(b.close()[-300:])
    c = _run_script(work_dir, afw_conf, """
        const n = length(retrieve_objects("lmdb", "TwoProcCreate",
            { filter: { op: "eq", property: "s_tpc", value: "b1" } }));
        assert(n === 1, "a new process finds b1 through the index: " + string(n));
        return 0;
        """)
    log.append("C: " + str(c.returncode) + " " + c.stderr[-300:])
    tests.append(_result(
        "another_process_writes_maintain_new_index",
        "A process's writes after another process created an index are in that index",
        a.returncode == 0 and in_b == "1" and c.returncode == 0,
        "\n".join(log),
    ))

    # 2. A removes an index B has used; B stops using it.
    log = []
    a = _run_script(work_dir, afw_conf, """
        add_object("lmdb", "TwoProcRemove", { s_tpr: "r0" }, "r0");
        index_create("lmdb", "s_tpr", undefined, ["TwoProcRemove"],
            undefined, undefined, false);
        return 0;
        """)
    log.append("A create: " + str(a.returncode) + " " + a.stderr[-300:])
    b = _LongLived(work_dir, afw_conf)
    try:
        before = b.ask('length(retrieve_objects("lmdb", "TwoProcRemove", '
            '{filter: {op: "eq", property: "s_tpr", value: "r0"}}))')
        log.append("B before: " + before)
        a2 = _run_script(work_dir, afw_conf, """
            index_remove("lmdb", "s_tpr");
            return 0;
            """)
        log.append("A remove: " + str(a2.returncode) + " " + a2.stderr[-300:])
        after = b.ask('length(retrieve_objects("lmdb", "TwoProcRemove", '
            '{filter: {op: "eq", property: "s_tpr", value: "r0"}}))')
        log.append("B after: " + after)
    finally:
        log.append(b.close()[-300:])
    tests.append(_result(
        "another_process_index_removed",
        "A process stops planning with an index another process removed (its cleared database gave nothing)",
        a.returncode == 0 and a2.returncode == 0 and before == "1" and after == "1",
        "\n".join(log),
    ))

    return {"description": description, "tests": tests}
