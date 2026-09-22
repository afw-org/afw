# overnight-soak

One hermetic `afwfcgi`, mixed requests, Adaptive metrics sampler appends
`/tmp/afw-overnight-soak/metrics.tsv`. After the firehose, `rss_check`
fails if RSS grew **64 MiB** from the first sample.

Not in `afwdev test -j`. The default is a check-in, not an overnight
run: **8 sequential model stop/start cycles**, each followed by a
metrics sample, then a **60s** firehose. Model swap stays out of the
firehose (a firehose of that swap grew RSS about 1 GiB/min).

Container disk: this leaf writes TSV + two small RSS files under
`/tmp/afw-overnight-soak` (check-in is a few MiB of TSV; 8h at this
sample rate is on the order of a few hundred MiB, not GiB). `adapter_churn` add/delete
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

| Field | Check-in (default) | 8h run 2026-09-19 |
|-------|--------------------|-------------------|
| model stop/start | 8, before the firehose, sampled | same schedule, then the long firehose |
| `afwfcgi.threads` | 8 | 32 |
| `timeout_s` | 180 | 30000 |
| `duration_s` | 60 | 28800 |
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

RSS grew 42 MiB from the first sample (under 64 MiB). After warmup it sat in a ~6 MiB band. The default stays the check-in above so `test -T tests-extra` is not 8 hours. Model stop/start is the first work after the baseline sample; raising `duration_s` alone does not run it again.

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
