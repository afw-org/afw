#!/bin/sh
# Optional overnight helper. Edit orchestration.yaml duration_s/timeout_s
# first for a long firehose. Does not change the yaml.
set -e
LEAF="$(cd "$(dirname "$0")" && pwd)"
mkdir -p /tmp/afw-overnight-soak
rm -f /tmp/afw-overnight-soak/metrics.tsv \
      /tmp/afw-overnight-soak/rss-first.txt \
      /tmp/afw-overnight-soak/rss-last.txt

SUITE_PID=""
if [ "${AFW_OVERNIGHT_SUITE:-1}" = "1" ]; then
    (
        while true; do
            afwdev test -j --srcdir-pattern afw \
                --test-pattern 'language/script|compiler/' \
                || true
            sleep 2
        done
    ) &
    SUITE_PID=$!
fi

afwdev test -T "$LEAF" --show-all
status=$?

if [ -n "$SUITE_PID" ]; then
    kill "$SUITE_PID" 2>/dev/null || true
    wait "$SUITE_PID" 2>/dev/null || true
fi
exit "$status"
