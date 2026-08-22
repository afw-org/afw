# issue-2 leak lab (opt-in)

Hard-loop Adaptive Scripts plus RSS sampling and gdb helpers. **Not** in
`afwdev test -j`. Use this while working optional `free` / same-size reuse
on [#2](https://github.com/afw-org/afw/issues/2).

The directory is `issue-2`, not `#2` — `#` starts a shell comment.

**Expected red on develop today.** Slot protocol and the eval heap wrap
already landed; a tight `i = i + 1` still climbs ~hundreds of MiB/s because
overwritten scalars are not recycled until the evaluate ends (it never
does). These cases should go green when reuse is real.

Campaign map: [`designs/issue-2-lifetime.md`](../../../designs/issue-2-lifetime.md)
(*Destroy is lifetime. Optional `free` is reuse.*).

## Run the RSS suite

```bash
afwdev test -T src/afw/tests-extra/issue-2 --show-all
```

Each workload is one `afw -s script` process, sampled from `/proc/<pid>/status`.
Default: 8s run, 2s warmup, 2s interval. Fail if a “should stay flat” loop
grows faster than 8 MiB/s after warmup. `array_append` is the opposite
control: it **must** grow, so a broken sampler cannot silently pass.

```bash
# one or a few workloads, longer window
AFW_ISSUE2_WORKLOAD=integer_assign,object_prop_assign \
  AFW_ISSUE2_DURATION_S=15 AFW_ISSUE2_INTERVAL_S=5 \
  afwdev test -T src/afw/tests-extra/issue-2 --show-all

# numbers only (do not fail on slope)
AFW_ISSUE2_RSS_ASSERT=0 afwdev test -T src/afw/tests-extra/issue-2 --show-all
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
src/afw/tests-extra/issue-2/_tools/sample-rss.sh integer_assign 20
# or
python3 src/afw/tests-extra/issue-2/_rss.py --list
python3 src/afw/tests-extra/issue-2/_rss.py integer_assign --duration 20 --interval 5
```

## Workloads (`_workloads/`)

Underscore dir on purpose: `afwdev test` must not evaluate these as tests
(they do not return).

| name | what | #2 slice |
|------|------|----------|
| `integer_assign` | `i = i + 1` | scalar slot overwrite |
| `object_prop_assign` | `o.x = i` | overlay `set` (S4) |
| `array_index_assign` | `a[0] = i` | array element overwrite |
| `object_rebind` | `o = { n: i }` | unmanaged face; zero does not destroy |
| `array_rebind` | `a = [i]` | same, array face |
| `string_same_size` | `"x"` / `"y"` overwrite | same-size utf8 reuse |
| `function_return` | `i = f()` temp integer | donate_return + slot_store |
| `try_catch` | throw/catch each iter | rewind; error object |
| `closure_rebind` | rebind a capturing function | closure/scope hold |
| `compile_once_eval` | compile once, `evaluate` loop | inner heap wrap |
| `array_push_pop` | push then pop | capacity once, then reuse |
| `empty_loop` | `while (true) {}` | while/block overhead — on develop this leaked *faster* than `integer_assign` (the loop itself allocates) |
| `array_append` | unbounded `push` | **must grow** (harness control) |

## gdb

Installed `afw` / `libafw` need debug info (the usual `--cdev` build).

```bash
# start under gdb (does not `run` until you type it)
src/afw/tests-extra/issue-2/_tools/gdb-run.sh integer_assign
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
afw -s script src/afw/tests-extra/issue-2/_workloads/integer_assign.as &
src/afw/tests-extra/issue-2/_tools/gdb-attach.sh $!
# or newest afw:
src/afw/tests-extra/issue-2/_tools/gdb-attach.sh
```

Useful hunts after Ctrl-C:

1. `afw-bt` — should sit in integer add / slot store / pool malloc for the
   scalar loop, overlay `set` for `o.x = i`.
2. `afw-heap` — `evaluation_heap->bytes_allocated` (type
   `afw_pool_internal_self_t`). If that number tracks RSS, the leak is the
   eval heap, not APR `xctx->p`.
3. Two interrupts 5s apart — if `bytes_allocated` only goes up, optional
   `free` is not returning blocks.
4. `afw-breaks` then `continue` on a **slow** script only. A hard loop
   will crawl if you break every `slot_store`.

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
