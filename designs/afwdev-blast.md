# afwdev blast (experimental)

**Status:** **\*\*\* Experimental \*\*\***  
**Audience:** maintainers debugging runtime / afwfcgi under load.  
**Related:** [#157](https://github.com/afw-org/afw/issues/157) advanced-test, [#158](https://github.com/afw-org/afw/issues/158) afwfcgi signals.  
**Branch:** `feature-afwfcgi-scenario-tests` (with advanced-test).  
**Sibling:** #149 catalog work was explored in a **separate Grok session** on `issue-#149-runtime-catalog-lifetime`; merge this harness first, then use blast + advanced leaves there.

## Product split

| Command | Job | Default `-j` gate? |
|---------|-----|--------------------|
| **`afwdev test`** | Jeremy language/package suite + tiny advanced-test multi-request | **Yes** |
| **`afwdev blast`** | On-demand random suite firehose at **afwfcgi** | **No** |

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

# Private / out-of-suite corpus (NOT discovered by afwdev test -j)
# Repeatable; exclusive — replaces package src/*/tests discovery
afwdev blast -T src/afw/tests_special/catalog -d 15s -c 4 -m 40
afwdev blast -T /path/to/more -T src/afw/tests_special/catalog -f my.conf -m 100
```

### `--tests-path` / `-T` (experimental)

| | |
|--|--|
| **When omitted** | Corpus = package `src/*/tests` (plus `-p` / `--test-pattern` / `--tags`; fixture skip by default) |
| **When set** | Corpus = only those directory trees (recursive `*.as`, skip `_` names); package discovery ignored |
| **test -j** | Never scans `--tests-path` roots; put load scripts outside `tests/` (e.g. `src/afw/tests_special/`) |

Example private tree: `src/afw/tests_special/catalog/` (#149 value-accessor / adapter smoke scripts).

### Defaults (plain `afwdev blast`)

| Option | Default |
|--------|---------|
| Target | **attach** `http://localhost:8080/afw` (unless `-f`/`--conf`) |
| `--duration` / `-d` | **5m** (`0` = no time limit; then need `-m`) |
| `--concurrency` / `-c` | **2×CPU count** (`0` = auto; historical gobench-style) |
| `--threads` / `-n` | **CPU count** when managed (`0` = auto); attach: set afwfcgi `-n` yourself |
| Corpus filters | same as `test`; **skip fixture groups** by default |
| Fixtures | skip `Environment=` / `afw.conf` unless `--include-fixtures` |

### Load diagnosis (later — avoid wheel-spinning)

Distinguish **too fast / overloaded** vs **real product failure**:

| Signal | Suggests |
|--------|----------|
| Timeouts rise with `-c`, vanish at low `-c` | Capacity / client timeout, not wrong expects |
| test_script fails at low `-c` too | Functional bug (suite usually already caught) |
| `err` / process death | Hard server bug |
| Latency p50/p99 climb over a long run | Queueing, leak, or progressive slowdown (#2 interest) |
| afwfcgi accept/active/in-flight gauges | Server-side overload vs client impatience |

Possible additions (not required for first PR): separate **timeout** vs **expect** counters; optional latency histogram on progress line; blast `--fail-log path`; Adaptive/runtime **request metrics** on afwfcgi for in-flight and queue depth.

### Short aliases

| Long | Short |
|------|-------|
| `--url` | `-u` |
| `--conf` | `-f` |
| `--duration` | `-d` |
| `--max-requests` | `-m` |
| `--concurrency` | `-c` |
| `--threads` | `-n` |
| `--srcdir-pattern` | `-p` (shared with other subcommands) |

Optional: personal shortcuts via **`afwdev task`** (`tasks` in afwdev-settings.json) if you want named recipes beyond defaults.

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
- Ctrl+C: stop sending; managed mode tears down spawn; attach does **not** kill your afwfcgi

## Implementation

- CLI: `cli/info.py` `_info_blast`, handler `subcommand_blast`, registry
- Logic: `_afwdev/blast/blast.py`
- Reuse: test discovery/tags, advanced FCGI client + afwfcgi host spawn

afwfcgi graceful SIGTERM: track **#158**.
