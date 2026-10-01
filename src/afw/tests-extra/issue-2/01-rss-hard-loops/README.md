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
4. **`function_return`** (`i = f()` inside `{ }`) is **flat**. There is **no** leftover function-return wrapper type
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
Default: **15 s** run, **5 s** warmup, **5 s** interval. Each workload has a
class (`flat` / `climb` / `grow`) and its own `in_use` ceiling. Disaster RSS
is still 8 MiB/s. `array_append` **must** grow, so a broken sampler cannot
silently pass. The slope line is on the case name (no `--verbose` needed).

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
| `AFW_ISSUE2_DURATION_S` | `15` | wall time of each `afw` |
| `AFW_ISSUE2_INTERVAL_S` | `5` | sample period |
| `AFW_ISSUE2_WARMUP_S` | `5` | ignore samples before this for slope |
| `AFW_ISSUE2_RSS_ASSERT` | `1` | `0` = report only |

| class | `in_use` fail | examples |
|-------|----------------|----------|
| **flat** | **64 KiB/s** (readln 128 KiB/s) | assign / overlay / rebind / splice / `managed_create` / `function_return` / listing / `clone_*` |
| **climb** | ~2× last 15 s (see `max_in_use_b_s` in `rss_hard_loops.py`) | `clone_nested_*`, `test_script_*` |
| **grow** | must grow ≥ 256 KiB/s | `array_append` |

60 s is the night / finish-pass window (`AFW_ISSUE2_DURATION_S=60`).

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
garbage (first or last sample 0). The lab then skips the `in_use`
slope for that run; RSS still gates. If RSS is flat and `in_use` is
huge or ~0, rerun that one workload.

Disaster RSS: **8 MiB/s**. Per-workload `in_use` ceilings are the leak gate
(`flat` 64 KiB/s; `climb` ~2× last 15 s). `array_append` must grow.

Assigned / unassigned pairs (same call, two leak classes):

| assigned (`slot_store` leftover RC) | unassigned (extra-hold / last-release) |
|-------------------------------------|----------------------------------------|
| `splice_assign` | `splice_unassigned` (last stmt `add(0, 0)`) |
| `managed_create_assign` | `managed_create_unassigned` (last stmt `add(0, 0)`) |
| `clone_assign` | `clone_unassigned` (last stmt `add(0, 0)`) |
| `clone_nested_assign` | `clone_nested_unassigned` (last stmt `add(0, 0)`) |
| `test_script_assign` | `test_script_unassigned` (last stmt `add(0, 0)`) |
| `compile_listing_assign` | `compile_listing_unassigned` (last stmt `add(0, 0)`) |

