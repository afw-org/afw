#!/bin/sh
# Attach gdb to a running afw (start the workload in another terminal first).
# Usage: gdb-attach.sh [pid]
#        gdb-attach.sh            # newest afw process
set -e
here=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
leaf=$(dirname "$here")
gdbdir="$leaf/_gdb"
root=$leaf
while [ "$root" != / ] && [ ! -f "$root/afw-package.json" ]; do
    root=$(dirname "$root")
done
if [ -n "${1-}" ]; then
    pid=$1
else
    pid=$(pgrep -n -x afw || true)
    if [ -z "$pid" ]; then
        echo "no afw process; start one with:" >&2
        echo "  afw -s script $leaf/_workloads/integer_assign.as" >&2
        exit 1
    fi
fi
export AFW_ISSUE2_GDB_DIR="$gdbdir"
echo "attaching to pid $pid" >&2
echo "inside gdb: afw-help / afw-rss / afw-heap / afw-bt / afw-breaks" >&2
exec gdb -q \
    -d "$root/src/afw" \
    -d "$root/src/afw/pool" \
    -d "$root/src/afw/value" \
    -d "$root/src/afw/xctx" \
    -d "$root/src/afw/function" \
    -d "$root/src/afw/object" \
    -x "$gdbdir/afw.gdb" \
    -p "$pid"
