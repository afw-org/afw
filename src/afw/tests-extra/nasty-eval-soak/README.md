# nasty-eval-soak

One hermetic `afwfcgi` (one process for the whole firehose — `afw
--local` would hide RSS leaks). Firehose of a Rube Goldberg script:
`eval_from_file`, capturing boxes, compile-once eval-many, `s=s+s`,
for-of of closures, assign/return evaluate across `{ }`, mixed throw,
`clone` + nested objects, reverse-eval of stashed units, templates,
nested evaluate, 1/0, two scratch streams (write/read/append),
push/pop/shift of boxes, linked object bag, catalog get. Functions
from `evaluate(compile)` are avoided (#342).

Default **60s**. For ~1h: `duration_s: 3600`, `timeout_s: 3900`.

```bash
afwdev test -T src/afw/tests-extra/nasty-eval-soak --show-all
```
