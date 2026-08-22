# afwdev test recipe (agents & maintainers)

**Audience:** maintainers / assistants.  
**Related:** [`afwdev-advanced-test.md`](afwdev-advanced-test.md) (history), [`afwdev-blast.md`](afwdev-blast.md) (retired), `src/afw/tests-extra/`, #149. **#157 closed.**

## Trees

| Path | Role |
|------|------|
| `src/*/tests/` | Always-on **regression** — `afwdev test -j` |
| `src/*/tests-extra/` | **Opt-in** only — `-T` / `--tests-path` |

## Commands

```bash
# 1) Gate (do not break)
afwdev test -j

# 2) Narrow product regression (example #149 catalog contracts)
afwdev test --test-pattern catalog-value-accessors --show-all

# 3) Opt-in lifecycle / multi-request
afwdev test -T src/afw/tests-extra/adapter-lifecycle --show-all

# 4) Optional load thrash (firehose leaf; not the default gate)
afwdev test -T src/afw/tests-extra/07b-firehose-catalog-pool

# 5) issue #2 hard-loop RSS lab (expected red until optional free)
afwdev test -T src/afw/tests-extra/issue-2 --show-all
```

### Machine-readable summary (opt-in)

```bash
# File
afwdev test -j --output /tmp/out.json
# failures[] is clipped; exit code still non-zero on fail

# Pure JSON on stdout (human progress suppressed when --output is '-')
afwdev test --test-pattern catalog-value-accessors --output - --output-format json
```

Formats: `--output-format json` (default) | `json-compact` | `text`.

### Optional personal task (`afwdev-settings.json`)

```json
{
  "tasks": {
    "check-149": "afwdev test --test-pattern catalog-value-accessors && afwdev test -T src/afw/tests-extra/adapter-lifecycle && afwdev test -T src/afw/tests-extra/07b-firehose-catalog-pool"
  }
}
```

Then: `afwdev task check-149`

## Notes

- After `./afwdev build --install` / `--cdev`, **restart afwfcgi** if attach tests talk to a long-lived process (stale libs).  
- **`afwdev blast` is retired** — use `schedule.firehose` leaves under `tests-extra/`.
- Full verify: `./afwdev build --fulldev` then `afwdev test --env-mode valgrind -j` then `afwdev test -j`. Valgrind is heavy; if thrashing, try **`-j 4`** (full cores can still finish ~5 min when healthy on a 32-core/30 GiB box). That valgrind run wraps `.as` via `afw` **and** C probes via `run_c_probe()` (suite suppressions cover libunwind on a throw). See [`c-probes.md`](c-probes.md).
- A real `--test-pattern` shows passing cases (no need for `--show-all`). `--srcdir-pattern` only counts matching source dirs in the summary.
- `//? expect-stdout` / `expect-stderr` on Adaptive test scripts; orchestrated leaves use hyphen keys + optional x-afw demux (`expect-response`, `expect-raw-*`).
- `service_start` after stop needs conf adapter + `_AdaptiveServiceConf_` (see lifecycle leaf).
- Structured Python errors: `_afwdev.common.errors` (`AfwAdaptiveError`, `AfwdevProcessError`, `AfwdevRunnerError`) — issue **#61**; Adaptive error objects ride on `exc.object` / `to_error_dict()`.
