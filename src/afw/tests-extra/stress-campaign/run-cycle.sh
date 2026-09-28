#!/bin/bash
# Endless afwfcgi stress rotation. See README.md.
set -euo pipefail

HERE=$(cd "$(dirname "$0")" && pwd)
ROOT=$(cd "$HERE/../../../.." && pwd)
cd "$ROOT"

LOG_ROOT="${AFW_STRESS_LOG_ROOT:-/tmp/afw-stress-campaign}"
PRACTICE_S="${AFW_STRESS_PRACTICE_S:-600}"
FIREHOSE_S="${AFW_STRESS_FIREHOSE_S:-600}"
ISSUE2_S="${AFW_STRESS_ISSUE2_S:-300}"
SLEEP_S="${AFW_STRESS_SLEEP_S:-15}"
PARALLEL="${AFW_STRESS_PARALLEL:-1}"
SERVER_THREADS="${AFW_STRESS_SERVER_THREADS:-50%}"
CLIENT_CPUS="${AFW_STRESS_CLIENT_CPUS:-50%}"
FH_CONCURRENCY="${AFW_STRESS_FIREHOSE_CONCURRENCY:-100%}"
PATCH="$HERE/patch-orchestration.py"

mkdir -p "$LOG_ROOT/cycles"

stage_leaf() {
    local relpath="$1"
    local duration="${2:-}"
    local threads="${3:-$SERVER_THREADS}"
    local drop_max="${4:-0}"
    local stage
    stage=$(mktemp -d /tmp/afw-stress-stage-XXXX)
    cp -a "$ROOT/src/afw/tests-extra" "$stage/tests-extra"
    local yaml="$stage/tests-extra/$relpath/orchestration.yaml"
    if [ -n "$duration" ]; then
        extra=()
        [ "$drop_max" = 1 ] && extra+=(--drop-max-requests)
        python3 "$PATCH" "$yaml" --duration "$duration" \
            --server-threads "$threads" \
            --concurrency "$FH_CONCURRENCY" \
            --client-processes "$CLIENT_CPUS" \
            "${extra[@]}"
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

run_leaf_bg() {
    local name="$1"
    local tests_path="$2"
    local tmpdir="$3"
    local log="$4"
    local rcfile="$5"
    (
        run_leaf "$name" "$tests_path" "$tmpdir" "$log"
        echo $? > "$rcfile"
    ) &
    echo $!
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

on_leaf_fail() {
    local cycle_dir="$1"
    local leaf="$2"
    local rc="$3"
    local tmpdir="$4"
    local artifact="$5"
    capture_workdir "$tmpdir" "$cycle_dir/$artifact"
    "$HERE/report-failure.sh" "$cycle_dir" "$leaf" "$rc" \
        > "$cycle_dir/$leaf.issue.txt" 2>/dev/null || true
    if [ -f "$cycle_dir/$leaf.issue.txt" ]; then
        "$HERE/file-github-issue.sh" "$cycle_dir/$leaf.issue.txt" \
            "stress campaign: $leaf failed (rc=$rc)" || true
    fi
}

cycle=0
while true; do
    cycle=$((cycle + 1))
    id=$(date -u +%Y%m%dT%H%M%SZ)-c${cycle}
    cycle_dir="$LOG_ROOT/cycles/$id"
    mkdir -p "$cycle_dir/pids"
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
        echo "parallel=$PARALLEL"
        echo "server_threads=$SERVER_THREADS"
    } > "$cycle_dir/meta.txt"

    failures=0

    record_leaf() {
        local leaf="$1"
        local rc="$2"
        append_record "$cycle_dir" "$(python3 -c 'import json,sys,time; print(json.dumps({"ts":time.strftime("%Y-%m-%dT%H:%M:%SZ",time.gmtime()),"cycle":sys.argv[1],"leaf":sys.argv[2],"rc":int(sys.argv[3]),"commit":sys.argv[4]}))' \
            "$id" "$leaf" "$rc" "$commit")"
        [ "$rc" -eq 0 ] || failures=$((failures + 1))
    }

    export AFW_STRESS_SERVER_THREADS="$SERVER_THREADS"
    export AFW_STRESS_CLIENT_CPUS="$CLIENT_CPUS"
    export AFW_STRESS_FIREHOSE_CONCURRENCY="$FH_CONCURRENCY"
    export AFW_STRESS_RESTART_THREADS="${AFW_STRESS_RESTART_THREADS:-8}"
    export AFW_ISSUE2_DURATION_S="$ISSUE2_S"

    practice_log="$cycle_dir/practice.log"
    if [ "$PARALLEL" = 1 ]; then
        echo "=== parallel wave $id ===" | tee -a "$cycle_dir/summary.txt"
        set +e
        "$HERE/../night/practice.sh" "$PRACTICE_S" >> "$practice_log" 2>&1 &
        pr_pid=$!
        echo "$pr_pid" > "$cycle_dir/pids/night-practice.pid"

        smr=$(stage_leaf stress-model-restart "$PRACTICE_S" "50%")
        sfr=$(stage_leaf stress-file-restart "$PRACTICE_S" "50%")
        fh7=$(stage_leaf 07-firehose-blast-style "$FIREHOSE_S" "50%")
        fh7b=$(stage_leaf 07b-firehose-catalog-pool "$FIREHOSE_S" "50%" 1)
        sfcgi=$(stage_leaf stress-fcgi "$FIREHOSE_S" "50%")
        for spec in \
            "stress-model-restart|$smr|/tmp/afw-stress-smr" \
            "stress-file-restart|$sfr|/tmp/afw-stress-sfr" \
            "07-firehose-blast-style|$fh7|/tmp/afw-stress-07-firehose-blast-style" \
            "07b-firehose-catalog-pool|$fh7b|/tmp/afw-stress-07b-firehose-catalog-pool" \
            "stress-fcgi|$sfcgi|/tmp/afw-stress-fcgi"; do
            name="${spec%%|*}"
            rest="${spec#*|}"
            tpath="${rest%%|*}"
            tmp="${rest#*|}"
            pid=$(run_leaf_bg "$name" "$tpath" "$tmp" \
                "$cycle_dir/$name.log" \
                "$cycle_dir/pids/$name.rc")
            echo "$pid" > "$cycle_dir/pids/$name.pid"
        done

        i2_pid=$(run_leaf_bg issue-2-rss \
            "$ROOT/src/afw/tests-extra/issue-2/01-rss-hard-loops" \
            /tmp/afw-stress-issue2 \
            "$cycle_dir/issue-2-rss.log" \
            "$cycle_dir/pids/issue-2-rss.rc")
        echo "$i2_pid" > "$cycle_dir/pids/issue-2-rss.pid"

        wait "$pr_pid"
        prc=$?
        for name in stress-model-restart stress-file-restart \
            07-firehose-blast-style 07b-firehose-catalog-pool stress-fcgi issue-2-rss; do
            pid=$(cat "$cycle_dir/pids/$name.pid" 2>/dev/null || true)
            [ -n "$pid" ] && wait "$pid" || true
        done
        set -e

        smr_rc=$(cat "$cycle_dir/pids/stress-model-restart.rc" 2>/dev/null || echo 1)
        sfr_rc=$(cat "$cycle_dir/pids/stress-file-restart.rc" 2>/dev/null || echo 1)
        fh7_rc=$(cat "$cycle_dir/pids/07-firehose-blast-style.rc" 2>/dev/null || echo 1)
        fh7b_rc=$(cat "$cycle_dir/pids/07b-firehose-catalog-pool.rc" 2>/dev/null || echo 1)
        sfcgi_rc=$(cat "$cycle_dir/pids/stress-fcgi.rc" 2>/dev/null || echo 1)
        i2_rc=$(cat "$cycle_dir/pids/issue-2-rss.rc" 2>/dev/null || echo 1)
    else
        set +e
        "$HERE/../night/practice.sh" "$PRACTICE_S" >> "$practice_log" 2>&1
        prc=$?
        set -e
        smr_rc=sfr_rc=fh7_rc=fh7b_rc=sfcgi_rc=i2_rc=0
    fi

    cp /tmp/afw-night-slope/practice.log "$cycle_dir/night-slope.harness.log" 2>/dev/null || true
    cp /tmp/afw-night-restart/practice.log "$cycle_dir/night-restart.harness.log" 2>/dev/null || true
    cp /tmp/afw-night-slope-metrics/metrics.tsv "$cycle_dir/night-slope-metrics.tsv" 2>/dev/null || true
    cp /tmp/afw-night-slope-metrics/rss-*.txt "$cycle_dir/" 2>/dev/null || true
    capture_workdir /tmp/afw-night-slope "$cycle_dir/night-slope-artifacts"
    capture_workdir /tmp/afw-night-restart "$cycle_dir/night-restart-artifacts"

    [ "$prc" -eq 0 ] || on_leaf_fail "$cycle_dir" night-practice "$prc" \
        /tmp/afw-night-slope night-slope-artifacts
    record_leaf night-practice "$prc"

    [ "$smr_rc" -eq 0 ] || on_leaf_fail "$cycle_dir" stress-model-restart "$smr_rc" \
        /tmp/afw-stress-smr stress-model-restart-artifacts
    record_leaf stress-model-restart "$smr_rc"

    [ "$sfr_rc" -eq 0 ] || on_leaf_fail "$cycle_dir" stress-file-restart "$sfr_rc" \
        /tmp/afw-stress-sfr stress-file-restart-artifacts
    record_leaf stress-file-restart "$sfr_rc"

    [ "$fh7_rc" -eq 0 ] || on_leaf_fail "$cycle_dir" 07-firehose-blast-style "$fh7_rc" \
        /tmp/afw-stress-07-firehose-blast-style 07-firehose-blast-style-artifacts
    record_leaf 07-firehose-blast-style "$fh7_rc"

    [ "$fh7b_rc" -eq 0 ] || on_leaf_fail "$cycle_dir" 07b-firehose-catalog-pool "$fh7b_rc" \
        /tmp/afw-stress-07b-firehose-catalog-pool 07b-firehose-catalog-pool-artifacts
    record_leaf 07b-firehose-catalog-pool "$fh7b_rc"

    [ "$sfcgi_rc" -eq 0 ] || on_leaf_fail "$cycle_dir" stress-fcgi "$sfcgi_rc" \
        /tmp/afw-stress-fcgi stress-fcgi-artifacts
    record_leaf stress-fcgi "$sfcgi_rc"

    [ "$i2_rc" -eq 0 ] || on_leaf_fail "$cycle_dir" issue-2-rss "$i2_rc" \
        /tmp/afw-stress-issue2 issue-2-artifacts
    record_leaf issue-2-rss "$i2_rc"

    echo "cycle $id done failures=$failures" | tee -a "$cycle_dir/summary.txt"
    sleep "$SLEEP_S"
done
