# afwdev blast (experimental)

**Status:** **\*\*\* Experimental \*\*\***  
**Audience:** maintainers debugging runtime / afwfcgi under load.  
**Related:** [#157](https://github.com/afw-org/afw/issues/157) advanced-test, [#158](https://github.com/afw-org/afw/issues/158) afwfcgi signals.

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
afwdev blast -d 30m -c 16
afwdev blast -u http://localhost:8080/afw -d 1h

# Managed — spawn installed afwfcgi from conf (-f like afw)
afwdev blast -f path/to/afw.conf -m 500

# Focus corpus (same filters as test)
afwdev blast -d 10m -p afw --test-pattern 'file_adapter/|rql/'
```

### Defaults (plain `afwdev blast`)

| Option | Default |
|--------|---------|
| Target | **attach** `http://localhost:8080/afw` (unless `-f`/`--conf`) |
| `--duration` / `-d` | **5m** (`0` = no time limit; then need `-m`) |
| `--concurrency` / `-c` | **CPU count** (`0` = auto) |
| `--threads` / `-n` | **CPU count** when managed (`0` = auto) |
| Corpus filters | same as `test` (all matching srcdirs / `.*` tags) |

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
- Attach: skip tests whose directory has local `afw.conf` (same spirit as live test mode)
- Pick: **random** with replacement
- Adaptive fail: **continue**; count fail
- Server dead / unreachable: **stop**; exit 2
- Any fails at end: exit 1
- Console progress lines + recent failures summary
- Ctrl+C: stop sending; managed mode tears down spawn; attach does **not** kill your afwfcgi

## Implementation

- CLI: `cli/info.py` `_info_blast`, handler `subcommand_blast`, registry
- Logic: `_afwdev/blast/blast.py`
- Reuse: test discovery/tags, advanced FCGI client + afwfcgi host spawn

afwfcgi graceful SIGTERM: track **#158**.
