#!/bin/sh
# Sample RSS of one hard-loop workload (no gdb).
# Usage: sample-rss.sh [workload] [duration_s]
#        sample-rss.sh integer_assign 20
set -e
here=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
leaf=$(dirname "$here")
name=${1:-integer_assign}
duration=${2:-15}
exec python3 "$leaf/_rss.py" "$name" --duration "$duration" --interval 5 --warmup 2
