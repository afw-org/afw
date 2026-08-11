# `afw --local` orchestrated leaf

**Author view:** `host: local` behaves like a **single-threaded FCGI** for
normal work — same `feed.kind: action`, `accept`, `sourceType` / `source`,
`expect`, `expect-stdout` as `host: afwfcgi`. The harness encodes
`++afw-local-mode-action` + perform JSON + `exit` on stdin.

**Escape hatch:** `feed.kind: local` with raw length-framed stdin (historical
`local_test` sessions, multi-directive scripts).

```bash
afwdev test --srcdir-pattern afw_command --test-pattern local-mode
afwdev test --capture-goldens --srcdir-pattern afw_command --test-pattern local-mode
```

Optional **`afw.conf`** in this directory is passed as `afw -f` (default flags,
adapters, etc.) — same conf story as other hosts.

| Case | Mode | Notes |
|------|------|--------|
| `hello-print` | action | `expect` + `expect-stdout` |
| `arithmetic` | action | simple return |
| `directives-switch` | raw local | skipped (ignore ranges) |
| `evaluate-x-afw` | raw local | x-afw wire golden |
| `issue-71-env` | raw local | env / multi-request |

Stdout compare for raw goldens normalizes the version banner so package version
string length does not force golden churn.