Unassigned loops whose last statement is a managed create would
`slot_store` that result into `xctx->script_result` on deactivate.
That is an isolate, not a missing extra-hold. Those loops end with a
scalar so the create is a pure temp. `unassigned_temps` is the other
class: unmanaged `add(1, 1)` as last statement, so the isolate is a
scalar on purpose.

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
| `compile_once_eval` | compile once, `evaluate` loop | **flat / flat** ([PR #439](https://github.com/afw-org/afw/pull/439), 2026-10-01 15 s: RSS 0 / `in_use` 0). Was ~50–65 MiB/s (`clone_managed` bump of already-managed isolate) | **flat / flat** |
| `array_push_pop` | push then pop | **flat / flat** | **flat / flat** |
| `splice_assign` | splice copy-out then assign | **flat / flat** (2026-09-29, 15 s after managed remove). Was **under bar** 2026-09-28 (~0.42 / ~0.21); leftover RC ~185 MiB/s before extra-hold-only | — |
| `splice_unassigned` | splice copy-out never assigned (last stmt `add()`) | **flat / flat** (2026-09-29, 15 s). Was ~2.58 / ~2.59 until managed `remove_value_by_index` last-released the source slot | — |
| `unassigned_temps` | unmanaged `add()` (last stmt isolates a scalar) | **flat / flat** (2026-09-28) | — |
| `readln_loop` | `readln` short+long lines | **under bar** (2026-09-28). 60 s: ~0.40 MiB/s RSS / ~0.04 MiB/s in_use | — |
| `managed_create_assign` | extra-hold create_managed then assign (reverse/slice/filter/map/sort/bag/intersection/split/union/keys/values/entries) | **under bar** (2026-09-28). 60 s ~0.16 MiB/s RSS / ~0.09 MiB/s in_use; 180 s slope fell to ~0.09 / ~0.05 | — |
| `managed_create_unassigned` | same calls, never assigned (last stmt `add()`) | **under bar**. 15 s 2026-09-29: ~0.30 MiB/s RSS / ~0.28 MiB/s in_use. 60 s 2026-09-28: ~0.17 / ~0.14 | — |
| `clone_assign` | `clone` array/object then assign | **flat / flat** (2026-10-01, 15 s: `in_use` 0; 60 s RSS wander ~0.21 MiB/s, `in_use` 0). Was **climb** ~0.76 MiB/s | leftover nested isolate + property entries |
| `clone_unassigned` | `clone` never assigned (last stmt `add()`) | **flat / flat** (2026-10-01, 15 s and 60 s: RSS 0 / `in_use` 0). Was **climb** ~0.37 MiB/s | ~3.3 MiB/s in_use with `afw_value_clone` unmanaged |
| `clone_nested_assign` | `v = clone(o).child` / `e = clone(o).arr` then mutate | **climb** (2026-10-01, 15 s): ~0.38 MiB/s RSS / ~0.28 MiB/s in_use | — |
| `clone_nested_unassigned` | `discard(clone(o).child)` never assigned (last stmt `add()`) | **climb** (2026-10-01, 15 s): ~0.28 MiB/s RSS / ~0.23 MiB/s in_use | — |
| `test_script_assign` | `test_script` clone extra-hold then assign | **under bar** (2026-09-29, 15 s): ~0.55 MiB/s RSS / ~0.56 MiB/s in_use | — |
| `test_script_unassigned` | `test_script` clone extra-hold (last stmt `add()`) | **under bar** (2026-09-29, 15 s): ~0.70 MiB/s RSS / ~0.71 MiB/s in_use | — |
| `compile_listing_assign` | compile listing assigned | **flat / flat** (2026-09-29, 15 s) | — |
| `compile_listing_unassigned` | compile listing last-releases unit (last stmt `add()`) | **flat / flat** (2026-09-29, 15 s) | — |
| `array_append` | unbounded `push` | **must grow** (~3 MiB/s both) | must grow (~2.8 MiB/s) |

`function_return` is **flat** (managed `closure_binding`;
0-param call does not isolate enclosing last). No `function_return_value`
wrapper. Pin is on the caller.

`try_catch`: `afw_os_backtrace` returns a managed hexBinary and is
called only when this xctx has **`response:error:backtrace`** on
(`xctx->flags`; action `_flags_` / `flag_set` override env
defaults). The `afw` command defaults `response:error` on, so this
soak still captures; caught ENDTRY / overwrite `afw_value_release`s
it. 15 s soak 2026-09-17: in_use **0 B/s**. Do not skip capture as a
paper-over.

## Server soaks (same day)

These are `afwfcgi` firehose leaves, not the hard-loop table. They **pass**
if requests succeed. They do **not** record `process::poolBytesInUse`.
Process size sampled from `/proc` on the `afwfcgi` pid while the leaf ran.

Remeasured **2026-09-17** on `develop` after [PR #354](https://github.com/afw-org/afw/pull/354) (1 s `/proc` VmRSS):

| leaf | what | result (2026-09-17) | was (2026-09-15) |
|------|------|---------------------|------------------|
| `issue-2/02-pool-eval-soak` | 20 s object / nested-eval / function-return-object | **PASS**. RSS **flat** 24576 kB | **PASS**. flat 25344 kB |
| `07-firehose-blast-style` | 30 s mixed cheap scripts | **PASS**. RSS 43.5→46.0 MiB then wander 46.0–46.8 (~0.08 MiB/s first-to-last; ~flat after ~10 s) | **PASS**. ~39–41 MB (~0.09 MiB/s wander) |
| `07b-firehose-catalog-pool` | 40 requests (hits `maxRequests` in ~0.3 s) | **PASS** (0.28 s). No RSS samples | **PASS**. Too short for a slope |

`07b` is not a long-running leak lab as written.

`array_push_pop`: `push` `slot_store`s; `pop`/`shift` transfer then
`afw_pool_release_value_at_cleanup` on the current scope (see
`afw_array_create_managed`). Do not `create_managed` in `array()`
(`[i]` compiles to it). Do not `get_assignable_for_lifetime` on the
pop result. `splice_assign` / `splice_unassigned` are the same extra-hold on the
removed array (not `get_assignable_for_scope_lifetime` after `create_managed`).
Managed `remove_value_by_index` last-releases the source slot hold; without
that, unlink left the occupant on `a` and `splice_unassigned` climbed
(~2.6 MiB/s). Both soaks are **flat** (2026-09-29). Assigned soaks are `slot_store` / leftover RC
after a bump; unassigned soaks are extra-hold / body last-release. Unassigned
loops whose result is managed end with `add(0, 0)` so deactivate does not
`slot_store` that result into `script_result`. `managed_create_assign` /
`managed_create_unassigned` cover the other create_managed extra-hold sites
(reverse, slice, filter, map, sort, bag, intersection, split, union, keys,
values, entries). `test_script_assign` / `test_script_unassigned` are the same
extra-hold on `create_managed_clone` of the result object.
`clone_assign` / `clone_unassigned` always-copy create_managed, extra-hold the
root only, take nested (not `afw_value_clone` into `x->p`; nested scalars
`get_assignable` of the source). Managed object last-release free_memorys
property entries and the name index. `clone_nested_assign` /
`clone_nested_unassigned` take a nested object/array occupant of a clone
(`clone(o).child`, `clone(o).arr`, `clone(o.child)`). Whole-container
`clone_*` is **flat**; this nested-occupant pair still **climbs**.
`compile_listing_assign` / `compile_listing_unassigned` last-release the unit
after the dump; do **not** extra-hold `compile()` of a unit (`evaluate(compile())`
/ closures still need that heap). `readln_loop`
needs a temp application conf (`rootFilePaths`); the
Python harness writes that for the spawn only.

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
