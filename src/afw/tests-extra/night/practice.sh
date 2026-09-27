#!/bin/bash
# Two afwfcgi processes, two temp directories.
# Default durations are the check-in values in the leaves.
# A number argument replaces duration_s in a copy of each leaf
# and sets timeout_s to that many seconds plus 150.
#
#   src/afw/tests-extra/night/practice.sh
#   src/afw/tests-extra/night/practice.sh 600

set -euo pipefail

HERE=$(cd "$(dirname "$0")" && pwd)
ROOT=$(cd "$HERE/../../../.." && pwd)
DURATION="${1:-}"
STAGE=""

stage_leaf() {
    local name="$1"
    local src="$HERE/$name"
    local dst="$STAGE/$name"
    rm -rf "$dst"
    mkdir -p "$dst"
    cp -aL "$src"/. "$dst"/
    if [ -n "$DURATION" ]; then
        python3 - "$dst/orchestration.yaml" "$DURATION" <<'PY'
import sys
path, duration = sys.argv[1], int(sys.argv[2])
timeout = duration + 150
text = open(path).read()
text2 = []
seen_duration = False
seen_timeout = False
for line in text.splitlines(True):
    if (not seen_duration) and line.startswith("      duration_s:"):
        line = "      duration_s: %d\n" % duration
        seen_duration = True
    elif (not seen_timeout) and line.startswith("timeout_s:"):
        line = "timeout_s: %d\n" % timeout
        seen_timeout = True
    text2.append(line)
open(path, "w").write("".join(text2))
if not seen_duration or not seen_timeout:
    sys.exit("did not find duration_s and timeout_s in " + path)
PY
    fi
}

if [ -n "$DURATION" ]; then
    STAGE=$(mktemp -d /tmp/afw-night-stage-XXXX)
    stage_leaf slope
    stage_leaf restart
    SLOPE="$STAGE/slope"
    RESTART="$STAGE/restart"
else
    SLOPE="$HERE/slope"
    RESTART="$HERE/restart"
fi

mkdir -p /tmp/afw-night-slope /tmp/afw-night-restart
rm -f /tmp/afw-night-slope/practice.log /tmp/afw-night-restart/practice.log

echo "slope   $SLOPE  tmpdir /tmp/afw-night-slope"
echo "restart $RESTART  tmpdir /tmp/afw-night-restart"

set +e
afwdev test --tmpdir /tmp/afw-night-slope -T "$SLOPE" \
    > /tmp/afw-night-slope/practice.log 2>&1 &
SLOPE_PID=$!
afwdev test --tmpdir /tmp/afw-night-restart -T "$RESTART" \
    > /tmp/afw-night-restart/practice.log 2>&1 &
RESTART_PID=$!

echo "slope pid $SLOPE_PID"
echo "restart pid $RESTART_PID"

# One line every 15s for the afwfcgi processes this practice started.
: > /tmp/afw-night-cpu.log
(
    while kill -0 "$SLOPE_PID" 2>/dev/null || kill -0 "$RESTART_PID" 2>/dev/null; do
        date -u +%H:%M:%S
        ps -C afwfcgi -o pid=,etime=,pcpu=,nlwp= \
            | awk 'NR==FNR { next } { print }' 
        ps -C afwfcgi -o pid=,pcpu=,nlwp=,cmd= | grep 'afw-night' || true
        sleep 15
    done
) >> /tmp/afw-night-cpu.log 2>&1 &
WATCH=$!

wait "$SLOPE_PID"
SLOPE_RC=$?
wait "$RESTART_PID"
RESTART_RC=$?
kill "$WATCH" 2>/dev/null || true
set -e

echo "slope exit $SLOPE_RC"
echo "restart exit $RESTART_RC"
echo "----- slope -----"
tail -20 /tmp/afw-night-slope/practice.log
echo "----- restart -----"
tail -20 /tmp/afw-night-restart/practice.log
exit $((SLOPE_RC || RESTART_RC))
