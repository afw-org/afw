#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Summarize #242 `>debug pool|eval|lock` lines. Do not dump the raw log.

    python3 _trace.py /tmp/pool.log
    python3 _trace.py /tmp/pool.log --top 20

Pool line (minimal):
  >debug pool release 3 bytes 100 env 200 rss 5000 KB refs 1 parent 0 (afw_pool.c:251)

Eval / lock:
  >debug eval push_value (afw_value_call.c:181)
  >debug lock obtain environment (afw_flag.c:200)
"""

from __future__ import print_function

import argparse
import collections
import re
import sys


POOL_RE = re.compile(
    r"^>debug pool (?P<op>.*) (?P<pool>-?\d+) bytes (?P<bytes>\d+) "
    r"env (?P<env>\d+) rss (?P<rss>\d+) KB refs (?P<refs>-?\d+) "
    r"parent (?P<parent>-?\d+) \((?P<src>[^)]+)\)\s*$"
)
KIND_RE = re.compile(
    r"^>debug (?P<kind>eval|lock) (?P<op>.*) \((?P<src>[^)]+)\)\s*$"
)


def summarize(lines, top=20):
    counts = collections.Counter()
    last_by_pool = {}
    env_last = None
    rss_last = None
    n_pool = n_eval = n_lock = n_other = 0
    first_pool = last_pool = None

    for raw in lines:
        line = raw.rstrip("\n")
        m = POOL_RE.match(line)
        if m:
            n_pool += 1
            op = m.group("op").strip()
            src = m.group("src")
            pool = int(m.group("pool"))
            rec = {
                "op": op,
                "bytes": int(m.group("bytes")),
                "env": int(m.group("env")),
                "rss": int(m.group("rss")),
                "refs": int(m.group("refs")),
                "parent": int(m.group("parent")),
                "src": src,
            }
            counts[("pool", op.split()[0], src)] += 1
            last_by_pool[pool] = rec
            if first_pool is None:
                first_pool = rec
            last_pool = rec
            env_last = rec["env"]
            rss_last = rec["rss"]
            continue
        m = KIND_RE.match(line)
        if m:
            kind = m.group("kind")
            if kind == "eval":
                n_eval += 1
            else:
                n_lock += 1
            counts[(kind, m.group("op").strip().split()[0], m.group("src"))] += 1
            continue
        if line.startswith(">debug "):
            n_other += 1

    return {
        "n_pool": n_pool,
        "n_eval": n_eval,
        "n_lock": n_lock,
        "n_other": n_other,
        "counts": counts,
        "last_by_pool": last_by_pool,
        "first_pool": first_pool,
        "last_pool": last_pool,
        "env_last": env_last,
        "rss_last": rss_last,
        "top": top,
    }


def format_summary(s):
    lines = []
    lines.append(
        "lines: pool=%s eval=%s lock=%s other_debug=%s"
        % (s["n_pool"], s["n_eval"], s["n_lock"], s["n_other"])
    )
    if s["first_pool"] and s["last_pool"]:
        f, last = s["first_pool"], s["last_pool"]
        lines.append(
            "env bytes: first=%s last=%s  delta=%s"
            % (f["env"], last["env"], last["env"] - f["env"])
        )
        lines.append(
            "rss KB (current): first=%s last=%s  delta=%s"
            % (f["rss"], last["rss"], last["rss"] - f["rss"])
        )
    lines.append("counts by kind/op/file:line (top %s):" % s["top"])
    for (kind, op, src), n in s["counts"].most_common(s["top"]):
        lines.append("  %6d  %s %s  (%s)" % (n, kind, op, src))
    pools = s["last_by_pool"]
    if pools:
        lines.append("last bytes/env/rss per pool (%s pools):" % len(pools))
        # Show the pools with largest last bytes, plus a few.
        ranked = sorted(
            pools.items(), key=lambda kv: kv[1]["bytes"], reverse=True)
        show = ranked[:12]
        for pool, rec in show:
            lines.append(
                "  pool %s  parent=%s  bytes=%s  env=%s  rss=%s KB  refs=%s  last=%s (%s)"
                % (pool, rec["parent"], rec["bytes"], rec["env"], rec["rss"],
                   rec["refs"], rec["op"], rec["src"])
            )
        extra = len(ranked) - len(show)
        if extra > 0:
            lines.append("  ... +%s more pools" % extra)
    return "\n".join(lines)


def main(argv):
    p = argparse.ArgumentParser(
        description="Summarize #242 >debug lines (do not paste the raw log)")
    p.add_argument("path", help="stderr capture, or - for stdin")
    p.add_argument("--top", type=int, default=20)
    args = p.parse_args(argv)
    if args.path == "-":
        lines = sys.stdin
        s = summarize(lines, top=args.top)
    else:
        with open(args.path, "r", encoding="utf-8", errors="replace") as fd:
            s = summarize(fd, top=args.top)
    print(format_summary(s))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
