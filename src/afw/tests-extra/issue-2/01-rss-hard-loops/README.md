# issue-2 leak lab (opt-in)

Hard-loop Adaptive Scripts plus RSS sampling and gdb helpers. **Not** in
`afwdev test -j`. Use this while working optional `free` / same-size reuse
on [#2](https://github.com/afw-org/afw/issues/2).

The directory is `issue-2`, not `#2` — `#` starts a shell comment.

**Expected red on develop today.** Two different climbs (do not mix them):

1. **Braces** — `while (true) {}` is a Block. Every iteration
   `afw_xctx_scope_create` → heap tracker → `apr_pcalloc` on the eval
   heap’s APR pool. Destroy does not recycle that header. **RSS climbs;
   `pool_bytes_in_use()` stays near startup** (APR, not AFW malloc).
   `while (true);` is flat.
2. **Unbraced `i = i + 1`** — no extra scope. `slot_store` →
   `create_managed_integer` via `afw_xctx_malloc` (**general** `xctx->p`).
   General-pool free is a no-op, so **`pool_bytes_in_use()` and RSS
   both climb**. Eval-heap `bytes_allocated` stays ~0.

`empty_stmt` / `*_no_brace` are the split. The Python judge uses `/proc`
RSS plus gdb `env->pool_bytes_in_use`. Sample from script:
`pool_bytes_in_use()` and `process_rss()`. `debug:pool` names call sites
on a **short** run — not on soaks.

Campaign map: [`designs/issue-2-lifetime.md`](../../../designs/issue-2-lifetime.md)
(*Destroy is lifetime. Optional `free` is reuse.*).

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

| name | what | today (RSS / in_use) |
|------|------|----------------------|
| `empty_stmt` | `while (true);` | flat / flat (**pass**) |
| `empty_loop` | `while (true) {}` | RSS up / in_use flat (APR tracker) |
| `integer_assign_no_brace` | unbraced `i = i + 1` | both up (scalar boxing) |
| `integer_assign` | braced `i = i + 1` | both up (tracker + scalar) |
| `object_prop_assign_no_brace` | unbraced `o.x = i` | overlay set, no `{ }` |
| `object_prop_assign` | braced `o.x = i` | overlay + tracker |
| `array_index_assign_no_brace` | unbraced `a[0] = i` | element set, no `{ }` |
| `array_index_assign` | braced `a[0] = i` | element + tracker |
| `object_rebind` | `o = { n: i }` | unmanaged face |
| `array_rebind` | `a = [i]` | unmanaged face |
| `string_same_size` | `"x"` / `"y"` overwrite | same-size utf8 |
| `function_return` | `i = f()` | donate_return + slot |
| `try_catch` | throw/catch each iter | rewind |
| `closure_rebind` | rebind capturing function | closure/scope |
| `compile_once_eval` | compile once, `evaluate` loop | inner heap wrap |
| `array_push_pop` | push then pop | capacity once |
| `array_append` | unbounded `push` | **must grow** (harness) |

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

1. `afw-bt` — braced empty sits in while / block / tracker create
   (`apr_pcalloc`). Unbraced `i = i + 1` sits in
   `create_managed_integer` / `slot_store`.
2. `afw-heap` / `afw-rss` — three numbers: VmRSS,
   `evaluation_heap->bytes_allocated`, `env->pool_bytes_in_use`.
   Two interrupts 5s apart:
   - RSS up, in_use flat → APR (tracker headers, pages not returned).
   - RSS and in_use up, heap bytes ~0 → general `xctx->p` (managed
     scalars; free is a no-op).
   - heap `bytes_allocated` up with in_use → eval-heap malloc not
     given back (optional `free` / reuse).
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

After warmup, `integer_assign` / `object_prop_assign` / `array_index_assign`
RSS slope should fall near allocator noise (well under 8 MiB/s; ideally
flat). `array_append` should still grow. `object_rebind` may stay red
until unmanaged faces can optional-`free` at zero — that is a different
sentence on the same pad, not a failed scalar reuse.

Do not put these loops in the default gate. Correctness of assign/faces
already lives under `src/afw/tests/language/script/`.
