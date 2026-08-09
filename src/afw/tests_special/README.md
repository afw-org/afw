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

Structured machine results already use **`afwdev test --output path.json`** (console text stays human-default; do not change that silently).

| Subdir | Purpose |
|--------|---------|
| `catalog/` | Blast corpus: value-accessor / adapter smoke scripts |
| `adapter-lifecycle/` | advanced-test: stop/start disposable file adapter vs catalog |

See `designs/afwdev-blast.md` and `designs/afwdev-advanced-test.md`.
