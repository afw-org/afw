# issue-2 leak lab (opt-in)

Hard-loop Adaptive Scripts plus RSS sampling and gdb helpers. **Not** in
`afwdev test -j`. Live leak status for umbrella
[#2](https://github.com/afw-org/afw/issues/2) lives **here**, not in the
2026-08-21 lifetime story.

The directory is `issue-2`, not `#2` — `#` starts a shell comment.

Do not mix these (older notes did):

1. **0-symbol `{ }`** — nested empty `{ }` is not a scope (`#245`).
   `empty_loop` matches `empty_stmt` (RSS and `in_use` flat).
   `while (true) { let x = 1; }` still creates a scope per iteration.
2. **Assign / overlay / rebind** — `i = i + 1`, `o.x = i`, `o = { n: i }`,
   `a[0] = i`, `a = [i]`, same-size string, closure rebind, compile-once
   eval. After eval `p` = `scope->p` ([PR #287](https://github.com/afw-org/afw/pull/287))
   and managed scalar last-release `free_memory` via `xctx->p`, these
   soaks are **flat**. The old “unbraced scalar boxing / general-pool
   free is a no-op” climb is gone (`xctx->p` is the ST heap; optional
   `free` is real).
3. **Still climbing** — `function_return` (`i = f()`) and
   `array_push_pop`. Those are AFW malloc not given back (`in_use`
   tracks RSS), not APR pages. See the table.

`empty_stmt` / `*_no_brace` still split “no extra `{ }`” from a braced
body. They are not a remaining scalar leak. The Python judge uses
`/proc` RSS plus gdb `env->pool_bytes_in_use`. Sample from script:
`pool_bytes_in_use()` and `process_rss()`. `debug:pool` names call sites
on a **short** run — not on soaks.

Live maps: [`designs/issue-2-hold-in-inf.md`](../../../designs/issue-2-hold-in-inf.md)
(rails), [`designs/experiment-brainstorm.md`](../../../designs/experiment-brainstorm.md)
(two worlds), [`designs/experiment-eval-p.md`](../../../designs/experiment-eval-p.md).
The 08-21 story is history: [`designs/issue-2-lifetime.md`](../../../designs/issue-2-lifetime.md).

## Run the RSS suite

```bash
afwdev test -T src/afw/tests-extra/issue-2/01-rss-hard-loops --show-all
```

Each workload is one `afw -s script` process, sampled from `/proc/<pid>/status`.
Default: 8s run, 2s warmup, 2s interval. Fail if a “should stay flat” loop
grows faster than 8 MiB/s after warmup. `array_append` is the opposite
control: it **must** grow, so a broken sampler cannot silently pass.

```bash
# one or a few workloads, longer window
AFW_ISSUE2_WORKLOAD=integer_assign,object_prop_assign \
  AFW_ISSUE2_DURATION_S=15 AFW_ISSUE2_INTERVAL_S=5 \
  afwdev test -T src/afw/tests-extra/issue-2/01-rss-hard-loops --show-all

# numbers only (do not fail on slope)
AFW_ISSUE2_RSS_ASSERT=0 afwdev test -T src/afw/tests-extra/issue-2/01-rss-hard-loops --show-all
```

| env | default | meaning |
|-----|---------|---------|
| `AFW_ISSUE2_WORKLOAD` | all | comma list of names |
| `AFW_ISSUE2_DURATION_S` | `8` | wall time of each `afw` |
| `AFW_ISSUE2_INTERVAL_S` | `2` | sample period |
| `AFW_ISSUE2_WARMUP_S` | `2` | ignore samples before this for slope |
| `AFW_ISSUE2_RSS_ASSERT` | `1` | `0` = report only |

One workload without the test runner:

```bash
src/afw/tests-extra/issue-2/01-rss-hard-loops/_tools/sample-rss.sh integer_assign 20
# or
python3 src/afw/tests-extra/issue-2/01-rss-hard-loops/_rss.py --list
python3 src/afw/tests-extra/issue-2/01-rss-hard-loops/_rss.py integer_assign --duration 20 --interval 5
```

## Workloads (`_workloads/`)

Underscore dir on purpose: `afwdev test` must not evaluate these as tests
(they do not return).

Measured **2026-09-07** on `develop` after [PR #293](https://github.com/afw-org/afw/pull/293)
(10 s soaks, 2 s warmup). `in_use` is `env->pool_bytes_in_use` (AFW malloc
not given back). Valgrind on `afwdev test -j` does **not** catch these —
request-end bulk-free hides them.

| name | what | RSS / in_use |
|------|------|----------------|
| `empty_stmt` | `while (true);` | flat / flat |
| `empty_loop` | `while (true) {}` | flat / flat (0-symbol `{ }` is not a scope) |
| `integer_assign_no_brace` | unbraced `i = i + 1` | flat / flat |
| `integer_assign` | braced `i = i + 1` | flat / flat |
| `object_prop_assign_no_brace` | unbraced `o.x = i` | flat / flat |
| `object_prop_assign` | braced `o.x = i` | flat / flat |
| `array_index_assign_no_brace` | unbraced `a[0] = i` | flat / flat |
| `array_index_assign` | braced `a[0] = i` | flat / flat |
| `object_rebind` | `o = { n: i }` | flat / flat |
| `array_rebind` | `a = [i]` | flat / flat |
| `string_same_size` | `"x"` / `"y"` overwrite | flat / flat |
| `function_return` | `i = f()` | **~150 MiB/s both** — FRV wrapper left in caller `p` |
| `try_catch` | throw/catch each iter | ~0.3 MiB/s RSS, `in_use` flat (APR; under fail bar) |
| `closure_rebind` | rebind capturing function | flat / flat |
| `compile_once_eval` | compile once, `evaluate` loop | flat / flat |
| `array_push_pop` | push then pop | **~115 MiB/s both** — managed entry in `xctx->p` + pop transfer |
| `array_append` | unbounded `push` | **must grow** (harness) |

`function_return`: unique FRV `get_assignable_value` transfers the occupant
and leaves the wrapper header in `self->p` until that pool dies. A 0-symbol
`while` body shares the enclosing frame tracker, so wrappers pile up.

`array_push_pop`: managed `push` calloc’s a ring entry in `xctx->p` and
`slot_store`s (extra hold). `pop` unlinks and transfers the occupant; it
does not `free_memory` the entry and does not drop that hold. Then
`i = i + 1` cannot last-release the old integer.

## gdb

Installed `afw` / `libafw` need debug info (the usual `--cdev` build).

```bash
# start under gdb (does not `run` until you type it)
src/afw/tests-extra/issue-2/01-rss-hard-loops/_tools/gdb-run.sh integer_assign
# (gdb) run
# Ctrl-C when RSS is climbing
# (gdb) afw-help
# (gdb) afw-rss
# (gdb) afw-bt
# (gdb) afw-heap          # needs a frame with xctx
# (gdb) afw-breaks        # slot_store / integer create / heap; skips missing names
```

Attach to an already-running loop (better when you want full speed, then
stop):

```bash
afw -s script src/afw/tests-extra/issue-2/01-rss-hard-loops/_workloads/integer_assign.as &
src/afw/tests-extra/issue-2/01-rss-hard-loops/_tools/gdb-attach.sh $!
# or newest afw:
src/afw/tests-extra/issue-2/01-rss-hard-loops/_tools/gdb-attach.sh
```

Useful hunts after Ctrl-C (**no debug flags** on a soak):

1. `afw-bt` — braced empty `{ }` should sit in while / boolean, not
   tracker create. Remaining climbs: `function_return` sits in FRV
   create / `get_assignable_value`; `array_push_pop` sits in managed
   setter push / pop.
2. `afw-heap` / `afw-rss` — three numbers: VmRSS,
   `evaluation_heap->bytes_allocated`, `env->pool_bytes_in_use`.
   Two interrupts 5s apart:
   - RSS up, in_use flat → APR (tracker headers, pages not returned).
     `try_catch` is this shape, under the fail bar.
   - RSS and in_use up together → AFW malloc not given back (`xctx->p`
     or leftovers on a tracker that never last-releases). That is
     `function_return` and `array_push_pop`.
   - heap `bytes_allocated` up with in_use → eval-heap malloc not
     given back.
3. Do **not** `call afw_os_get_rss()` from gdb after SIGSTOP.
   `>debug pool` tags: `in_use` (this pool), `total` (env), `rss`
   (process KB).
4. `afw-breaks` then `continue` on a **slow** script only.

## #242 debug lines (`debug:pool`)

Compile-time probes are on for `--cdev` / `--fulldev`; **runtime flags
stay off** until `flag_set`. Do **not** `flag_set` `debug:pool` or
`debug:evaluation` in soak workloads — I/O dominates RSS.

Short finite loop, stderr to a file, then summarize (not the log):

```bash
# 50 iterations, debug:pool only (not :detail, not evaluation)
afw -s script /tmp/fifty.as 2>/tmp/pool.log
python3 src/afw/tests-extra/issue-2/01-rss-hard-loops/_trace.py /tmp/pool.log
```

From script without gdb (finite loops / tests):

```adaptive
let used = pool_bytes_in_use();
let rss = process_rss();
```

`flag_set` lasts the whole process. Isolation = separate `.as`.
`debug:evaluation` is the flow log for `_trace.py`; do not turn it
on a soak. `debug:pool:detail` adds `alloc reuse` vs `alloc apr`.

If a symbol is missing (`nm` on this `libafw` may not export
`afw_value_slot_store`), break by file:

```
(gdb) break afw_value.c:104
(gdb) break afw_pool_heap.c:909
```

`_tools/gdb-run.sh` adds `-d src/afw/{pool,value,xctx,function,object}`.

## What “fixed” looks like

Assign / overlay / rebind / empty `{ }` are already at allocator noise
(0 KiB/s RSS, ~0 `in_use` on 2026-09-07). Remaining red:
`function_return` and `array_push_pop`. `array_append` should still grow.
`try_catch` may show a small RSS-only APR climb; that is not `in_use`.

Do not put these loops in the default gate. Correctness of assign/faces
already lives under `src/afw/tests/language/script/`.
