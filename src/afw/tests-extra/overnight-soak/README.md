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

TSV columns: `uuid`, `process_rss` (KB), `pool_bytes_in_use`,
`afw` adapter `getObjectCount`.

## Overnight

In `orchestration.yaml`:

| Field | Smoke | Overnight |
|-------|-------|-----------|
| `afwfcgi.threads` | 8 | 16 |
| `timeout_s` | 90 | 30000 |
| `duration_s` | 20 | 28800 |
| `concurrency` | 16 | 24 |
| `stopOnError` | true | false |
| `maxFailRate` | (unset) | 0.01 |

`timeout_s` must exceed `duration_s`.

Optional extra load (separate `afw` processes, not the soak `afwfcgi`):

```bash
./src/afw/tests-extra/overnight-soak/overnight-run.sh
```

That script bumps nothing; it runs the leaf as committed, and can loop
`language/` + `compiler/` tests on spare cores. Edit yaml first for an
8h firehose.

Pool: eval pin (object/array/closure/throw/concat/compile-once/nested/
template), model `onGetObject` evaluate(compile), file get/retrieve,
add/replace/delete churn, REST GET, catalog, for-let / object / array
churn, push/pop, metrics sample.
