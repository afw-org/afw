# `afw --local` orchestrated leaf

**Author view:** `host: local` behaves like a **single-threaded FCGI** for
normal work — `feed.kind: action`, `accept`, `source`, `expect`,
`expect-stdout`. The harness encodes `++afw-local-mode-action`.

**x-afw demux expects** (also on afwfcgi):

| Key | Meaning |
|-----|---------|
| `expect-response` | Demuxed `response` stream **payloads** only |
| `expect-stdout` / other `expect-<streamId>` | Demuxed stream payloads |
| `expect-raw-response` (alias `expectResponse`) | Full raw body / local stdout |

**Escape hatch:** `feed.kind: local` with raw length-framed stdin.

```bash
afwdev test --srcdir-pattern afw_command --test-pattern local-mode
```
