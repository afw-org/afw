#!/bin/bash
# Print a GitHub-issue-shaped summary for a failed cycle directory.
set -euo pipefail

CYCLE_DIR="${1:?cycle dir}"
LEAF="${2:-unknown}"
RC="${3:-1}"

COMMIT=$(git -C "$(cd "$(dirname "$0")/../../../.." && pwd)" rev-parse HEAD 2>/dev/null || echo unknown)
SHORT=$(git -C "$(cd "$(dirname "$0")/../../../.." && pwd)" rev-parse --short HEAD 2>/dev/null || echo unknown)
BRANCH=$(git -C "$(cd "$(dirname "$0")/../../../.." && pwd)" branch --show-current 2>/dev/null || echo unknown)

cat <<EOF
## Stress campaign failure

| Field | Value |
|-------|-------|
| Leaf | \`$LEAF\` |
| Exit | $RC |
| Commit | \`$SHORT\` (\`$BRANCH\`) |
| Cycle dir | \`$CYCLE_DIR\` |

### Logs (tail)

\`\`\`
$(tail -40 "$CYCLE_DIR"/*.log 2>/dev/null | tail -80)
\`\`\`

### firehose / diag hints

$(find "$CYCLE_DIR" -name 'firehose-errors.txt' -exec tail -25 {} \; 2>/dev/null | head -40)

### RSS (night slope)

$(cat "$CYCLE_DIR"/rss-first.txt "$CYCLE_DIR"/rss-last.txt 2>/dev/null || echo '(no rss files)')

Full artifacts under \`$CYCLE_DIR\`.
EOF
