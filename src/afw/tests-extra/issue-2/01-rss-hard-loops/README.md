# issue-2 leak lab (opt-in)

Hard-loop Adaptive Scripts plus RSS sampling and gdb helpers. **Not** in
`afwdev test -j`. Live leak status for umbrella
[#2](https://github.com/afw-org/afw/issues/2) lives **here**, not in the
2026-08-21 lifetime story.

The directory is `issue-2`, not `#2` — `#` starts a shell comment.

Do not mix these:

1. **Empty `{ }`** — every `{ }` is a scope ([PR #306](https://github.com/afw-org/afw/pull/306)).
   `empty_loop` (`while (true) {}`) is still **flat** (tracker last-release).
   `while (true) { let x = 1; }` still creates a scope per iteration.
2. **Braced assign** — `{ i = i + 1 }`, `{ o.x = i }`, `{ a[0] = i }` stay
   **flat** (temps die with the body frame).
3. **Unbraced assign** — compile wraps a 0-symbol `{ }` (parse Statement
   in the current block first, so `for (let x of []) let x` still
   clashes). Temps die with that frame. **Flat** (was ~80–131 MiB/s
   after #306).
4. **`function_return`** (`i = f()` inside `{ }`) is **flat** (2026-09-16
   on this branch). There is **no** leftover function-return wrapper type
   ([PR #326](https://github.com/afw-org/afw/pull/326)). Do not add one
   back. Unbraced `while (true) i = f();` is wrapped the same way.
5. **`array_push_pop`** is **flat**: `pop`/`shift` extra-hold is a temp
   on the current scope.

`empty_stmt` / `*_no_brace` still split surface syntax. Unbraced loop
bodies are `{ }` at compile. The Python judge uses
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

Measured **2026-09-16**; isolate sitting on `develop` as
[PR #340](https://github.com/afw-org/afw/pull/340). Same 8 s soaks,
2 s warmup as **2026-09-15**. Nested `{ }` last plant dropped
(`try.as` SIGSEGV); soaks unchanged. `try_catch` in_use is **flat**
(2026-09-17): managed hexBinary backtrace released on caught ENDTRY
([#341](https://github.com/afw-org/afw/issues/341) / [PR #354](https://github.com/afw-org/afw/pull/354)).
`in_use` is `env->pool_bytes_in_use` (AFW malloc not given
back). Valgrind on `afwdev test -j` does **not** catch these —
request-end bulk-free hides them. gdb `in_use` can occasionally return
garbage; if RSS is flat and `in_use` is huge or ~0, rerun that one
workload.

Fail line: RSS **8 MiB/s**, in_use **2 MiB/s**. `array_append` must grow.

| name | what | RSS / in_use (2026-09-16) | was (2026-09-15) |
|------|------|---------------------------|------------------|
| `empty_stmt` | `while (true);` | **flat / flat** | **flat / flat** |
| `empty_loop` | `while (true) {}` | **flat / flat** | **flat / flat** |
| `integer_assign_no_brace` | unbraced `i = i + 1` | **flat / flat** | **flat / flat** |
| `integer_assign` | braced `i = i + 1` | **flat / flat** | **flat / flat** |
| `object_prop_assign_no_brace` | unbraced `o.x = i` | **flat / flat** | **flat / flat** |
| `object_prop_assign` | braced `o.x = i` | **flat / flat** | **flat / flat** |
| `array_index_assign_no_brace` | unbraced `a[0] = i` | **flat / flat** | **flat / flat** |
| `array_index_assign` | braced `a[0] = i` | **flat / flat** | **flat / flat** |
| `object_rebind` | `o = { n: i }` | **flat / flat** | **flat / flat** |
| `array_rebind` | `a = [i]` | **flat / flat** | **flat / flat** |
| `string_same_size` | `"x"` / `"y"` overwrite | **flat / flat** | **flat / flat** |
| `function_return` | `i = f()` inside `{ }` | **flat / flat** | **~1.5 MiB/s both** (under bar) |
| `try_catch` | throw/catch each iter | **flat / flat** (2026-09-17) | RSS wander / ~0.25 MiB/s in_use |
| `closure_rebind` | rebind capturing function | **flat / flat** | **flat / flat** |
| `compile_once_eval` | compile once, `evaluate` loop | **flat / flat** | **flat / flat** |
| `array_push_pop` | push then pop | **flat / flat** | **flat / flat** |
| `array_append` | unbounded `push` | **must grow** (~3 MiB/s both) | must grow (~2.8 MiB/s) |

`function_return` is **flat** on this branch (managed `closure_binding`;
0-param call does not isolate enclosing last). No `function_return_value`
wrapper. Pin is on the caller.

`try_catch`: `afw_os_backtrace` returns a managed hexBinary; caught
ENDTRY / overwrite `afw_value_release`s it. 15 s soak 2026-09-17:
in_use **0 B/s**. Do not skip capture.

## Server soaks (same day)

These are `afwfcgi` firehose leaves, not the hard-loop table. They **pass**
if requests succeed. They do **not** record `process::poolBytesInUse`.
Process size sampled from `/proc` on the `afwfcgi` pid while the leaf ran:

| leaf | what | result (2026-09-15) |
|------|------|---------------------|
| `issue-2/02-pool-eval-soak` | 20 s object / nested-eval / function-return-object | **PASS**. `afwfcgi` RSS **flat** 25344 kB |
| `07-firehose-blast-style` | 30 s mixed cheap scripts | **PASS**. `afwfcgi` RSS ~39–41 MB (~0.09 MiB/s wander) |
| `07b-firehose-catalog-pool` | 40 requests (hits `maxRequests` in ~0.3 s) | **PASS**. Too short for a slope |

`07b` is not a long-running leak lab as written.

`array_push_pop`: `push` `slot_store`s; `pop`/`shift` transfer then
`afw_pool_release_value_at_cleanup` on the current scope (see
`afw_array_create_managed`). Do not `create_managed` in `array()`
(`[i]` compiles to it). Do not `get_assignable_for_lifetime` on the
pop result.

Unbraced assign: compile wraps a 0-symbol `{ }`; temps die with the trip.

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
   tracker create. `function_return` and `array_push_pop` are flat.
   Remaining climb in this lab: none of the hard-loop table
   (`try_catch` backtrace leftover closed).
2. `afw-heap` / `afw-rss` — three numbers: VmRSS,
   `xctx->p` `bytes_allocated` / `chunk_bytes`, `env->pool_bytes_in_use`.
   Two interrupts 5s apart:
   - RSS up, in_use flat → pages not returned to the OS (not AFW
     asked-for).
   - RSS and in_use up together → AFW malloc not given back.
     `try_catch` / `function_return` / `array_push_pop` are flat.
   - heap `bytes_allocated` up with in_use → xctx heap malloc not
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

Assign / overlay / rebind / empty `{ }` / `array_push_pop` /
`function_return` stay at allocator noise. Remaining growth in this
lab: hard-loop table is flat. `array_append`
should still grow.

Do not put these loops in the default gate. Correctness of assign/faces
already lives under `src/afw/tests/language/script/`.
