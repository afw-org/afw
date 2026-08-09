# tests_special/ — opt-in tests (not in default `afwdev test -j`)

Regression suite for always-on gates lives under `src/*/tests/`.

This tree is for **opt-in** scenarios and load corpora:

```bash
# Correctness (advanced-test, .as, …) — exclusive roots
afwdev test -T src/afw/tests_special/adapter-lifecycle --show-all
afwdev test -T src/afw/tests_special --test-pattern adapter-lifecycle

# Load thrash
afwdev blast -T src/afw/tests_special/catalog -d 15s -m 50
```

| Subdir | Purpose |
|--------|---------|
| `catalog/` | Blast corpus: value-accessor / adapter smoke scripts |
| `adapter-lifecycle/` | advanced-test: stop/start disposable file adapter vs catalog |

See `designs/afwdev-blast.md` and `designs/afwdev-advanced-test.md`.
