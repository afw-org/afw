# `afw --local` orchestrated leaf

Hermetic tests for the **`afw --local`** stdin protocol (including
`application/x-afw` frames). Replaces the former `local_test.py` +
`local_test_*_{input,expect}.txt` harness.

```bash
afwdev test --srcdir-pattern afw_command --test-pattern local-mode
# refresh goldens after intentional protocol changes:
afwdev test --capture-goldens --srcdir-pattern afw_command --test-pattern local-mode
```

| Case | Role |
|------|------|
| `directives-switch` | Skipped — needs ignore ranges for times/UUIDs |
| `evaluate-x-afw` | Evaluate with Accept x-afw |
| `issue-71-env` | Environment / multi-request properties |

`host: local` does **not** require `afw.conf` (built-in default conf). Compare
normalizes the version-dependent local-mode banner so goldens need not track
package version string length.
