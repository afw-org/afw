# `src/afw/tests-extra` — opt-in tests (not in default `test -j`)

Sits next to **`src/afw/tests/`** (the package gate). Use this tree for
anything you want **saved and runnable**, but **not** part of the regular
`afwdev test -j` discovery:

- longer **firehose / soak** leaves (ex-blast)
- progressive / REST / multi-request **orchestrated** scenarios
- sketches and design stress leaves
- any future “lab” tests that would slow or destabilize the gate

**Run with** exclusive `-T` / `--tests-path`:

```bash
afwdev test -T src/afw/tests-extra
afwdev test -T src/afw/tests-extra/07c-firehose-max-requests
afwdev test --capture-goldens -T src/afw/tests-extra/03-progressive-to-response
```

**Related names:**

| Path | Role |
|------|------|
| `src/afw/tests/` | Default gate (`afwdev test -j`) |
| `src/afw/tests-extra/` | **This tree** — opt-in extras (orchestration, firehose, …) |

(`tests_special/` was the previous opt-in root; content lives here now.)

**Orchestration engine:** `_afwdev/test/orchestrated/` — marker
`orchestration.yaml` / `.json`. Schema: [`SCHEMA.md`](SCHEMA.md).

**Retired:** `advanced-test.yaml` as the long-term name; **`afwdev blast`**
(use `schedule.firehose` on an orchestrated leaf instead). Short firehose and
stream-expect smoke for the **gate** live under `src/afw/tests/advanced/`.

## Golden capture (`expectResponse`)

For progressive / REST bodies judged with `expectResponse: <<< goldens/…`:

```bash
afwdev test --capture-goldens -T src/afw/tests-extra/03-progressive-to-response
# review goldens/*, commit, then gate without capture:
afwdev test -T src/afw/tests-extra/03-progressive-to-response
```

Also: `AFWDEV_CAPTURE_GOLDENS=1`. Does not rewrite `//? expect` in test scripts.

## Leaf stream expects

`expect-stdout` / `expect-stderr` on a work item (hyphen keys) set
`response:stdout` / `response:stderr` on the action and compare the JSON
response properties. Gate leaf: `src/afw/tests/advanced/expect-streams/`.

With **`Accept: application/x-afw`**, prefer demux keys (see SCHEMA):

| Key | Meaning |
|-----|---------|
| `expect-response` | Concatenated **payloads** of streamId `response` (x-afw only) |
| `expect-raw-response` | Full wire body (all frames). Alias: `expectResponse` |
| `expect-stdout` / `expect-<streamId>` | Same idea per streamId |

## Host local (`afw --local`)

Same authoring as single-thread FCGI for `feed.kind: action` (`source` /
`accept` / expects). Gate suite: `src/afw_command/tests/local-mode/`.
Raw multi-segment stdin: `feed.kind: local`. Optional leaf `afw.conf` →
`afw -f`. Under `--env-mode valgrind`, the host is wrapped (with timeouts).

## Feed defaults

Document-level **`feed:`** is a **default** for every work item. Each test may
set **`feed:`** (full or partial) to **override** only what differs (e.g. one
step with `accept: application/x-afw`, or `kind: rest`). See SCHEMA.md §2.

## Mental model

| Piece | Role |
|-------|------|
| **Test scripts** (`//?` + source) | Dense sequential cases; one-process batch |
| **Orchestrated leaves** | `orchestration.yaml` + fixtures; **how** work is fed to a host |
| **Firehose** | `schedule.firehose` in an orchestrated leaf (blast replacement) |
| **`tests-extra`** | Where opt-in leaves live (this directory) |

## Layout

```text
src/afw/tests-extra/
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
| **03-progressive-to-response** | #127 wire proof + golden body |
| **04-include-test-script** | test_script as one work unit |
| **05-rest-collection** | REST feed (no source required) |
| **06-parallel-wave** | schedule.parallel |
| **07-firehose-blast-style** | Blast A: timed random firehose |
| **07b-firehose-catalog-pool** | Catalog/registry firehose (ex blast catalog corpus) |
| **07c-firehose-max-requests** | maxRequests-only firehose |
| **08-action-any-function** | non-eval action shape |
| **09-multi-eval-lifetime** | Port of multi-eval-lifetime |
| **10-catalog-value-accessors** | Port of catalog-value-accessors |
| **11-mixed-accept-json-and-x-afw** | Document feed + per-test Accept |
| **adapter-lifecycle** | #149 stop/start disposable adapter (ex tests_special) |
| **issue-2** | #2 hard-loop RSS lab + gdb helpers (optional `free` / reuse; expected red today) |

**Gate smokes** (default `test -j`, not this tree):

| Leaf | Purpose |
|------|---------|
| `tests/advanced/firehose-smoke` | Short firehose (roundRobin, maxFail) |
| `tests/advanced/expect-streams` | Leaf `expect-stdout` / `expect-stderr` |

### Blast → firehose mapping

| Old `afwdev blast` | Leaf / knobs |
|--------------------|--------------|
| `-d 30s -c 4` | `07` `duration_s` + `concurrency` |
| `-T catalog -m 40` | `07b` pool + `maxRequests` |
| `-m 100` without duration | `07c` `maxRequests` only |
| Stay out of `test -j` | Live under `tests-extra/` |

Firehose pass criteria: optional `maxFail` / `maxFailRate`; else blast-like
(tolerate mixed errors unless *all* requests fail). Summary includes
`failRate` and `rps`. Policies: `random` (default) and `roundRobin`; `seed`
applies to random.

**Not ported as orchestration YAML:** `tests/advanced/afwfcgi_signal_shutdown`
(Python special-case host test — stays under advanced/).
