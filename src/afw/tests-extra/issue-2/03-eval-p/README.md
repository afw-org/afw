# 03-eval-p — finite probes for `FIXME_GET_IT_WORKING`

Not in `afwdev test -j`. Discovery sitting: [`designs/experiment-eval-p.md`](../../../../../designs/experiment-eval-p.md) (`experiment/eval-p-scope`).

These are **finite** loops (unlike `01-rss-hard-loops`). Each script prints
`pool_bytes_in_use` and `process_rss` before/after so we can tell linear leak
from superlinear tracker climb.

```bash
src/afw/tests-extra/issue-2/03-eval-p/run.sh
# one probe
afw -s script src/afw/tests-extra/issue-2/03-eval-p/_workloads/for_let_eval.as
```

Underscore dir on purpose: `afwdev test` must not pick these up.
