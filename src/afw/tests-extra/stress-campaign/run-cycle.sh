#!/bin/bash
# Endless afwfcgi stress rotation. See README.md.
set -euo pipefail

HERE=$(cd "$(dirname "$0")" && pwd)
ROOT=$(cd "$HERE/../../../.." && pwd)
cd "$ROOT"

LOG_ROOT="${AFW_STRESS_LOG_ROOT:-/tmp/afw-stress-campaign}"
PRACTICE_S="${AFW_STRESS_PRACTICE_S:-600}"
FIREHOSE_S="${AFW_STRESS_FIREHOSE_S:-300}"
ISSUE2_S="${AFW_STRESS_ISSUE2_S:-120}"
SLEEP_S="${AFW_STRESS_SLEEP_S:-30}"

mkdir -p "$LOG_ROOT/cycles"

patch_orchestration_duration() {
    local path="$1"
    local duration="$2"
    python3 - "$path" "$duration" <<'PY'
import sys
path, duration = sys.argv[1], int(sys.argv[2])
timeout = duration + 150
lines = open(path).read().splitlines(True)
out, sd, st = [], False, False
for line in lines:
    if not sd and line.startswith("      duration_s:"):
        line = "      duration_s: %d\n" % duration
        sd = True
    elif not st and line.startswith("timeout_s:"):
        line = "timeout_s: %d\n" % timeout
        st = True
    out.append(line)
open(path, "w").write("".join(out))
if not sd or not st:
    sys.exit("did not find duration_s/timeout_s in " + path)
PY
}

stage_leaf() {
    local relpath="$1"
    local duration="${2:-}"
    local stage
    stage=$(mktemp -d /tmp/afw-stress-stage-XXXX)
    cp -a "$ROOT/src/afw/tests-extra" "$stage/tests-extra"
    if [ -n "$duration" ]; then
        patch_orchestration_duration \
            "$stage/tests-extra/$relpath/orchestration.yaml" "$duration"
    fi
    echo "$stage/tests-extra/$relpath"
}

run_leaf() {
    local name="$1"
    local tests_path="$2"
    local tmpdir="$3"
    local log="$4"
    mkdir -p "$tmpdir"
    echo "--- $name $(date -u +%Y-%m-%dT%H:%M:%SZ) ---" | tee -a "$log"
    set +e
    afwdev test --tmpdir "$tmpdir" -T "$tests_path" >> "$log" 2>&1
    local rc=$?
    set -e
    echo "exit $rc" | tee -a "$log"
    return "$rc"
}

