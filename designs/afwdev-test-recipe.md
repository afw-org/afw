# afwdev test / blast recipe (agents & maintainers)

**Audience:** maintainers / assistants.  
**Related:** [`afwdev-blast.md`](afwdev-blast.md), [`afwdev-advanced-test.md`](afwdev-advanced-test.md), #149.

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
- `service_start` after stop needs conf adapter + `_AdaptiveServiceConf_` (see lifecycle leaf).
- Structured Python errors: `_afwdev.common.errors` (`AfwAdaptiveError`, `AfwdevProcessError`, `AfwdevRunnerError`) — issue **#61**; Adaptive error objects ride on `exc.object` / `to_error_dict()`.
