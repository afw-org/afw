# afwdev blast (retired)

**Status:** **Retired** (PR **#167** → `mgg-develop`).  
**Audience:** archaeology / redirect only.  
**Related:** [#157](https://github.com/afw-org/afw/issues/157) orchestrated tests, [#158](https://github.com/afw-org/afw/issues/158) afwfcgi signals, [#13](https://github.com/afw-org/afw/issues/13) stress knobs (still open for Jeremy’s story).

**Use instead:** orchestrated leaves with **`schedule.firehose`** under
[`src/afw/tests-extra/`](../src/afw/tests-extra/) (07 / 07b / 07c, etc.):

```bash
afwdev test -T src/afw/tests-extra/07b-firehose-catalog-pool
```

Schema: [`src/afw/tests-extra/SCHEMA.md`](../src/afw/tests-extra/SCHEMA.md).
Recipe: [`afwdev-test-recipe.md`](afwdev-test-recipe.md).

## Product split (current)

| Surface | Job | Default `-j` gate? |
|---------|-----|--------------------|
| **`afwdev test`** | Language/package suite + short orchestrated leaves under `src/*/tests/` | **Yes** |
| **`afwdev test -T …/tests-extra/`** | Soaks, firehose, progressive / lab leaves | **No** (opt-in) |
| **`afwdev blast`** | *(removed)* | — |

## Usage

```bash
# Typical docker/dev: nginx + afwfcgi already up — all defaults
afwdev blast

# Same idea, short aliases
afwdev blast -d 30m              # still 2×CPU concurrency by default
afwdev blast -d 1h -c 16         # override concurrency

# Managed — spawn installed afwfcgi from conf (-f like afw)
afwdev blast -f path/to/afw.conf -m 500

# Focus package suite corpus (same filters as test)
afwdev blast -d 10m -p afw --test-pattern 'file_adapter/|rql/'

# Private / out-of-suite corpus (NOT discovered by default test -j)
# Repeatable; exclusive — replaces package src/*/tests discovery
afwdev blast -T src/afw/tests-extra/07b-firehose-catalog-pool -d 15s -c 4 -m 40
afwdev blast -T /path/to/more -T src/afw/tests-extra/07b-firehose-catalog-pool -f my.conf -m 100

# Same -T on afwdev test for opt-in correctness (advanced-test, .as, …)
afwdev test -T src/afw/tests-extra/adapter-lifecycle --show-all
```

### `--tests-path` / `-T` (experimental; **test** and **blast**)

| | |
|--|--|
| **When omitted** | Package `src/*/tests` (blast: + fixture skip by default; test: normal `-j` gate) |
| **When set** | Only those directory trees (exclusive); package `tests/` ignored |
| **Default `-j`** | Never scans these roots — use `src/afw/tests-extra/` for opt-in |

Examples: `src/afw/tests-extra/07b-firehose-catalog-pool/` (blast), `src/afw/tests-extra/adapter-lifecycle/` (test advanced-test).

### Defaults (plain `afwdev blast`)

| Option | Default |
|--------|---------|
| Target | **attach** `http://localhost:8080/afw` (unless `-f`/`--conf`) |
| `--duration` / `-d` | **5m** (`0` = no time limit; then need `-m`) |
| `--concurrency` / `-c` | **2×CPU count** (`0` = auto; historical gobench-style) |
| `--threads` / `-n` | **CPU count** when managed (`0` = auto); attach: set afwfcgi `-n` yourself |
| Corpus filters | same as `test`; **skip fixture groups** by default |
| Fixtures | skip `Environment=` / `afw.conf` unless `--include-fixtures` |

### Machine summary (`--output` / `--output-format`)

Same idea as `afwdev test` (opt-in; human console stays default):

```bash
afwdev blast -T src/afw/tests-extra/07b-firehose-catalog-pool -m 40 --output /tmp/blast.json
afwdev blast -T src/afw/tests-extra/07b-firehose-catalog-pool -m 20 --output - --output-format json-compact
```

| `--output-format` | Meaning |
|-------------------|---------|
| `json` (default) | Indented JSON: `requests` (ok/fail/timeout/err/total), `latency_ms`, `failures[]`, `server_dead` |
| `json-compact` | Single-line JSON |
| `text` | Simple totals + failure lines |

When `--output` is `-`, final human “blast done …” block is suppressed so stdout is clean for the summary.

### Load diagnosis

Distinguish **too fast / overloaded** vs **real product failure**:

| Signal | Suggests |
|--------|----------|
| Timeouts rise with `-c`, vanish at low `-c` | Capacity / client timeout, not wrong expects |
| test_script fails at low `-c` too | Functional bug (suite usually already caught) |
| `err` / process death | Hard server bug; after install, **restart afwfcgi** (stale libs) |
| Latency climb over a long run | Queueing, leak, or progressive slowdown (#2 interest) |

Still optional later: latency histogram on progress line; Adaptive/runtime request metrics on afwfcgi.

### Short aliases

| Long | Short |
|------|-------|
| `--url` | `-u` |
| `--conf` | `-f` |
| `--duration` | `-d` |
| `--max-requests` | `-m` |
| `--concurrency` | `-c` |
| `--threads` | `-n` |
| `--tests-path` | `-T` |
| `--srcdir-pattern` | `-p` (package corpus only) |

Optional: personal shortcuts via **`afwdev task`** (`tasks` in afwdev-settings.json). Full agent recipe: [`afwdev-test-recipe.md`](afwdev-test-recipe.md).

## Behavior

- Corpus: `.as` test_scripts from package `tests/` (not advanced-test markers, not `.py`)
- **Default skip fixtures** so a normal blast is “server healthy under load,” not “suite expects private conf”:
  - group `config.py` with **`Environment = "..."`** (shared `tests/environments/`)
  - group-level or nearby **`afw.conf`**
  - opt in with **`--include-fixtures`** when you intentionally blast adapter/env tests
- Pick: **random** with replacement
- Adaptive fail: **continue**; count fail (expect **near-zero** without `--include-fixtures`)
- Server dead / unreachable: **stop**; exit 2
- Any fails at end: exit 1
- Console progress lines + recent failures summary; reports `skipped_fixture=N`
- Optional `--output` summary (see above)
- Ctrl+C: stop sending; managed mode tears down spawn; attach does **not** kill your afwfcgi
- Attach connection/5xx messages hint restart after install when server looks dead

## Implementation

- CLI: `cli/info.py` blast args, handler `subcommand_blast`, registry
- Logic: `_afwdev/blast/blast.py` (summary via shared `write_results_summary`)
- Reuse: test discovery/tags, advanced FCGI client + afwfcgi host spawn
- Errors: `_afwdev.common.errors` (issue **#61**) where applicable

afwfcgi graceful SIGTERM/SIGINT: **#158** (landed on `issue-158-afwfcgi-graceful-shutdown` / `mgg-develop` when merged).