capture_workdir() {
    local tmpdir="$1"
    local dest="$2"
    local out="$tmpdir/afwdev_test_output"
    [ -d "$out" ] || return 0
    mkdir -p "$dest"
    for d in "$out"/*/; do
        [ -d "$d" ] || continue
        cp -a "$d/diag" "$dest/$(basename "$d")-diag" 2>/dev/null || true
        cp "$d/afwfcgi.stderr.log" "$dest/$(basename "$d").stderr.log" 2>/dev/null || true
        cp "$d/afwfcgi.stdout.log" "$dest/$(basename "$d").stdout.log" 2>/dev/null || true
    done
}

append_record() {
    local cycle_dir="$1"
    local json="$2"
    printf '%s\n' "$json" >> "$cycle_dir/record.jsonl"
    printf '%s\n' "$json" >> "$LOG_ROOT/record.jsonl"
}

cycle=0
while true; do
    cycle=$((cycle + 1))
    id=$(date -u +%Y%m%dT%H%M%SZ)-c${cycle}
    cycle_dir="$LOG_ROOT/cycles/$id"
    mkdir -p "$cycle_dir"
    commit=$(git rev-parse HEAD 2>/dev/null || echo unknown)
    short=$(git rev-parse --short HEAD 2>/dev/null || echo unknown)
    branch=$(git branch --show-current 2>/dev/null || echo unknown)

    {
        echo "cycle=$id"
        echo "commit=$short"
        echo "branch=$branch"
        echo "practice_s=$PRACTICE_S"
        echo "firehose_s=$FIREHOSE_S"
        echo "issue2_s=$ISSUE2_S"
    } > "$cycle_dir/meta.txt"

    failures=0

    record_leaf() {
        local leaf="$1"
        local rc="$2"
        append_record "$cycle_dir" "$(python3 -c 'import json,sys,time; print(json.dumps({"ts":time.strftime("%Y-%m-%dT%H:%M:%SZ",time.gmtime()),"cycle":sys.argv[1],"leaf":sys.argv[2],"rc":int(sys.argv[3]),"commit":sys.argv[4]}))' \
            "$id" "$leaf" "$rc" "$commit")"
        [ "$rc" -eq 0 ] || failures=$((failures + 1))
    }

    # 1) Night pair (parallel inside practice.sh)
    practice_log="$cycle_dir/practice.log"
    set +e
    "$HERE/../night/practice.sh" "$PRACTICE_S" >> "$practice_log" 2>&1
    prc=$?
    set -e
    cp /tmp/afw-night-slope/practice.log "$cycle_dir/night-slope.harness.log" 2>/dev/null || true
    cp /tmp/afw-night-restart/practice.log "$cycle_dir/night-restart.harness.log" 2>/dev/null || true
    cp /tmp/afw-night-slope-metrics/metrics.tsv "$cycle_dir/night-slope-metrics.tsv" 2>/dev/null || true
    cp /tmp/afw-night-slope-metrics/rss-*.txt "$cycle_dir/" 2>/dev/null || true
    capture_workdir /tmp/afw-night-slope "$cycle_dir/night-slope-artifacts"
    capture_workdir /tmp/afw-night-restart "$cycle_dir/night-restart-artifacts"
    if [ "$prc" -ne 0 ]; then
        "$HERE/report-failure.sh" "$cycle_dir" night-practice "$prc" \
            > "$cycle_dir/night-practice.issue.txt" || true
        "$HERE/file-github-issue.sh" "$cycle_dir/night-practice.issue.txt" \
            "night practice (${PRACTICE_S}s): slope or restart leaf failed" \
            || true
    fi
    record_leaf night-practice "$prc"

    # 2) Model restart under load (#382 class)
    smr=$(stage_leaf stress-model-restart "")
    set +e
    run_leaf stress-model-restart "$smr" /tmp/afw-stress-smr \
        "$cycle_dir/stress-model-restart.log"
    smr_rc=$?
    set -e
    if [ "$smr_rc" -ne 0 ]; then
        capture_workdir /tmp/afw-stress-smr "$cycle_dir/stress-model-restart-artifacts"
        "$HERE/report-failure.sh" "$cycle_dir" stress-model-restart "$smr_rc" \
            > "$cycle_dir/stress-model-restart.issue.txt" || true
    fi
    record_leaf stress-model-restart "$smr_rc"

    # 3) File adapter restart stress
    sfr=$(stage_leaf stress-file-restart "")
    set +e
    run_leaf stress-file-restart "$sfr" /tmp/afw-stress-sfr \
        "$cycle_dir/stress-file-restart.log"
    sfr_rc=$?
    set -e
    if [ "$sfr_rc" -ne 0 ]; then
        capture_workdir /tmp/afw-stress-sfr "$cycle_dir/stress-file-restart-artifacts"
        "$HERE/report-failure.sh" "$cycle_dir" stress-file-restart "$sfr_rc" \
            > "$cycle_dir/stress-file-restart.issue.txt" || true
    fi
    record_leaf stress-file-restart "$sfr_rc"

    # 4) issue-2 hard loops (RSS lab)
    export AFW_ISSUE2_DURATION_S="$ISSUE2_S"
    set +e
    run_leaf issue-2-rss \
        "$ROOT/src/afw/tests-extra/issue-2/01-rss-hard-loops" \
        /tmp/afw-stress-issue2 \
        "$cycle_dir/issue-2-rss.log"
    i2_rc=$?
    set -e
    if [ "$i2_rc" -ne 0 ]; then
        capture_workdir /tmp/afw-stress-issue2 "$cycle_dir/issue-2-artifacts"
        "$HERE/report-failure.sh" "$cycle_dir" issue-2-rss "$i2_rc" \
            > "$cycle_dir/issue-2-rss.issue.txt" || true
    fi
    record_leaf issue-2-rss "$i2_rc"

    # 5) Named firehose soaks (#379)
    for leaf in 07-firehose-blast-style 07b-firehose-catalog-pool; do
        staged=$(stage_leaf "$leaf" "$FIREHOSE_S")
        safe=$(echo "$leaf" | tr '/' '-')
        set +e
        run_leaf "$leaf" "$staged" "/tmp/afw-stress-$safe" \
            "$cycle_dir/$safe.log"
        fh_rc=$?
        set -e
        if [ "$fh_rc" -ne 0 ]; then
            capture_workdir "/tmp/afw-stress-$safe" "$cycle_dir/$safe-artifacts"
            "$HERE/report-failure.sh" "$cycle_dir" "$leaf" "$fh_rc" \
                > "$cycle_dir/$safe.issue.txt" || true
        fi
        record_leaf "$leaf" "$fh_rc"
    done

    echo "cycle $id done failures=$failures" | tee -a "$cycle_dir/summary.txt"
    sleep "$SLEEP_S"
done
