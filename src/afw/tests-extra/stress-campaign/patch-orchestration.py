#!/usr/bin/env python3
"""Patch orchestration.yaml for stress duration and load knobs."""

import argparse
import re
import sys


def patch_file(path, args):
    lines = open(path).read().splitlines(True)
    out = []
    in_afwfcgi = False
    in_firehose = False
    firehose_depth = 0
    saw_duration = False
    saw_timeout = False
    saw_max_requests = False

    for line in lines:
        if re.match(r"^afwfcgi:\s*$", line):
            in_afwfcgi = True
            out.append(line)
            continue
        if in_afwfcgi:
            if line.startswith("  ") and not line.startswith("    "):
                in_afwfcgi = False
            elif args.server_threads and re.match(r"^\s+threads:\s*", line):
                line = "  threads: %s\n" % args.server_threads
            out.append(line)
            continue

        if re.match(r"^\s+- firehose:\s*$", line):
            in_firehose = True
            firehose_depth = len(line) - len(line.lstrip())
            out.append(line)
            continue

        if in_firehose:
            indent = len(line) - len(line.lstrip())
            if line.strip() and indent <= firehose_depth and not line.lstrip().startswith("-"):
                in_firehose = False
            elif args.drop_max_requests and re.match(r"^\s+maxRequests:\s*", line):
                saw_max_requests = True
                continue
            elif in_firehose:
                if args.duration is not None and (not saw_duration) and re.match(
                    r"^\s+duration_s:\s*", line
                ):
                    line = re.sub(
                        r"duration_s:\s*.*", "duration_s: %d" % args.duration, line
                    )
                    saw_duration = True
                elif args.concurrency and re.match(r"^\s+concurrency:\s*", line):
                    line = re.sub(
                        r"concurrency:\s*.*",
                        "concurrency: %s" % args.concurrency,
                        line.rstrip("\n"),
                    ) + "\n"
                elif args.client_processes and re.match(
                    r"^\s+clientProcesses:\s*", line
                ):
                    line = re.sub(
                        r"clientProcesses:\s*.*",
                        "clientProcesses: %s" % args.client_processes,
                        line.rstrip("\n"),
                    ) + "\n"

        if args.duration is not None and (not saw_timeout) and line.startswith(
            "timeout_s:"
        ):
            line = "timeout_s: %d\n" % (args.duration + 150)
            saw_timeout = True

        out.append(line)

    open(path, "w").write("".join(out))
    if args.duration is not None and not saw_timeout:
        sys.exit("did not find timeout_s in " + path)


def main():
    p = argparse.ArgumentParser()
    p.add_argument("path")
    p.add_argument("--duration", type=int, default=None)
    p.add_argument("--server-threads", default=None)
    p.add_argument("--concurrency", default=None)
    p.add_argument("--client-processes", default=None)
    p.add_argument("--drop-max-requests", action="store_true")
    ns = p.parse_args()
    patch_file(ns.path, ns)


if __name__ == "__main__":
    main()
