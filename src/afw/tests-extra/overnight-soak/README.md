# overnight-soak

One hermetic `afwfcgi`, mixed requests, Adaptive metrics sampler appends
`/tmp/afw-overnight-soak/metrics.tsv`. After the firehose, `rss_check`
fails if RSS grew **64 MiB** from the first sample.

Not in `afwdev test -j`. Default `duration_s` is a **20s smoke** so
`afwdev test -T src/afw/tests-extra` does not run for hours.

Container disk: this leaf writes TSV + two small RSS files under
`/tmp/afw-overnight-soak` (smoke ~0.2 MiB; 8h at the 20s rate is on
the order of a few hundred MiB, not GiB). `adapter_churn` add/delete
so the file adapter does not accumulate objects. The harness wipes
`/tmp/afwdev_test_output` each leaf.

```bash
mkdir -p /tmp/afw-overnight-soak
rm -f /tmp/afw-overnight-soak/metrics.tsv \
      /tmp/afw-overnight-soak/rss-first.txt \
      /tmp/afw-overnight-soak/rss-last.txt
afwdev test -T src/afw/tests-extra/overnight-soak --show-all
cat /tmp/afw-overnight-soak/rss-first.txt /tmp/afw-overnight-soak/rss-last.txt
```

TSV columns (tab-separated), from **`process::`** and
**`_AdaptiveServer_/current`**:

| col | source |
|-----|--------|
| uuid | `generate_uuid()` |
| rss | `process::rss` (bytes) |
| poolBytesInUse | `process::poolBytesInUse` |
| peakPoolBytesInUse | `process::peakPoolBytesInUse` |
| poolChunkBytes | `process::poolChunkBytes` |
| peakPoolChunkBytes | `process::peakPoolChunkBytes` |
| concurrent | server `concurrent` |
| maxConcurrent | server `maxConcurrent` |
| requestCount | server `requestCount` |
| getObjectCount | `_AdaptiveAdapter_/afw` metrics |
| memoryRegionBytesInUse | `process::memoryRegionBytesInUse` |
| memoryRegionRegionsInUse | `process::memoryRegionRegionsInUse` |
| memoryRegionFreeListBytes | `process::memoryRegionFreeListBytes` |
| memoryRegionFreeListCount | `process::memoryRegionFreeListCount` |
| memoryRegionGetHits | `process::memoryRegionGetHits` |
| memoryRegionGetMisses | `process::memoryRegionGetMisses` |
| memoryRegionFreeOverCap | `process::memoryRegionFreeOverCap` |
| memoryRegionPeakBytesInUse | `process::memoryRegionPeakBytesInUse` |
| memoryRegionPeakFreeListBytes | `process::memoryRegionPeakFreeListBytes` |

`rss_check` uses `process::rss` bytes (64 MiB growth cap).

## Overnight

In `orchestration.yaml`:

| Field | Smoke (default) | 8h run 2026-09-19 |
|-------|-----------------|-------------------|
| `afwfcgi.threads` | 8 | 32 |
| `timeout_s` | 90 | 30000 |
| `duration_s` | 20 | 28800 |
| `concurrency` | 16 | 32 |
| `stopOnError` | true | false |
| `maxFailRate` | (unset) | 0.01 |

That 8h run **passed** (`28806s`, `rss_check` held):

| | |
|--|--|
| `requestCount` | 1 → 39,763,411 |
| TSV samples | 1,529,362 |
| `process::rss` | 23.8 → 66.1 MiB (peak 69.2) |
| After warmup (skip first 5%) | 62.9–69.2 MiB |
| `poolBytesInUse` | ~4.5–8.3 MiB |
| `poolChunkBytes` | ~10.9–16.9 MiB |
| `concurrent` | 1–29 (`maxConcurrent` 32) |

RSS grew 42 MiB from the first sample (under 64 MiB). After warmup it sat in a ~6 MiB band. Default yaml stays the 20s smoke so `test -T tests-extra` is not 8 hours.

`timeout_s` must exceed `duration_s`.

```bash
./src/afw/tests-extra/overnight-soak/overnight-run.sh
```

That only wipes the TSV/RSS files and runs this leaf. Edit yaml first
for an 8h firehose. It does **not** loop `afwdev test -j`.

Pool: eval pin, model `onGetObject` evaluate(compile), file CRUD,
REST GET, catalog, language-shaped scripts, **model stop/start with
onGetObject swap** (isolated `swap-model` / `swap-backend`; sequential
warmup — firehose restart leaked RSS), **file produce/consume** inbox
to outbox, **nasty-eval** and anniversary Rube Goldberg in the
firehose, plus **gate `test_script` files** as FCGI actions:
`function`, `let_const`, `for`, `void_result`, `pool_eval_lifetime`,
`mini_suite`. Copies live in `tests/suites/`.
