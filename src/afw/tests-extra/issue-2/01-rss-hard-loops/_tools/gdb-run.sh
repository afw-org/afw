#!/bin/sh
# Start `afw -s script <workload>` under gdb with issue-2 helpers.
# Usage: gdb-run.sh [workload]
#        gdb-run.sh integer_assign
set -e
here=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
leaf=$(dirname "$here")
gdbdir="$leaf/_gdb"
root=$leaf
while [ "$root" != / ] && [ ! -f "$root/afw-package.json" ]; do
    root=$(dirname "$root")
done
name=${1:-integer_assign}
if [ -n "${1-}" ]; then
    shift
fi
script="$leaf/_workloads/${name}.as"
if [ ! -f "$script" ]; then
    echo "unknown workload: $name" >&2
    echo "have:" >&2
    ls "$leaf/_workloads" >&2
    exit 1
fi
export AFW_ISSUE2_GDB_DIR="$gdbdir"
echo "gdb --args afw -s script $script" >&2
echo "inside gdb: run    then Ctrl-C    then afw-help / afw-rss / afw-heap / afw-bt" >&2
exec gdb -q \
    -d "$root/src/afw" \
    -d "$root/src/afw/pool" \
    -d "$root/src/afw/value" \
    -d "$root/src/afw/xctx" \
    -d "$root/src/afw/function" \
    -d "$root/src/afw/object" \
    -x "$gdbdir/afw.gdb" \
    --args afw -s script "$script"
