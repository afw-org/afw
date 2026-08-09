# tests_special/ — opt-in tests (not in default `afwdev test -j`)

## Layout (maintainer / agent recipe)

| Tree | Role | Command |
|------|------|---------|
| **`src/*/tests/`** | Always-on **regression** (`test -j`) | `afwdev test -j` |
| **`src/*/tests_special/`** | Opt-in scenarios / load corpora | `afwdev test -T …` or `afwdev blast -T …` |

`--tests-path` / `-T` is **exclusive**: only those roots (not package `tests/`). Same flag on **test** and **blast**.

```bash
# Always-on regression
afwdev test -j

# Opt-in correctness (advanced-test, .as, …)
afwdev test -T src/afw/tests_special/adapter-lifecycle --show-all
afwdev test -T src/afw/tests_special --list

# Load thrash (not a correctness suite)
afwdev blast -T src/afw/tests_special/catalog -d 15s -c 4 -m 50
```

**Machine-readable results** (human console stays default):

```bash
# File (default --output-format json, includes failures[])
afwdev test -T src/afw/tests_special/adapter-lifecycle --output /tmp/out.json

# Pure JSON on stdout (human progress suppressed when --output is '-')
afwdev test --test-pattern catalog-value-accessors --output - --output-format json

# Compact / text / blast
afwdev test -j --output /tmp/out.json --output-format json-compact
afwdev test -T src/afw/tests_special/adapter-lifecycle --output /tmp/out.txt --output-format text
afwdev blast -T src/afw/tests_special/catalog -m 40 --output /tmp/blast.json
```

| Subdir | Purpose |
|--------|---------|
| `catalog/` | Blast corpus: value-accessor / adapter smoke scripts |
| `adapter-lifecycle/` | advanced-test: stop/start disposable file adapter vs catalog |

Full recipe: [`designs/afwdev-test-recipe.md`](../../../designs/afwdev-test-recipe.md).  
Also: `designs/afwdev-blast.md`, `designs/afwdev-advanced-test.md`.
