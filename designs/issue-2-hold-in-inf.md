# Issue #2 — hold in the inf (revised 2026-08-24)

**Audience:** maintainers / assistants. **Not** handbook.

**GitHub:** [#2](https://github.com/afw-org/afw/issues/2).  
**How close (scoreboard):** [Live status](#live-status-2026-09-25--how-close-is-2) below. **What to do next (lifetime story):** [implementation order](#lifetime-story--implementation-order). If this pad’s older “Order / Parked” lists, the RSS lab README, the two-worlds pad, or the GitHub issue body disagree with that section, **the live status wins** until someone updates it.

**On `develop`:** pool two-impls ([PR #267](https://github.com/afw-org/afw/pull/267)). Two worlds (unmanaged dest `p` / managed `p->managed_p`) **[#277](https://github.com/afw-org/afw/issues/277) closed** (PR **#278**) — pad [`experiment-brainstorm.md`](experiment-brainstorm.md). `issue-2-managed-p` is gone. Dest `p` on `get_assignable` / `slot_store` / `create_managed` / evaluate pin: [PR #355](https://github.com/afw-org/afw/pull/355).

**Lifetime story:** [`lifetime-principles.md`](lifetime-principles.md). This pad is the rails for inf methods. **Two worlds:** [`experiment-brainstorm.md`](experiment-brainstorm.md). **Eval `p`:** [`experiment-eval-p.md`](experiment-eval-p.md). Pool doors: [`remaining-apr.md`](remaining-apr.md). History: [`issue-2-lifetime.md`](issue-2-lifetime.md). If a leak tempts a helper *around* assign — **stop and ask**.

### Lifetime story — implementation order

Say **let’s talk about the next *n*** (or the issue id). Story: [`lifetime-principles.md`](lifetime-principles.md). Lab: `src/afw/tests-extra/issue-2/01-rss-hard-loops/`. C sittings will churn leftover until last RC completes the walk. Do not extra-hold / register last-release on a method that returns a held value.

| n | Status | What |
|---|--------|------|
| 0 | landed | Story pad on `develop` ([PR #447](https://github.com/afw-org/afw/pull/447)). |
| 1 | landed | [#443](https://github.com/afw-org/afw/issues/443) / [PR #448](https://github.com/afw-org/afw/pull/448). Every former `afw_xctx_*alloc`/`free` site has dest `p` via `afw_pool_*`. Macros deleted. Managed wrappers RC 1. |
| 2 | landed | [#445](https://github.com/afw-org/afw/issues/445) / [PR #452](https://github.com/afw-org/afw/pull/452) `f5927219`. Last RC of managed containers; drop `is_root`. |
| 3 | landed | [#446](https://github.com/afw-org/afw/issues/446) / [PR #456](https://github.com/afw-org/afw/pull/456) `03046439`. Evaluate dest `p` honor. Dest cache `xctx->script_result_p` was later dropped: `script_result_set` takes dest `p` (`scope->p` at deactivate). Nested `evaluate(compile())` parks `script_result`. |
| 4 | landed | [PR #457](https://github.com/afw-org/afw/pull/457) `58e1c952`. 60s RSS finish-pass. Climb class empty. `AFW_ISSUE2_JOBS` parallel soaks; gdb miss judge. |
| 5 | landed | [PR #460](https://github.com/afw-org/afw/pull/460) `3fdaabdd`. Dest-p honor. Helpers take dest `p`. After-evaluate `get_assignable_for_p_lifetime`. Script-function with no Adaptive caller isolates dest evaluate dest `p`. `pop`/`shift` take dest `p`. `afw_compile_*` is caller does not release: result lasts for dest `p`; managed `compiled_value` last-releases that dest `p`. Adaptive `compile` / `eval*` / `test_script` / `test_template` / `compile_from_file` pass `x->p`. Listing does not extra-`release` the unit. `afw_function_eval_reference_escaped_unit` `get_reference`s an escaped function onto the unit (dest `p` owns the birth hold). `test_script` error `create_managed` last-releases dest `p`. Host evaluate dest `xctx->p` is chosen dest. `eval_closure_rebind` **flat**. `eval_object_rebind` **climb** (nested leftover; follow-up [#458](https://github.com/afw-org/afw/issues/458)). 60s RSS `AFW_ISSUE2_JOBS=16` (~184s wall, 2026-10-02): 35 passed; `eval_object_rebind` ~8.5 MiB/s RSS / 3.05 MiB/s in_use. Leave: clone nested scalar, `compiler_internal` assignment aggregate, runtime registry clone, `array_from_values` occupant isolate, compile-literal `clone_managed`. Function `@return` sweep later. #2 stays open. |

[#379](https://github.com/afw-org/afw/issues/379) (server soak) and [#404](https://github.com/afw-org/afw/issues/404) (overnight) stay complementary, not this sequence.

---

## Live status (2026-09-25) — how close is #2?

**Still open.** Do not close. The **protocol is the system** on `develop`. The **campaign is not complete**. This section is the scoreboard for “how close”; it is not a sitting plan and not a rewrite of the rails below.

**Complete when** all of these are true (then remaining items are follow-up issues or explicit will-not-do):

1. Protocol matches the tree (maps and the GitHub body stop teaching dropped plans).
2. Script scope lifetimes are correct for the doors we already named: assign, overlay, rebind, `return`, closures, throw rewind, `last_statement_non_void_value`. Product tests green; RSS soaks **flat or under an agreed bar**.
3. A long-lived `afwfcgi` does not silently climb on firehose / nested-eval soaks (process/server pool stats — request-end valgrind hides leftovers).
4. Parked polish is split off or will-not-do. Allocator tuning is not a close gate.

“One request can loop forever without climbing” is the close bar we have been using. **Values that survive a request** (reused compile units) is a separate remainder — child issue or will-not-do, not silent “#2 done.” The adapter cache is request-scoped.

### Landed on `develop` (do not re-litigate)

Slot protocol; pool two-impls ([PR #267](https://github.com/afw-org/afw/pull/267)); two worlds **#277** closed (PR **#278**); names as values (PR **#220**); closures / throw-path rewind (**#35** closed); compile-literal + intern **#280**; `script_result` ([#62](https://github.com/afw-org/afw/issues/62) closed); `get_reference` takes `xctx` only (no dest `p` on the bump); scope RC 1 + `parent_scope_block` / `scope_depth`; eval `p` = `scope->p` when `{ }` has a frame ([PR #287](https://github.com/afw-org/afw/pull/287)); `compile()` is a unit ([PR #305](https://github.com/afw-org/afw/pull/305)); **every `{ }` is a scope** + `last_statement_non_void_value` on the running frame ([PR #306](https://github.com/afw-org/afw/pull/306)); isolate last at clone + wrap unbraced loop bodies ([PR #307](https://github.com/afw-org/afw/pull/307)); builtin lifetime ([PR #308](https://github.com/afw-org/afw/pull/308)); managed `pop`/`shift` temp on current scope ([PR #309](https://github.com/afw-org/afw/pull/309)); script return pins on the caller — **no** `function_return_value` wrapper ([PR #326](https://github.com/afw-org/afw/pull/326)); APR gone from libafw, one ST heap per xctx, managed allocs in `p->managed_p` ([PR #327](https://github.com/afw-org/afw/pull/327)). **[PR #355](https://github.com/afw-org/afw/pull/355):** script/template/test_script compile returns a **managed** `compiled_value`; `create_managed` / `clone_managed` / `get_assignable` / `slot_store` / `closure_binding_create` take dest `p` (`p->managed_p`); evaluation `{ }` is `scope_create` (ST heap, 4k, inherit `managed_p`, throw last-release delay); evaluate of a compiled value **pins** `script_result` on dest `p` (no `clone_unmanaged`).

Hard-loop soaks remeasured **2026-09-16**; isolate sitting landed on `develop` as [PR #340](https://github.com/afw-org/afw/pull/340) (`d9cb1583`). Assign / overlay / rebind / empty `{ }` / unbraced loop body / `array_push_pop` / **`function_return`** / `let`/`const` empty `f()` **flat**. Nested `{ }` that wrote `script_result` **clears** parent last instead of planting the occupant. Try returns current last; `keep_return` extra-holds a real `return`. `try_catch` in_use **flat** (2026-09-17): managed hexBinary backtrace released on overwrite / caught ENDTRY / xctx teardown ([#341](https://github.com/afw-org/afw/issues/341) / [PR #354](https://github.com/afw-org/afw/pull/354)). Table: [`src/afw/tests-extra/issue-2/01-rss-hard-loops/README.md`](../src/afw/tests-extra/issue-2/01-rss-hard-loops/README.md). Gate 2026-09-17: `./afwdev build --fulldev` + `afwdev test -j` + valgrind **4516 passed**, 71 skipped. Later gate, PR #377: same commands, **4545 passed**, 71 skipped. PR #378 (`afw` leaves `response:error` off) was `./afwdev build --cdev` + `afwdev test -j`, **4545 passed**, 71 skipped, not a valgrind run.

**Since the 2026-09-19 scoreboard (not #2 close work):**

- [#342](https://github.com/afw-org/afw/issues/342): a function or closure from `eval<script>` keeps that compile unit on the closure binding. The binding's last release drops the scope, then the unit. `evaluate(compile<script>(...))` does not last-release its unit. Data results stay pinned on dest `p`. Managed alloc stays dest `p->managed_p`.
- [#363](https://github.com/afw-org/afw/issues/363) is **closed**. Profile hot spots are not a close gate. The job-heap free list skips the walk when the largest free block is too small ([PR #375](https://github.com/afw-org/afw/pull/375)). Free-list size buckets and the dual-ended carve stay off. Heap chunks come from the thread `afw_memory_region` ([#358](https://github.com/afw-org/afw/issues/358), closed). Pool doors stay [`remaining-apr.md`](remaining-apr.md).
- [PR #378](https://github.com/afw-org/afw/pull/378): the `afw` command leaves `response:error` off. A test script no longer keeps an evaluation listing on each caught error unless `--conf` (`defaultFlags`) or `flag_set` turns the flag on. The terminal printer still shows the error type, the message, and the short contextual lines. The 2026-09-17 `try_catch` soak was measured while the command still defaulted the flag on. `ks` is unchanged.
- GitHub [#2](https://github.com/afw-org/afw/issues/2) is the single umbrella (rewritten 2026-09-30 / 2026-10-01 to match the lab). Living leak list: RSS lab classes (`flat` / `climb` / `grow`). Children that last more than one sitting: [#379](https://github.com/afw-org/afw/issues/379) (server soak), overnight [#404](https://github.com/afw-org/afw/issues/404). [#342](https://github.com/afw-org/afw/issues/342), [#380](https://github.com/afw-org/afw/issues/380), [#381](https://github.com/afw-org/afw/issues/381) are **closed**. Do not open a child per leftover RC.
- Occupant mint from C internals ([PR #437](https://github.com/afw-org/afw/pull/437), `42321273` on `develop`): `create_managed` then `set_property_take` / `push_value_take` (`afw_value_slot_take`). Slot takes the birth hold. Object and array infs have `is_managed`. Lab per-workload `in_use` ceilings. `managed_create` pair **flat**.
- `compile_once_eval` ([PR #439](https://github.com/afw-org/afw/pull/439)): `evaluate` of a `compiled_value` in a loop. `clone_managed` of an already-managed `script_result` isolate was leftover RC (~50–65 MiB/s). Copy unit-backed literals only; already-managed keeps the store reference, then pin dest `p`. 15 s 2026-10-01: RSS 0, `in_use` 0. Full lab suite passed.
- Adaptive `clone()` leftover RC: register last-release of the **execute result** only after the copy; take nested (parent holds one reference). Do not `afw_value_clone` unmanaged into `x->p`. `is_root` is gone. Managed object last-release `release`s held values, `free_memory`s property entries, and `free_memory`s `copy_meta` utf8 (`id` / uri / object_type_uri). Nested literals are embedded with `meta.id`. `test_script` / `test_template` `create_managed` the result from the start (`unmanaged_new_p` left a child of `managed_p`). `afw_error_to_object` is unmanaged in dest `p`; a managed parent takes a `create_managed` error object. Object pattern rest is dest `p` unmanaged (array rest already was). Core execute leftover scan: auth wrapper instance-releases in `FINALLY`; adapter journal stays for the sweep; `index_create` CATCH is dead (`value` evaluated later). 15 s 2026-10-01: whole-container and nested-occupant `clone_*` **flat**; `test_script_*` **flat**; unused `test_script(..., "error")` **flat**; `object_rest_*` **flat**. [#445](https://github.com/afw-org/afw/issues/445).
- [#446](https://github.com/afw-org/afw/issues/446) closed ([PR #456](https://github.com/afw-org/afw/pull/456)). 60s RSS finish-pass **2026-10-02** (n=4, 33 workloads): climb class empty. Sequential 60s: `object_rest_unassigned` once failed `in_use` 64 KiB/s on a gdb warmup sample 262144 then the usual ~4 MiB floor; RSS 0 for 60 s. Re-run 60s `in_use` 3967264→3968448, 0.00 MiB/s. `AFW_ISSUE2_JOBS=16` 60s (~184s wall): 33 passed; parallel gdb can miss (`in_use` 0 or a pointer-sized integer) and the judge ignores those. Judge also ignores a warmup `in_use` far below last when RSS is not climbing.
- n=5 dest-p honor landed ([PR #460](https://github.com/afw-org/afw/pull/460) `3fdaabdd`). 60s RSS `AFW_ISSUE2_JOBS=16` **2026-10-02** (~184s wall, 35 workloads): 35 passed. `eval_closure_rebind` **flat**. `eval_object_rebind` **climb** ~8.5 MiB/s RSS / 3.05 MiB/s in_use ([#458](https://github.com/afw-org/afw/issues/458)). `empty_stmt` parallel gdb `in_use` n/a; RSS 0. Host evaluate dest `xctx->p` is chosen dest.
- Lifetime story: [`lifetime-principles.md`](lifetime-principles.md). Tree vs that story: [#443](https://github.com/afw-org/afw/issues/443) closed ([PR #448](https://github.com/afw-org/afw/pull/448)), [#445](https://github.com/afw-org/afw/issues/445) closed ([PR #452](https://github.com/afw-org/afw/pull/452)), [#446](https://github.com/afw-org/afw/issues/446) closed ([PR #456](https://github.com/afw-org/afw/pull/456)), n=5 landed ([PR #460](https://github.com/afw-org/afw/pull/460) `3fdaabdd`).

Complementary, not this close bar: request caps / `process::` telemetry ([#329](https://github.com/afw-org/afw/issues/329) / [PR #330](https://github.com/afw-org/afw/pull/330)); retrieve `maxObjects` ([#49](https://github.com/afw-org/afw/issues/49)); progressive release ([#127](https://github.com/afw-org/afw/issues/127)); qualifier snapshots ([#9](https://github.com/afw-org/afw/issues/9)).

### Still inside #2 (decide or do before close)

| Item | Why it is still #2 |
|------|---------------------|
| **Evidence** | [#379](https://github.com/afw-org/afw/issues/379). **2026-09-17** hard-loop table including `try_catch` **flat**. Isolate sitting [PR #340](https://github.com/afw-org/afw/pull/340). **2026-09-25** short remeasure on current `develop` (response:error off): `02` RSS locked at 22.00 MiB, pool floor 3.66 MiB; `07` RSS band about 22.5–24.5 MiB, pool floor 3.66 MiB; `07b` for 30 s with `maxRequests` removed, RSS locked at 22.25 MiB. A 5-minute high-rate run (about 2,400 and 4,800 requests/s) stayed flat: `07` RSS 27.7–30.4 MiB, pool lows near 5 MiB; `07b` RSS 22.50 MiB, pool 3.95–4.19 MiB. The old 43–46 MiB `07` reading is stale. The harness for a longer watch is [PR #383](https://github.com/afw-org/afw/pull/383): `tests-extra/firehose` (load, until stopped) and `tests-extra/manual` (no load, heartbeat). Overnight wall-clock is still not done. [#382](https://github.com/afw-org/afw/issues/382) is a crash, not this slope. |
| **Watch (leak, not crash)** | [#380](https://github.com/afw-org/afw/issues/380) children done (**closed**). Lab classes: **flat** 64 KiB/s `in_use` (splice, `managed_create`, `function_return`, assign/rebind, listing, `compile_once_eval`, `clone_*`, `test_script_*`, `object_rest_*`, `eval_closure_rebind`); **climb** leftover RC `eval_object_rebind` ([#458](https://github.com/afw-org/afw/issues/458)); **grow** `array_append`. `compile_once_eval` is **flat** ([PR #439](https://github.com/afw-org/afw/pull/439)). Adaptive `clone()` registers last-release of the execute result, takes nested, and last RC `free_memory`s cloned `meta.id`. `test_script` `create_managed`s the result ([#445](https://github.com/afw-org/afw/issues/445)). Object pattern rest is dest `p` unmanaged. Unassigned unmanaged values **flat** (will-not-do). `readln` **under the bar** (will-not-do). |
| **Escape past one xctx** | [#381](https://github.com/afw-org/afw/issues/381). Runtime objects: [PR #386](https://github.com/afw-org/afw/pull/386) snapshots shared counters and properties into the caller pool (pin under `adapter_id_anchor_lock`, copy outside it, release after). `metrics.additional` is the short pin: held across `get_additional_metrics` only, released in `AFW_FINALLY`; the object is built in `p` and must not refer to the adapter after return. Clone-into-the-requestor-pool **under** the lock is **will-not-do**. Adapter cache is will-not-do: the cache lives in `xctx->p` and dies with the request xctx. Hosts call `afw_adapter_session_commit_and_release_cache` on the normal path and the handled-error path. `afw_adapter_session_commit_and_release_cache` finishes every transaction and session release, clears `xctx->cache`, then rethrows the first error ([PR #398](https://github.com/afw-org/afw/pull/398)). Leftover `afw_adapter_get_reference` references are released when the xctx pool is destroyed ([PR #401](https://github.com/afw-org/afw/pull/401)). Compile units: script, template, and test_script compile to a managed immutable `compiled_value`; evaluate does not free the unit and pins the result on the caller pool. Model `on*` scripts and conf / `app::` templates are compiled once into a longer-lived pool and re-evaluated later; that reuse is intended and stays. Index filter and value scripts recompile each use into `object->p`. The model-location compile cache (`afw_model_location_get_model`, compile on miss into the location adapter pool, dropped by restarting that adapter) is intended and outside this door. An eval<script> closure that keeps its unit is [#342](https://github.com/afw-org/afw/issues/342) ([PR #394](https://github.com/afw-org/afw/pull/394)). No path on `develop` compiles a fresh unit into a process or adapter pool for a later request by accident; unintentional compile-unit survival across requests is **will-not-do**. [#381](https://github.com/afw-org/afw/issues/381) **closed**. |

### Parked — not close-blockers unless we say so

C `clone_unmanaged` / `clone_managed` / dropping `clone()` and the `clone_or_reference` macro is [#424](https://github.com/afw-org/afw/issues/424). Adapter clones / clone-of-unmanaged meta. `qualifier("current")` snapshot tail. These parked items are still listed here. They are not child issues yet, so close criterion 4 is not met. mmap and a per-size free list inside the heap stay parked. The thread `afw_memory_region` chunk free list and the job-heap largest-block skip ([PR #375](https://github.com/afw-org/afw/pull/375)) already landed. Size buckets stay off. Heap mixed-size rewrite **withdrawn** (14k concat nest ~0.05s after #287). Do not add `get_base`. Do not mint a mutable face over `environment::`, `process::`, `request::`, `application::`, or adapter/custom conf objects. Overlay `o.x = i` / `o = { n: i }` are **not** parked leak verticals — those soaks were flat.

### Do not start (dropped or parked)

- Unique consume, eval-stack leftover, `#function_return_value`, or a call-result leftover inf.
- Hop dest `p` / wrap at execute / helpers around assign.
- Heap size-class rewrite for the 14k nest.
- **`designs/maps/` harvest** — decided, not started. Topic-named current kit later; do not copy live maps in the meantime.

### Where truth lives

| Kind | Open |
|------|------|
| **This scoreboard** | This section |
| **Live maps** | **Story** [`lifetime-principles.md`](lifetime-principles.md); this pad (rails); [`experiment-brainstorm.md`](experiment-brainstorm.md); [`experiment-eval-p.md`](experiment-eval-p.md); [`remaining-apr.md`](remaining-apr.md) |
| **Lab** | [`src/afw/tests-extra/issue-2/`](../src/afw/tests-extra/issue-2/) — table in `01-rss-hard-loops/README.md` (pairs 2026-09-29) |
| **History — do not copy wording** | [`issue-2-lifetime.md`](issue-2-lifetime.md), [`issue-2-hold-in-inf-plan.md`](issue-2-hold-in-inf-plan.md), [`memory-management.md`](memory-management.md) |
| **Code / tests** | Ground truth. When they disagree with a pad, fix the pad. |

Keep from `develop`: slot protocol, pool split, two worlds **#277**, `#35` store-time bind, `#245` then **every `{ }` is a scope** ([PR #306](https://github.com/afw-org/afw/pull/306)), `#246`/`#247` honest heap/tracker.

**Landed on `develop`:** compile facts `parent_scope_block` / `scope_depth` after the unit is complete (`finalize_scope_tree`). Every `{ }` is a scope ([PR #306](https://github.com/afw-org/afw/pull/306)); flattening a useless block is compile-side only. Scope create/clone start at RC 1; creator `release`s; `activate` / `deactivate` only push/pop. Bind script functions via `enclosing_block`, not syntax depth. `get_assignable_value` table (managed bump; unmanaged scalar promote; script `{}` / `[]` `clone_managed`; runtime/adapter object managed wrapper; permanent scalar as-is; permanent object/array wrapper/clone). Dest `p` ripped from `slot_store` / `as_assignable` / `get_assignable_value` / `get_reference` / `add_reference`. `get_reference` is a bump (`instance, xctx`) — leftover dest `p` was an abandoned plan to have it do `clone_or_reference`. `last_statement_non_void_value` on each scope; deactivate `script_result_set`s unless cloned ([PR #306](https://github.com/afw-org/afw/pull/306)). **[PR #340](https://github.com/afw-org/afw/pull/340):** drop extra-hold last adopts; nested `{ }` that wrote `script_result` clears parent last; try `keep_return` extra-holds a real `return`. Clone isolates `script_result_set(original last)` then clone last stays void; loop `{ }` bodies do not extra-hold onto the clone; unbraced loop bodies wrap after parse in the current block ([PR #307](https://github.com/afw-org/afw/pull/307)). Built-in lifetime: `get_assignable_for_lifetime` / `set_last_statement_non_void_value_for_lifetime`; hold instance first then mutate; new array results `create_managed` then fill; `array()` stays a script wrapper ([PR #308](https://github.com/afw-org/afw/pull/308)). Nested eval save/restore `script_result`. Script return is `get_assignable_for_p_lifetime` on the caller — no `function_return_value` wrapper ([PR #326](https://github.com/afw-org/afw/pull/326)). Eval `p` = `scope->p` when `{ }` has a frame: [PR #287](https://github.com/afw-org/afw/pull/287) / [`experiment-eval-p.md`](experiment-eval-p.md). Compile-literal inf + interned parse-word strings **#280**. One ST heap per xctx; evaluation `{ }` is `afw_pool_scope_create` of that heap. Managed allocs use `p->managed_p` ([`remaining-apr.md`](remaining-apr.md)).

---

## Locked design

Value lifetime story: [`lifetime-principles.md`](lifetime-principles.md) wins if this section disagrees. In this pad, **extra-hold** means register last-release of a managed hold on dest `p` when the contract is caller does not release.

Add a value method only when we want to do **one thing** and the code is `if` this type **then** A **else** B. Callers name the thing; each inf implements A or B. If we cannot name the thing in one phrase, split the **use**, do not add a kitchen-sink method. (`metas()` views may bite later; fix on that inf, not a new method.)

**Value infs (and optional `compiler_internal` calls) are the compiler’s runtime.** Create = pick the instruction. `optional_evaluate` / `execute_*` = run it. Graph nodes are declarative IR. A second compiler may mint the same infs and call `afw_value_evaluate()`. Prefer a new inf when it is a *kind of value* that will sit in a slot.

Callers:

- **Read** a slot → the pointer.
- **Keep alive** → `get_reference` (matching `release`).
- **Fill a slot** (assign, param, overlay set, `return` / call result) → `get_assignable_value` (matching `release`).
- **Operators / `+`** → pointer. Do not wrap at execute.
- **Call / `return`:** isolate **before** the callee frame dies (`script_result_set` at deactivate, dest `scope->p`). Everyday `evaluate()` is caller does not release. Script return is `get_assignable_for_p_lifetime` on the caller; no leftover wrapper inf.

**Script return (no FRV wrapper):** `return()` is `set_last_statement_non_void_value` (pointer). Pin is `get_assignable_for_p_lifetime` on the **caller** while the callee frame is alive. Do **not** reopen unique consume, eval-stack leftover, or `#function_return_value`. Parameter window: `evaluate_for_parameter` (raw eval) then park the **occupant** in the parameter-number slot (`pop_parameter_number(VALUE)` replaces `#`). `pop_value` / rewind `release` parked occupants. One live marker pair per function while that parameter is being evaluated; after pop, `[call]` + 0 or more returns to top. Do **not** extra-push while a marker is on top. Do **not** hop `compiled_value` on assign to `unevaluated`. `meta()` does not evaluate argv first (property `key`). Script `meta()` snapshots are immutable. Array look-through: not a goal. Model mapped modify tuples: hold. Map: [`compile-unit-and-frv-next.md`](compile-unit-and-frv-next.md).

Tests: `language/script/return_temps.as`. `function_return` soak is **flat** on `develop` (2026-09-16); `array_push_pop` is **flat** (temp on current scope).
- **Retrieve / Adaptive `clone()`:** return the entity or structural clone. Slot fill wraps. Reconcile diffs the **face** (overlay sets); do not peel `wrapper_base`. No skip-hold. No `get_base` method until more than one product site type-switches for “entity.”
- **Script door ≠ C memory object.** Adapters/conf may leave a memory object writable. Script retrieve gets a working copy (slot fill / view with no setter). **Catalog qualifiers** (`environment::`, `process::`, `request::`, `application::`, `adapter::` conf, `custom::` conf) stay **read-only in script** — SET should throw, not mint a face, even if C never called `set_immutable`. `qualifier()` / `qualifiers()` are **copies** (#9); mutating a copy is fine. `current::` is a **name set per context**, not one object: `current::object` in a model hook is a working object (retrieve-like); in auth/index it is the resource/row (read). Live catalog writes, if ever, are a **function** with execute access and authorization that actually changes the host. Do not stamp catalog objects unmanaged “for consistency” with views.

Default impl: `#define impl_afw_value_get_assignable_value impl_afw_value_get_reference` before `afw_value_impl_declares.h`. Graph infs NULL (evaluate first).

| Inf | `get_reference` | `get_assignable_value` |
|-----|-----------------|------------------------|
| Managed scalar | bump | bump |
| Permanent scalar / compile literal | self | self |
| Unmanaged scalar | **throw** | managed copy in `p->managed_p` |
| Unmanaged object & array | **throw** | already managed dual-face bump; generic `"memory"` not a wrapper → `clone_managed`; else managed look-through wrapper |
| Permanent object & array | self | managed wrapper (object) or clone (array) |
| Assignable object & array face | bump | bump |
| `script_function` | self | `closure_binding` |
| `closure_binding` | bump | `get_reference(self)` |
| `metas` / const view | self | self |
| Graph (`call`, `block`, `symbol_reference`, `object_expression`, …) | NULL | NULL |

No new **object/array literal** inf (compile-literal infs for scalar integer/double/string **#280** are a different thing). Constant `{a:1}` / `[1,2]` stay fully evaluated unmanaged (or permanent) objects/arrays. Isolation is assignable on that inf. Do not compile constants as `object_expression`. Do not promote them to permanent as the isolation fix.

`object_expression` / `object_construct` stay: evaluate mints an **assignable** face. Spread / mixed arrays often a `call` of `array(...)` / `add_properties(...)`.

`create_wrapper_*` / `create_script_wrapper` are the **create** of the assignable inf, not a wrap protocol.

`property_get` / `variable_get` default is the evaluated occupant (**identity**). No `isolate_mutable_default` in the model. Inline `{ }` is isolated because it is still a raw unmanaged object when assignable runs.

`slot_store` = `get_assignable_value(incoming)` **then** `release` occupant **then** store. Same-pointer skip stays. Clone/hold first so incoming that still points at the occupant’s bytes (`s = s + s`, substring onto self) is not memcpy’d into a reused pool block (issue #275, valgrind `Overlap`). `let y = x` shares the assignable face.

**Place (LHS) `reference_by_key`:** `get_assignable_value(evaluate(aggregate))`, set, `release`. `{}.x = 1` mints a throwaway assignable face, set, `release` tears it down.

Face GET: store `get_assignable_value` of the retrieved child locally. Face SET: same pair into overlay. No write-through. Generic objects **do not** own properties.

`freeze()`: `get_assignable_value` then freeze **that** handle. Adaptive `clone()`: structural copy, then `get_assignable_value` (no path exception).

`get_reference` is a bump (`instance, xctx`). `clone_or_reference` is a compatibility name for that bump. Slot fill is `get_assignable_value(incoming, p, xctx)` / `slot_store(..., p, xctx)` — dest `p`, promote/clone uses `p->managed_p`.

---

## Frame, last_return, tracker (locked 2026-08-27)

This is the path. Not `assignable_p` on create, not hopping dest `p` inside `as_assignable`, not “keep objects off the tracker so last-release is safe.” Those got tests green and flattened `i = i + 1` by **not** putting the trip on the tracker. That work was thrown away. Do not put it back.

**Evaluate `p`:** `scope->p` when the `{ }` has a frame ([PR #287](https://github.com/afw-org/afw/pull/287) / [`experiment-eval-p.md`](experiment-eval-p.md)). Nested `evaluate` in a frame gets the same `p`. Temps (`1+1`, `{}`, `[]`) land there and die with last-release.

**Frame vs block.** Frame deactivate is the pool story. Throw/break/rewind deactivates frames and does not invent a result. **Every remaining `{ }` is a frame** ([PR #306](https://github.com/afw-org/afw/pull/306)). Compile omits empty `{ }` (no names, no statements). `{ stmt }` stays a frame. `for (let …)` clones extra **names** frames (siblings), not a result stack.

**`last_statement_non_void_value`** ([PR #306](https://github.com/afw-org/afw/pull/306) / [PR #307](https://github.com/afw-org/afw/pull/307)). Pointer on the running frame at the last non-void statement. Valid while `scope->p` is alive. Not a managed slot like `frame_slots[]`. Starts void. `return()` is `set_last_statement_non_void_value` (pointer); pin is `get_assignable_for_p_lifetime` on the caller. `let`/`const` do not write it. Nested assignment **does**. `for`/`while`/`try` are C-void except `return`/`rethrow`. Declared `: void` does not write.

**`script_result`** ([#62](https://github.com/afw-org/afw/issues/62)). Managed isolate of the running script result. Does not require the current scope. Deactivate `script_result_set`s `last_statement_non_void_value` unless the scope was **cloned**. Nested `{ }` that wrote `script_result` is a normal statement result so parent last is a **pointer** at that occupant (a prior parent last must not stomp it at deactivate). Nested `compiled_value` / script call / block-as-value park and restore `script_result`. Nested `evaluate(compile())` parks `script_result`. Isolate dest is dest `p` of the write (`scope->p` at deactivate, `original_scope->p` at clone). Loop `{ }` bodies `evaluate_block`; if `script_result` changed, pointer last on the parent. A normal `finally` does not replace a pending try/catch return. **undefined is a value** and does replace. Tests: `script_result.as`, `for-let-break-keeps-previous-last`, `loop_unbraced_body.as`. `{ let x = 1; { add(1,1) } }` last is `2`; `{ add(1,1); let x = 1 }` stays `2`.

**`for (let)` clone:** first trip is the for-let `{ }`. Next trip sibling-clones (copy slots). While the original `p` is still alive, `script_result_set_value(original last, original_scope->p)` (void is a no-op); clone last stays void; original is marked cloned so deactivate does not write the slot again. Increment runs on the clone so a closure still sees the old `i`. Creator-`release` the previous; it dies unless a closure holds it. Without closures: wrapper until `for` ends + current clone. The never-cloned frame is the last iteration. Unbraced loop bodies wrap as a 0-symbol `{ }` after parse in the current block.

**`get_reference` pairs with `release` where they exist.** Managed / assignable / wrappers: if you `get_reference`, you owe a `release`. Unmanaged object/array **value** infs **throw** on those methods ([#277](https://github.com/afw-org/afw/issues/277)); isolate with `get_assignable_value`. **Instance** `get_reference` / `release` still pin `object->p`. Unmanaged instances still **die with their pool** if nobody `get_reference`d them.

**Script sees values, not C instances.** Value protocol is `get_reference` / `get_assignable_value` / `release`. `get_assignable_value` always returns a value **that method’s caller must release** (managed scalar in dest `p->managed_p`, or a face that references the instance). Story: [`lifetime-principles.md`](lifetime-principles.md). Wrappers `get_reference`/`release` the instance; they do not care in_pool vs and_pool vs permanent.

**`get_assignable_value` is the heavy lifter.** If the inf is wrong, fix **that inf**. Returns something that can be released, or that dies with a pool:

| Kind | `release` | Where it lives | Assignable |
|------|-----------|----------------|------------|
| Permanent scalar | no-op | forever | self |
| Permanent object/array | no-op | forever | managed wrapper/clone |
| **unmanaged** object/array (live in `p`) | **value** `release` **throws** | caller’s `p` | `clone_managed` (script `{}` / `[]`) or managed wrapper (runtime/adapter). **Instance** `get_reference` pins `object->p` |
| **unmanaged_new_p / cede_p** object/array | instance last `release` drops the pool | own pool | same isolate |
| Unmanaged scalar | **throw** | tracker / heap bump-alloc | managed copy in **`p->managed_p`** |
| Managed scalar | `free_memory` on the alloc pool | `p->managed_p` | bump |

**Managed never lives on a tracker.** Tracker last-release (when the **frame hold hits 0**) frees leftover **unmanaged and random** allocs. `release` of unmanaged/permanent is a no-op; the pool going away is what reclaims them.

**Natural lifetime:** allocate in the current frame; **assign** if it must outlive this frame; deactivate / last-release when the frame is unreferenced. Inner `{ i = i + 1 }` does not last-release outer `i` (different frame). Loop: replace last_return → previous in_pool hold drops → previous frame can die.

**`compiled_value` evaluate:** caller does not release. Park `script_result`. Nested `evaluate(compile())` parks `script_result`. If `script_result` is set, that is the result (managed in dest `p->managed_p` of the isolate write: `scope->p` at deactivate). Register last-release of that one hold on this evaluate dest `p` and return as-is. No `clone_unmanaged`. Permanents skip the register.

**Function return:** pin on the caller (`get_assignable_for_p_lifetime`). No wrapper inf. Parameter args are a **frame** (replace into param slots); rewind = deactivate that frame.

**`xctx->p`:** one ST job heap for that run (request, CLI), created `*_as_managed_p`. Base xctx `p` is `env->p` (MT process heap). Tracker = leftover list on the ancestor heap. Unmarked heap/MT create **inherits** `managed_p`; `*_as_managed_p` is `managed_p = self`. Managed values allocate in dest `p->managed_p`. Compile units are their own ST heap (`heap_create`, inherit, 4k min). Evaluation `{ }` is `scope_create` (ST heap, 4k, inherit, throw delay). Heap chunks come from the thread `afw_memory_region`. The job-heap free list skips a walk when its largest free block is too small ([PR #375](https://github.com/afw-org/afw/pull/375)). Size buckets stay off. Live pool map: [`remaining-apr.md`](remaining-apr.md).

**Tests** `return_temps.as` / `evaluate_once.as` are **product** (don’t drop a callee result before the caller uses it; `key()` once). They are not a reason to hop create.

### Rejected (do not remember as the plan)

- `afw_pool_assignable_p` / hopping dest inside `as_assignable` or object/array **create**. Create uses the `p` the caller passed. Properties stay in `object->p`.
- Donate / poke FRV into `script_result`; extra-hold save/restore helpers; `as_assignable` of an unheld leftover last after the occupant was last-released.
- `get_reference` doing `clone_or_reference` (dest `p` on the bump). Abandoned; dest `p` ripped.
- `create_and_activate`. Create RC 1; creator `release`s; activate is only push.
- Frame assign onto a second eval heap so overwrite recycles — that kept names off the frame tracker. Names of **this** frame live in **this** `scope->p` (in_pool / unmanaged). Promote only on assign **out**. There is no `evaluation_heap`; scopes track `xctx->p`.

### Watch

- Unmanaged scalars die with the frame (`release` no-op). They become managed only via `get_assignable_value` (variable assign, last_return). `add(1,1)` as a temp is not promoted. Unassigned-temps soak **flat** (will-not-do, [#406](https://github.com/afw-org/afw/issues/406)).
- Unmanaged **value** `get_reference` / `release` throw. Instance methods pin the pool. Isolate with `get_assignable_value`.
- An in_pool last_return holds the **frame** until the next replace (leftovers of that trip stay). That is any in_pool value, not a special `{ }`. Empty **block** `{ }` has no last_return; tracker header already recycles. Empty **object** `{ }` is a real in_pool value. No special case; if something still special-cases empty object/block, find why and prefer inf methods. Tiny workaround OK only if we come back.
- Rewind: do not leave last_return pointing into a tracker whose hold already hit 0. Either assignable already held the frame, or set undefined/void.
- Catch last_return: bind the error after the catch frame exists, then `afw_value_block_evaluate_statements`. Empty catch writes nothing (`try` is void except `return`/`rethrow`). Inner `AFW_TRY` shadows `this_THROWN_ERROR` — save the caught error first.
- Object and array are responsible for the lifetime of values they store. Managed object last-release free_memorys remaining property entries and the name index. Splice copy-out leftover RC (**#405**): `create_managed` plus `get_assignable_for_scope_lifetime` left an extra RC. Register last-release on dest `p` (`afw_pool_scope_release_value_at_cleanup`) is the `create_managed` path for splice / reverse / slice / filter / map / sort / bag / keys / values / entries (caller does not release). Mutate-input (`push` / `pop` / `freeze`) uses `get_assignable_value` then register last-release (`get_assignable_for_scope_lifetime`). Managed `remove` / `remove_value_by_index` last-releases the source occupant; `pop` / `shift` transfer. Assigned soaks watch leftover RC after `slot_store`; unassigned soaks watch register last-release (last stmt must not be the create). C `_internal` mint into a managed object/array is `create_managed` then `set_property_take` / `push_value_take` (`afw_value_slot_take`): one managed value, slot takes the birth hold, no `get_assignable` bump ([PR #437](https://github.com/afw-org/afw/pull/437)). `slot_store` stays isolate (temps, unmanaged incoming). `cede` stays the pool word (`_cede_p`).
- FRV leftover wrapping **dropped** ([PR #326](https://github.com/afw-org/afw/pull/326)): no `function_return_value`; pin on caller. Do not reopen unique consume or a call-result leftover inf. Product tests (`return_temps.as`) stay; the soak lab is [`src/afw/tests-extra/issue-2/01-rss-hard-loops/README.md`](../src/afw/tests-extra/issue-2/01-rss-hard-loops/README.md). `function_return` is **flat** on `develop` (2026-09-16). Map: [`compile-unit-and-frv-next.md`](compile-unit-and-frv-next.md).
- Managed array owns stored values (`afw_array_create_managed`). Push/set/insert `slot_store`. Get peeks. `remove` / `remove_value_by_index` `release` the occupant. `pop`/`shift` transfer, then `afw_pool_release_value_at_cleanup` on dest `p` (caller does not release). Do not `get_assignable_for_lifetime` on the pop result (extra bump). `array_push_pop` and both splice soaks **flat**.

---

## Creates (`<thing>_<action>[_<modifier>]`)

| | Function | Meaning |
|--|----------|---------|
| Object | `afw_object_create_unmanaged(p, xctx)` | Lives in `p`. Start 0. Value `get_reference` / `release` throw. |
| Object | `afw_object_create_unmanaged_new_p(p, xctx)` | New child of `p->managed_p`. Object interface last `release` drops the pool. |
| Object | `afw_object_create_unmanaged_cede_p(p, xctx)` | Same, using caller-built `p`. |
| Array | `afw_array_create_unmanaged` / `create_unmanaged_of` / `create_unmanaged_new_p` | Same pair. |
| Scalar | `afw_value_<dt>_create(…)` | Unmanaged. Header in `p`. utf8/memory: copy the struct only, not octets. |
| Scalar | `afw_value_<dt>_create_managed(…)` | Start 1. utf8/memory copy octets. Must `release`. |
| Scalar | `afw_value_<dt>_create_managed_slice(…)` | View of a managed utf8/memory value. Holds containing. |

Dual face: the instance **is** the value. Pool-world object/array creates always have `unmanaged` / `unmanaged_new_p` / `unmanaged_cede_p` in the name; frames are `create_managed`.

---

## MUST NOT

- Wrap at execute (`create_managed` in `+`, etc.). Builtin that *fills a slot* uses `get_assignable_value`.
- Helpers around assign (`create_if_needed`, donate list, silent dest hop / `assignable_p` on create).
- Putting **managed** values on a **tracker**.
- Last-release of a frame while last_return still points at that tracker without `get_assignable_value` (no frame hold).
- Face vs in_pool **store fork**: face `slot_store`s; generic memory object stores a raw pointer (compile literals must not wrap nested objects/arrays). Drop `release`s only if the array held (face), like object delete. Splice return is an unmanaged object of slot copies: `get_reference` the occupant, then source drop. Assign / FRV `get_assignable_value` of that object mints the script face (self if already a face). Do not wrap the return at execute. pop/shift transfer (no `release`).
- Compiler wrap emit as the isolation protocol.
- Teach generic memory objects to own properties.
- Put “always new overlay” on every face (`let y = x` is bump).
- Promote compiled constants to the **permanent** inf as the isolation fix.
- Add a literal inf or compile constants as `object_expression` for isolation.
- Skip-hold at retrieve or `clone()` because of path/reconcilable.
- Mint a mutable face over catalog qualifiers (`environment::`, `process::`, `request::`, …) because C left the memory object writable.
- `if (is_memory_wrapper)` at call sites — that is the assignable inf.
- Use infinite no-brace `i = i + 1` RSS as proof of wrap (temps die with the pool).
- Mix `#62` `script_result` into this.
- Use `create_managed_*` from execute paths without a matching `release`.
- Continue `issue-2-managed-p` C.

Large-string RC later is **private** to the string inf. Nothing else notices.

---

## Order (re-decide after each)

**Done on `develop`:** slot protocol; pool two-impls ([PR #267](https://github.com/afw-org/afw/pull/267)); two worlds **#277** closed; compile-literal + intern **#280**; compile facts + RC 1, enclosing_block bind, `get_assignable_value` table; `get_reference` takes `xctx` only; eval `p` = `scope->p` ([PR #287](https://github.com/afw-org/afw/pull/287)); compile() is a unit ([PR #305](https://github.com/afw-org/afw/pull/305)); every `{ }` is a scope + `last_statement_non_void_value` on the running frame ([PR #306](https://github.com/afw-org/afw/pull/306)); isolate last at clone + wrap unbraced loop bodies ([PR #307](https://github.com/afw-org/afw/pull/307)); builtin lifetime hold ([PR #308](https://github.com/afw-org/afw/pull/308)); managed `pop`/`shift` temp-on-scope ([PR #309](https://github.com/afw-org/afw/pull/309)). Heap and tracker are the two pool impls (plus mt lock wrappers). APR is gone from libafw. `AFW_DEBUG_POOL` prefix always checked on free; USER poison on free. `xctx->p` is ST job heap; `env->p` is mt heap. last_statement_non_void_value: [PR #306](https://github.com/afw-org/afw/pull/306) / [PR #307](https://github.com/afw-org/afw/pull/307) / `script_result.as`. Script/function door: empty void → `undefined`. Declared `: void` stays void. Managed scalar/slice last-release **does** `free_memory` via the stored p. RSS soaks for assign / overlay / rebind / empty `{ }` / unbraced loop body are **flat** (2026-09-10). Live table: [`src/afw/tests-extra/issue-2/01-rss-hard-loops/README.md`](../src/afw/tests-extra/issue-2/01-rss-hard-loops/README.md). **[PR #355](https://github.com/afw-org/afw/pull/355), on `develop`:** managed `compiled_value`; dest `p` on `create_managed` / `clone_managed` / `get_assignable` / `slot_store`; `{ }` is inherit-heap `scope_create`; evaluate **pins** dest `p`.

**Parked / follow-up:** FRV leftover wrapping is **dropped** (pin on caller). Adapter clones. Clone-of-unmanaged object meta. Adaptive `clone()` of object/array is always-copy `create_managed`, register last-release of the execute result after the copy, take nested (C clone helpers [#424](https://github.com/afw-org/afw/issues/424); `is_root` gone on [#445](https://github.com/afw-org/afw/issues/445)). Functions/closures as an eval result still alias the unit ([#342](https://github.com/afw-org/afw/issues/342)). `qualifier("current")` snapshot tail. Do not spread `get_reference` in `execute_*`. Tracker allocated list forward-only later. Do not wrap catalog qualifiers. Do not add `get_base` unless more than one product site type-switches for “entity.” Overlay `o.x = i` / `o = { n: i }` are **not** parked leak verticals — those soaks are flat.

**Nominated next eval win (withdrawn):** heap free-list mixed sizes. Re-measured on `develop` after #287: concat + integer `last_return` in the 14k nest is ~0.05s (pre-#277). Timings: [`experiment-brainstorm.md`](experiment-brainstorm.md). Do not rewrite the pool for that loop.

If a step gets clever, stop and ask.

---

## Scatter (current code vs rails)

`clone_or_reference` is a compatibility name for `get_reference`. Donate list and `isolate_mutable_default` are gone. **Locked design** wins. Do not add helpers.

**Around-assign wrap (`create_if_needed`):** moved into `script_function` `get_assignable_value` (slot_store / face overlay `set` call it). Helper remains only as the inf’s wrap. Compiler assign/return/object/array sites no longer call it.

**Compiler `wrap_literal_*` emit:** removed. Isolation is `get_assignable_value` (clone or wrapper). Permanent scalars stay as-is. LHS `reference_by_key` `get_assignable_value`s, sets, releases. Face GET/array materialize/retrieve/journal `slot_store`. Donate list removed. Unmanaged memory object store is a raw pointer (like object set).

**`compiled_value` evaluate:** last-release of the result on dest `p` of this evaluate (`release_value_at_cleanup`); return as-is. Isolate dest is dest `p` of the write (`script_result_set`, `scope->p` at deactivate). Not FRV. `script_result` is **#62**.

**Donate / extra slot:** removed. `slot_store` is `get_assignable_value` then release occupant.

**Scalar managed (start 1):** in `p->managed_p`, must `release` (`free_memory`). Unmanaged scalars: `release` no-op; die with the frame tracker. Frame **names** of this `{ }` live in `scope->p` (in_pool), not on a second eval heap. Promote only when assigning **out** (`last_return` / outer frame / caller `p`).

**Current code:** pool-world dual-faces (including env-vars object, `create_embedded`) use **unmanaged** inf. Views stamp **unmanaged** so retrieve slot fill wraps. `isolate_mutable_default` deleted; array materialize/GET is `slot_store`. Reconcile diffs the **face**.

**Storeable infs with `get_reference` NULL (need a method or stay graph-only):** `script_function` implements `get_assignable_value` (`closure_binding`). `reference_by_key` is a place, not stored. Graph: `block`, `call`, `compiled_value`, `symbol_reference`, … Data-type bindings have generated methods (managed / unmanaged / slice).
