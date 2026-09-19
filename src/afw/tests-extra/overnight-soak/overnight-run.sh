#!/bin/sh
# Optional helper: wipe metrics files and run this leaf.
# Edit orchestration.yaml duration_s/timeout_s first for a long firehose.
set -e
LEAF="$(cd "$(dirname "$0")" && pwd)"
mkdir -p /tmp/afw-overnight-soak
rm -f /tmp/afw-overnight-soak/metrics.tsv \
      /tmp/afw-overnight-soak/rss-first.txt \
      /tmp/afw-overnight-soak/rss-last.txt
exec afwdev test -T "$LEAF" --show-all
