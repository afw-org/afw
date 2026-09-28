#!/bin/bash
# Create or update a GitHub issue from a failure report (dedupe by title hash).
set -euo pipefail

REPORT="${1:?report file (e.g. cycle-dir/night-practice.issue.txt)}"
TITLE="${2:?issue title}"

HERE=$(cd "$(dirname "$0")" && pwd)
ROOT=$(cd "$HERE/../../../.." && pwd)
SIG_FILE="${AFW_STRESS_LOG_ROOT:-/tmp/afw-stress-campaign}/issue-signatures.txt"
mkdir -p "$(dirname "$SIG_FILE")"
hash=$(printf '%s' "$TITLE" | sha256sum | awk '{print $1}')

if [ -f "$SIG_FILE" ] && grep -q "^$hash " "$SIG_FILE"; then
    num=$(grep "^$hash " "$SIG_FILE" | awk '{print $2}')
    echo "Already filed as issue #$num — commenting"
    gh issue comment "$num" --body-file "$REPORT"
    exit 0
fi

num=$(gh issue create --title "$TITLE" --body-file "$REPORT" | awk -F/ '{print $NF}')
echo "$hash $num" >> "$SIG_FILE"
echo "Created issue #$num"
