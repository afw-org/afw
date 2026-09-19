# overnight-soak

One hermetic `afwfcgi`, mixed requests, Adaptive metrics sampler appends
`/tmp/afw-overnight-soak/metrics.tsv`.

Not in `afwdev test -j`. Default `duration_s` is a **20s smoke** so
`afwdev test -T src/afw/tests-extra` does not run for hours.

```bash
mkdir -p /tmp/afw-overnight-soak
afwdev test -T src/afw/tests-extra/overnight-soak --show-all
cat /tmp/afw-overnight-soak/metrics.tsv
```

TSV columns: `uuid`, `process_rss` (KB), `pool_bytes_in_use`,
`afw` adapter `getObjectCount`.

## Overnight

In `orchestration.yaml`:

| Field | Smoke | Overnight |
|-------|-------|-----------|
| `afwfcgi.threads` | 8 | 8 (or 16) |
| `timeout_s` | 90 | 30000 |
| `duration_s` | 20 | 28800 |
| `concurrency` | 16 | 16 (or 32) |
| `stopOnError` | true | false |
| `maxFailRate` | (unset) | 0.01 |

`timeout_s` must exceed `duration_s` (the runner aborts the firehose at
leaf timeout).

Pool: cheap arithmetic, catalog retrieve, `evaluate(compile)` object /
array / closure / throw / `s=s+s` / compile-once, model `onGetObject`
that evaluate(compile)s, file get, metrics sample (round-robin so the
sampler keeps writing).
