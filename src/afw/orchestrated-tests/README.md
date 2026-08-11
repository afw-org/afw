# Orchestrated tests

**Status:** **Runner implemented** (`_afwdev/test/orchestrated/`). Marker
**`orchestration.yaml` / `.json`**. Gate leaves under `src/afw/tests/advanced/`
(and `tests_special/` when opted in). This tree is **outside** default
`src/*/tests` discovery — use
`afwdev test -T src/afw/orchestrated-tests/...` for sketches and long firehose.

**Retired:** `advanced-test.yaml` marker and **`afwdev blast`** subcommand
(use `schedule.firehose` on an orchestrated leaf instead).

**Branch / campaign:** feature work on **`mgg-develop`** for heavy review
before a big merge to **`develop`**. See [`beta-backlog.md`](../../../beta-backlog.md).

## Golden capture (`expectResponse`)

For progressive / REST bodies judged with `expectResponse: <<< goldens/…`:

```bash
# create or refresh goldens from actual response bodies
afwdev test --capture-goldens -T src/afw/orchestrated-tests/03-progressive-to-response
# review goldens/*.xafw (or whatever path), commit, then gate without capture:
afwdev test -T src/afw/orchestrated-tests/03-progressive-to-response
```

Also: `AFWDEV_CAPTURE_GOLDENS=1`. Does not rewrite `//? expect` in test scripts.

## Leaf stream expects

`expect-stdout` / `expect-stderr` on a work item (hyphen keys) set
`response:stdout` / `response:stderr` on the action and compare the JSON
response properties. See gate leaf `src/afw/tests/advanced/expect-streams/`.

## Feed defaults (yes — helpful)

Document-level **`feed:`** is a **default** for every work item. Each test may
set **`feed:`** (full or partial) to **override** only what differs (e.g. one
step with `accept: application/x-afw`, or `kind: rest`). That keeps leaves DRY
without losing per-request control. See SCHEMA.md §2.

## Mental model

| Piece | Role |
|-------|------|
| **Test scripts** (`//?` + source) | Dense sequential cases; directives + source; one-process batch |
| **Orchestrated tests** | **Orchestration file** + **tests** + fixtures; **how** work is fed to a host |
| **Blast** (today) | Direction: heavy orchestration leaves + schedule.firehose; separate discovery root |

**Pretend pattern:** sketch → throw scenarios at it → trim for v1.
(`designs/mantras-and-working-style.md` — *Design method*).

## Layout

```text
src/afw/orchestrated-tests/
  README.md
  SCHEMA.md
  <leaf>/
    orchestration.yaml|json
    afw.conf
    tests/ …
```

## Scenario matrix

| Leaf | Origin / purpose |
|------|------------------|
| **01-smoke-sequential** | Port of `tests/advanced/smoke` |
| **01b-smoke-json** | Port of `tests/advanced/smoke-json` (JSON marker) |
| **02-multi-request-file** | Port of `tests/advanced/multi-request-file` |
| **03-progressive-to-response** | #127 wire proof sketch (new) |
| **04-include-test-script** | test_script as one work unit (new) |
| **05-rest-collection** | REST feed sketch (new) |
| **06-parallel-wave** | schedule.parallel (**later**) |
| **07-firehose-blast-style** | Blast A: timed random firehose (`blast -d`) |
| **07b-firehose-catalog-pool** | Blast B: catalog pool (`blast -T catalog -m`) |
| **07c-firehose-max-requests** | Blast C: maxRequests-only firehose (`blast -m`) |
| **gate:** `tests/advanced/firehose-smoke` | Short firehose (roundRobin, maxFail) in default `test -j` |
| **gate:** `tests/advanced/expect-streams` | Leaf `expect-stdout` / `expect-stderr` |
| **08-action-any-function** | non-eval action shape (**later**) |
| **09-multi-eval-lifetime** | Port of `tests/advanced/multi-eval-lifetime` |
| **10-catalog-value-accessors** | Port of `tests/advanced/catalog-value-accessors` |
| **11-mixed-accept-json-and-x-afw** | Document feed JSON + per-test `accept: application/x-afw` |

### Blast → orchestration mapping (pretend)

| Today `afwdev blast` | Orchestration sketch |
|----------------------|----------------------|
| `-d 30s -c 4` | `07` `schedule.firehose.duration_s` + `concurrency` |
| `-T tests_special/catalog -m 40` | `07b` pool of catalog scripts + `maxRequests` |
| `-m 100` without duration | `07c` `maxRequests` only |
| Default package tests thrash | `fromTests` / later `includeGlob` over a test pool |
| Stay out of `test -j` | Leaves live under `orchestrated-tests/` (or future load root) |

Firehose pass criteria: optional `maxFail` / `maxFailRate`; else blast-like
(tolerate mixed errors unless *all* requests fail). Summary includes
`failRate` and `rps`. Policies: `random` (default) and `roundRobin`; `seed`
applies to random.

**Not ported as orchestration YAML:** `tests/advanced/afwfcgi_signal_shutdown`
(Python special-case host test — stays as-is under advanced/).

## Old format still supported

| Location | Marker | Runner |
|----------|--------|--------|
| `src/afw/tests/advanced/*` | `advanced-test.yaml` / `.json` | `_afwdev/test/advanced/` — **live** |
| `src/afw/orchestrated-tests/*` | `orchestration.yaml` / `.json` | **sketch only** until implemented |

Do **not** delete or break advanced leaves while the new schema is experimental.

## Gaps (from pretend stress)

1. Canonical file ref: `<<<` vs `sourcePath`  
2. Real FCGI path for `sourceType: test_script`  
3. `feed.args` for arbitrary Adaptive functions  
4. Soft expects for env-dependent values  
5. Golden capture workflow for `expectResponse`  
6. Firehose success criteria  

## Related

- Live pad: `designs/afwdev-advanced-test.md`  
- Runner: `src/afw_dev/_afwdev/test/advanced/`  
