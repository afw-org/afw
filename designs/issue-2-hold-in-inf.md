# Issue #2 — hold in the inf (revised 2026-08-24)

**Audience:** maintainers / assistants. **Not** handbook.

**GitHub:** [#2](https://github.com/afw-org/afw/issues/2).  
**How close (scoreboard):** [Live status](#live-status-2026-09-25--how-close-is-2) below. If this pad’s older “Order / Parked” lists, the RSS lab README, the two-worlds pad, or the GitHub issue body disagree with that section, **the live status wins** until someone updates it.

**On `develop`:** pool two-impls ([PR #267](https://github.com/afw-org/afw/pull/267)). Two worlds (unmanaged dest `p` / managed `p->managed_p`) **[#277](https://github.com/afw-org/afw/issues/277) closed** (PR **#278**) — pad [`experiment-brainstorm.md`](experiment-brainstorm.md). `issue-2-managed-p` is gone. Dest `p` on `get_assignable` / `slot_store` / `create_managed` / evaluate pin: [PR #355](https://github.com/afw-org/afw/pull/355).

**This pad is the rails for inf methods** (`get_reference` / `get_assignable_value`, faces, MUST NOT). **Two worlds, create names, last_return slot:** [`experiment-brainstorm.md`](experiment-brainstorm.md) ([#277](https://github.com/afw-org/afw/issues/277)). **Eval `p`:** [`experiment-eval-p.md`](experiment-eval-p.md) ([PR #287](https://github.com/afw-org/afw/pull/287)). Pool doors: [`remaining-apr.md`](remaining-apr.md). The 2026-08-21 story is [`issue-2-lifetime.md`](issue-2-lifetime.md) (history — do not copy wording). If a leak tempts a helper *around* assign, operators, or the compiler — **stop and ask**.

---

## Live status (2026-09-25) — how close is #2?

**Still open.** Do not close. The **protocol is the system** on `develop`. The **campaign is not complete**. This section is the scoreboard for “how close”; it is not a sitting plan and not a rewrite of the rails below.

**Complete when** all of these are true (then remaining items are follow-up issues or explicit will-not-do):

1. Protocol matches the tree (maps and the GitHub body stop teaching dropped plans).
2. Script scope lifetimes are correct for the doors we already named: assign, overlay, rebind, `return`, closures, throw rewind, `last_result`. Product tests green; RSS soaks **flat or under an agreed bar**.
3. A long-lived `afwfcgi` does not silently climb on firehose / nested-eval soaks (process/server pool stats — request-end valgrind hides leftovers).
4. Parked polish is split off or will-not-do. Allocator tuning is not a close gate.

“One request can loop forever without climbing” is the close bar we have been using. **Values that survive a request** (adapter cache, reused compile units, runtime objects pin until the **caller** pool dies) is a separate remainder — child issue or will-not-do, not silent “#2 done.”

### Landed on `develop` (do not re-litigate)

Slot protocol; pool two-impls ([PR #267](https://github.com/afw-org/afw/pull/267)); two worlds **#277** closed (PR **#278**); names as values (PR **#220**); closures / throw-path rewind (**#35** closed); compile-literal + intern **#280**; `script_result` ([#62](https://github.com/afw-org/afw/issues/62) closed); `get_reference` takes `xctx` only (no dest `p` on the bump); scope RC 1 + `parent_scope_block` / `scope_depth`; eval `p` = `scope->p` when `{ }` has a frame ([PR #287](https://github.com/afw-org/afw/pull/287)); `compile()` is a unit ([PR #305](https://github.com/afw-org/afw/pull/305)); **every `{ }` is a scope** + `last_result` on the running frame ([PR #306](https://github.com/afw-org/afw/pull/306)); isolate last at clone + wrap unbraced loop bodies ([PR #307](https://github.com/afw-org/afw/pull/307)); builtin lifetime ([PR #308](https://github.com/afw-org/afw/pull/308)); managed `pop`/`shift` temp on current scope ([PR #309](https://github.com/afw-org/afw/pull/309)); script return pins on the caller — **no** `function_return_value` wrapper ([PR #326](https://github.com/afw-org/afw/pull/326)); APR gone from libafw, one ST heap per xctx, managed allocs in `p->managed_p` ([PR #327](https://github.com/afw-org/afw/pull/327)). **[PR #355](https://github.com/afw-org/afw/pull/355):** script/template/test_script compile returns a **managed** `compiled_value`; `create_managed` / `clone_managed` / `get_assignable` / `slot_store` / `closure_binding_create` take dest `p` (`p->managed_p`); evaluation `{ }` is `scope_create` (ST heap, 4k, inherit `managed_p`, throw last-release delay); evaluate of a compiled value **pins** `script_result` on dest `p` (no `clone_unmanaged`).

Hard-loop soaks remeasured **2026-09-16**; isolate sitting landed on `develop` as [PR #340](https://github.com/afw-org/afw/pull/340) (`d9cb1583`). Assign / overlay / rebind / empty `{ }` / unbraced loop body / `array_push_pop` / **`function_return`** / `let`/`const` empty `f()` **flat**. Nested `{ }` that wrote `script_result` **clears** parent last instead of planting the occupant. Try returns current last; `keep_return` extra-holds a real `return`. `try_catch` in_use **flat** (2026-09-17): managed hexBinary backtrace released on overwrite / caught ENDTRY / xctx teardown ([#341](https://github.com/afw-org/afw/issues/341) / [PR #354](https://github.com/afw-org/afw/pull/354)). Table: [`src/afw/tests-extra/issue-2/01-rss-hard-loops/README.md`](../src/afw/tests-extra/issue-2/01-rss-hard-loops/README.md). Gate 2026-09-17: `./afwdev build --fulldev` + `afwdev test -j` + valgrind **4516 passed**, 71 skipped. Later gate, PR #377: same commands, **4545 passed**, 71 skipped. PR #378 (`afw` leaves `response:error` off) was `./afwdev build --cdev` + `afwdev test -j`, **4545 passed**, 71 skipped, not a valgrind run.

**Since the 2026-09-19 scoreboard (not #2 close work):**

- [#363](https://github.com/afw-org/afw/issues/363) is **closed**. Profile hot spots are not a close gate. The job-heap free list skips the walk when the largest free block is too small ([PR #375](https://github.com/afw-org/afw/pull/375)). Free-list size buckets and the dual-ended carve stay off. Heap chunks come from the thread `afw_memory_region` ([#358](https://github.com/afw-org/afw/issues/358), closed). Pool doors stay [`remaining-apr.md`](remaining-apr.md).
- [PR #378](https://github.com/afw-org/afw/pull/378): the `afw` command leaves `response:error` off. A test script no longer keeps an evaluation listing on each caught error unless `--conf` (`defaultFlags`) or `flag_set` turns the flag on. The terminal printer still shows the error type, the message, and the short contextual lines. The 2026-09-17 `try_catch` soak was measured while the command still defaulted the flag on. `ks` is unchanged.
- GitHub [#2](https://github.com/afw-org/afw/issues/2) was rewritten 2026-09-25 so it stops teaching the old `function_return` climb and `clone_unmanaged` evaluate. Children: [#379](https://github.com/afw-org/afw/issues/379) (server soak), [#342](https://github.com/afw-org/afw/issues/342) (function/closure eval result), [#380](https://github.com/afw-org/afw/issues/380) (watch list), [#381](https://github.com/afw-org/afw/issues/381) (values that survive a request).

Complementary, not this close bar: request caps / `process::` telemetry ([#329](https://github.com/afw-org/afw/issues/329) / [PR #330](https://github.com/afw-org/afw/pull/330)); retrieve `maxObjects` ([#49](https://github.com/afw-org/afw/issues/49)); progressive release ([#127](https://github.com/afw-org/afw/issues/127)); qualifier snapshots ([#9](https://github.com/afw-org/afw/issues/9)).

### Still inside #2 (decide or do before close)

| Item | Why it is still #2 |
|------|---------------------|
| **Evidence** | [#379](https://github.com/afw-org/afw/issues/379). **2026-09-17** hard-loop table including `try_catch` **flat**. Isolate sitting [PR #340](https://github.com/afw-org/afw/pull/340). Server soaks remeasured after [PR #354](https://github.com/afw-org/afw/pull/354): `02` RSS **flat** 24576 kB; `07` wander 43.5→46 MiB then flat-ish (same shape as 2026-09-15); `07b` still too short (0.28 s). |
| **Managed dest / compiled results** | [#342](https://github.com/afw-org/afw/issues/342). Evaluated data results are **pinned** on dest `p` (not `clone_unmanaged`). Functions/closures as the eval result still alias the unit; `eval<script>` can dangle that result. Managed alloc is dest `p->managed_p`, not a hardcoded `xctx->p`. |
| **Watch (leak, not crash)** | [#380](https://github.com/afw-org/afw/issues/380). Splice copy-out then later assign; unassigned unmanaged temps in a tight loop; `readln` grows the line in `x->p` (`@fixme #2` in `afw_function_stream.c`). Soak or will-not-do. |
| **Escape past one xctx** | [#381](https://github.com/afw-org/afw/issues/381). Runtime objects / adapter cache / reused compile units. #149 closed the accessor slice. Clone-into-requestor-pool under the lock is a later option — not silent close of #2. |

### Parked — not close-blockers unless we say so

Adaptive `clone()` still calls `afw_value_clone()` (script `clone()` already extra-holds; aligning with `clone_unmanaged` / `clone_managed` is C-surface). Adapter clones / clone-of-unmanaged meta. `qualifier("current")` snapshot tail. `double_free_throws` skip (wrong throw prefix). These parked items are still listed here. They are not child issues yet, so close criterion 4 is not met. mmap and a per-size free list inside the heap stay parked. The thread `afw_memory_region` chunk free list and the job-heap largest-block skip ([PR #375](https://github.com/afw-org/afw/pull/375)) already landed. Size buckets stay off. Heap mixed-size rewrite **withdrawn** (14k concat nest ~0.05s after #287). Do not add `get_base`. Do not mint a mutable face over `environment::`, `process::`, `request::`, `application::`, or adapter/custom conf objects. Overlay `o.x = i` / `o = { n: i }` are **not** parked leak verticals — those soaks were flat.

### Do not start (dropped or parked)

- Unique consume, eval-stack leftover, `#function_return_value`, or a call-result leftover inf.
- Hop dest `p` / wrap at execute / helpers around assign.
- Heap size-class rewrite for the 14k nest.
- **`designs/maps/` harvest** — decided, not started. Topic-named current kit later; do not copy live maps in the meantime.

### Where truth lives

| Kind | Open |
|------|------|
| **This scoreboard** | This section |
| **Live maps** | This pad (rails); [`experiment-brainstorm.md`](experiment-brainstorm.md); [`experiment-eval-p.md`](experiment-eval-p.md); [`remaining-apr.md`](remaining-apr.md) |
| **Lab** | [`src/afw/tests-extra/issue-2/`](../src/afw/tests-extra/issue-2/) — table in `01-rss-hard-loops/README.md` (2026-09-17) |
| **History — do not copy wording** | [`issue-2-lifetime.md`](issue-2-lifetime.md), [`issue-2-hold-in-inf-plan.md`](issue-2-hold-in-inf-plan.md), [`memory-management.md`](memory-management.md) |
| **Code / tests** | Ground truth. When they disagree with a pad, fix the pad. |

Keep from `develop`: slot protocol, pool split, two worlds **#277**, `#35` store-time bind, `#245` then **every `{ }` is a scope** ([PR #306](https://github.com/afw-org/afw/pull/306)), `#246`/`#247` honest heap/tracker.

**Landed on `develop`:** compile facts `parent_scope_block` / `scope_depth` after the unit is complete (`finalize_scope_tree`). Every `{ }` is a scope ([PR #306](https://github.com/afw-org/afw/pull/306)); flattening a useless block is compile-side only. Scope create/clone start at RC 1; creator `release`s; `activate` / `deactivate` only push/pop. Bind script functions via `enclosing_block`, not syntax depth. `get_assignable_value` table (managed bump; unmanaged scalar promote; script `{}` / `[]` `clone_managed`; runtime/adapter object managed wrapper; permanent scalar as-is; permanent object/array wrapper/clone). Dest `p` ripped from `slot_store` / `as_assignable` / `get_assignable_value` / `get_reference` / `add_reference`. `get_reference` is a bump (`instance, xctx`) — leftover dest `p` was an abandoned plan to have it do `clone_or_reference`. `last_result` on each scope; deactivate `script_result_set`s unless cloned ([PR #306](https://github.com/afw-org/afw/pull/306)). **[PR #340](https://github.com/afw-org/afw/pull/340):** drop extra-hold last adopts; nested `{ }` that wrote `script_result` clears parent last; try `keep_return` extra-holds a real `return`. Clone isolates `script_result_set(original last)` then clone last stays void; loop `{ }` bodies do not extra-hold onto the clone; unbraced loop bodies wrap after parse in the current block ([PR #307](https://github.com/afw-org/afw/pull/307)). Built-in lifetime: `get_assignable_for_lifetime` / `set_last_result_for_lifetime`; hold instance first then mutate; new array results `create_managed` then fill; `array()` stays a script wrapper ([PR #308](https://github.com/afw-org/afw/pull/308)). Nested eval save/restore `script_result`. Script return is `get_assignable_for_p_lifetime` on the caller — no `function_return_value` wrapper ([PR #326](https://github.com/afw-org/afw/pull/326)). Eval `p` = `scope->p` when `{ }` has a frame: [PR #287](https://github.com/afw-org/afw/pull/287) / [`experiment-eval-p.md`](experiment-eval-p.md). Compile-literal inf + interned parse-word strings **#280**. One ST heap per xctx; evaluation `{ }` is `afw_pool_scope_create` of that heap. Managed allocs use `p->managed_p` ([`remaining-apr.md`](remaining-apr.md)).

---

## Locked design

Add a value method only when we want to do **one thing** and the code is `if` this type **then** A **else** B. Callers name the thing; each inf implements A or B. If we cannot name the thing in one phrase, split the **use**, do not add a kitchen-sink method. (`metas()` views may bite later; fix on that inf, not a new method.)

**Value infs (and optional `compiler_internal` calls) are the compiler’s runtime.** Create = pick the instruction. `optional_evaluate` / `execute_*` = run it. Graph nodes are declarative IR. A second compiler may mint the same infs and call `afw_value_evaluate()`. Prefer a new inf when it is a *kind of value* that will sit in a slot.

Callers:

- **Read** a slot → the pointer.
- **Keep alive** → `get_reference` (matching `release`).
- **Fill a slot** (assign, param, overlay set, `return` / call result) → `get_assignable_value` (matching `release`).
- **Operators / `+`** → pointer. Do not wrap at execute.
- **Call / `return`:** isolate **before** the callee frame dies (extra-hold on the running scope at `return()`, `script_result_set` at deactivate). Everyday `evaluate()` must not leave a return temp in the caller’s hands. Script return pins on the caller (`get_assignable_for_p_lifetime`); no leftover wrapper inf.

**Script return (no FRV wrapper):** `return()` is `set_last_result` (pointer). Pin is `get_assignable_for_p_lifetime` on the **caller** while the callee frame is alive. Do **not** reopen unique consume, eval-stack leftover, or `#function_return_value`. Parameter window: `evaluate_for_parameter` (raw eval) then park the **occupant** in the parameter-number slot (`pop_parameter_number(VALUE)` replaces `#`). `pop_value` / rewind `release` parked occupants. One live marker pair per function while that parameter is being evaluated; after pop, `[call]` + 0 or more returns to top. Do **not** extra-push while a marker is on top. Do **not** hop `compiled_value` on assign to `unevaluated`. `meta()` does not evaluate argv first (property `key`). Script `meta()` snapshots are immutable. Array look-through: not a goal. Model mapped modify tuples: hold. Map: [`compile-unit-and-frv-next.md`](compile-unit-and-frv-next.md).

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

**last_result** (code: `scope->last_result` plus `xctx->script_result`, [#62](https://github.com/afw-org/afw/issues/62) / [PR #306](https://github.com/afw-org/afw/pull/306) / [PR #307](https://github.com/afw-org/afw/pull/307)). Pointer only — not a managed slot like `frame_slots[]`. Starts void. Nested `{ }` that wrote `script_result` is returned as a normal statement result so parent last is a **pointer** at that occupant (a prior parent last must not stomp it at deactivate). `return()` is `set_last_result` (pointer); pin is `get_assignable_for_p_lifetime` on the caller. Deactivate `script_result_set`s unless the scope was **cloned**. Nested `compiled_value` / script call / block-as-value save and restore `script_result`. `let`/`const` do not write it. Nested assignment **does**. `for`/`while`/`try` are C-void except `return`/`rethrow`. Loop `{ }` bodies `evaluate_block`; if `script_result` changed, pointer last on the parent. A normal `finally` does not replace a pending try/catch return. **undefined is a value** and does replace. Tests: `script_result.as`, `for-let-break-keeps-previous-last`, `loop_unbraced_body.as`. `{ let x = 1; { add(1,1) } }` last is `2`; `{ add(1,1); let x = 1 }` stays `2`. Declared `: void` does not write.

**`for (let)` clone:** first trip is the for-let `{ }`. Next trip sibling-clones (copy slots). While the original `p` is still alive, `script_result_set_value(original last)` (void is a no-op); clone last stays void; original is marked cloned so deactivate does not write the slot again. Increment runs on the clone so a closure still sees the old `i`. Creator-`release` the previous; it dies unless a closure holds it. Without closures: wrapper until `for` ends + current clone. The never-cloned frame is the last iteration. Unbraced loop bodies wrap as a 0-symbol `{ }` after parse in the current block.

**`get_reference` pairs with `release` where they exist.** Managed / assignable / wrappers: if you `get_reference`, you owe a `release`. Unmanaged object/array **value** infs **throw** on those methods ([#277](https://github.com/afw-org/afw/issues/277)); isolate with `get_assignable_value`. **Instance** `get_reference` / `release` still pin `object->p`. Unmanaged instances still **die with their pool** if nobody extra-held them.

**Script sees values, not C instances.** Value protocol is `get_reference` / `get_assignable_value` / `release`. `get_assignable_value` is what makes a temp into something that **has** those three methods (managed scalar in dest `p->managed_p`, or a face that extra-holds). Wrappers `get_reference`/`release` the instance; they do not care in_pool vs and_pool vs permanent.

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

**`compiled_value` evaluate:** park `script_result`; run the graph; if `script_result` is set, that is the result (already assignable). Register `afw_pool_release_value_at_cleanup` on dest `p` and return as-is. No `clone_unmanaged`. Permanents skip the register. Nested `evaluate(compile())` is the same nest.

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

- Unmanaged scalars die with the frame (`release` no-op). They become managed only via `get_assignable_value` (variable assign, last_return). `add(1,1)` as a temp is not promoted. If a loop of unassigned temps grows, revisit — do not hop create.
- Unmanaged **value** `get_reference` / `release` throw. Instance methods pin the pool. Isolate with `get_assignable_value`.
- An in_pool last_return holds the **frame** until the next replace (leftovers of that trip stay). That is any in_pool value, not a special `{ }`. Empty **block** `{ }` has no last_return; tracker header already recycles. Empty **object** `{ }` is a real in_pool value. No special case; if something still special-cases empty object/block, find why and prefer inf methods. Tiny workaround OK only if we come back.
- Rewind: do not leave last_return pointing into a tracker whose hold already hit 0. Either assignable already held the frame, or set undefined/void.
- Catch last_return: bind the error after the catch frame exists, then `afw_value_block_evaluate_statements`. Empty catch writes nothing (`try` is void except `return`/`rethrow`). Inner `AFW_TRY` shadows `this_THROWN_ERROR` — save the caught error first.
- Object and array are responsible for the lifetime of values they store. Splice copy-out `get_reference` into an unmanaged object/array, then source drop, may extra-hold when assign later mints a face and materialize `slot_store`s again — leak, not crash. Clone-into-`x->p` instead would get weird for nested object/array elements. Keep on the worry list; do not "fix" with wrap-at-execute.
- FRV leftover wrapping **dropped** ([PR #326](https://github.com/afw-org/afw/pull/326)): no `function_return_value`; pin on caller. Do not reopen unique consume or a call-result leftover inf. Product tests (`return_temps.as`) stay; the soak lab is [`src/afw/tests-extra/issue-2/01-rss-hard-loops/README.md`](../src/afw/tests-extra/issue-2/01-rss-hard-loops/README.md). `function_return` is **flat** on `develop` (2026-09-16). Map: [`compile-unit-and-frv-next.md`](compile-unit-and-frv-next.md).
- Managed array owns stored values (`afw_array_create_managed`). Push/set `slot_store`. Get peeks. `pop`/`shift` transfer, then `afw_pool_release_value_at_cleanup` on the current scope so the extra-hold acts like a temp. Do not `get_assignable_for_lifetime` on the pop result (extra bump). `array_push_pop` soak **flat** on `develop` (2026-09-16).

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

**Done on `develop`:** slot protocol; pool two-impls ([PR #267](https://github.com/afw-org/afw/pull/267)); two worlds **#277** closed; compile-literal + intern **#280**; compile facts + RC 1, enclosing_block bind, `get_assignable_value` table; `get_reference` takes `xctx` only; eval `p` = `scope->p` ([PR #287](https://github.com/afw-org/afw/pull/287)); compile() is a unit ([PR #305](https://github.com/afw-org/afw/pull/305)); every `{ }` is a scope + `last_result` on the running frame ([PR #306](https://github.com/afw-org/afw/pull/306)); isolate last at clone + wrap unbraced loop bodies ([PR #307](https://github.com/afw-org/afw/pull/307)); builtin lifetime hold ([PR #308](https://github.com/afw-org/afw/pull/308)); managed `pop`/`shift` temp-on-scope ([PR #309](https://github.com/afw-org/afw/pull/309)). Heap and tracker are the two pool impls (plus mt lock wrappers). APR is gone from libafw. `AFW_DEBUG_POOL` prefix always checked on free; USER poison on free. `xctx->p` is ST job heap; `env->p` is mt heap. last_result: [PR #306](https://github.com/afw-org/afw/pull/306) / [PR #307](https://github.com/afw-org/afw/pull/307) / `script_result.as`. Script/function door: empty void → `undefined`. Declared `: void` stays void. Managed scalar/slice last-release **does** `free_memory` via the stored p. RSS soaks for assign / overlay / rebind / empty `{ }` / unbraced loop body are **flat** (2026-09-10). Live table: [`src/afw/tests-extra/issue-2/01-rss-hard-loops/README.md`](../src/afw/tests-extra/issue-2/01-rss-hard-loops/README.md). **[PR #355](https://github.com/afw-org/afw/pull/355), on `develop`:** managed `compiled_value`; dest `p` on `create_managed` / `clone_managed` / `get_assignable` / `slot_store`; `{ }` is inherit-heap `scope_create`; evaluate **pins** dest `p`.

**Parked / follow-up:** FRV leftover wrapping is **dropped** (pin on caller). Skip `double_free_throws`. Adapter clones. Clone-of-unmanaged object meta. Adaptive `clone()` still the old function. Functions/closures as an eval result still alias the unit ([#342](https://github.com/afw-org/afw/issues/342)). `qualifier("current")` snapshot tail. Double-free today throws prefix “pool does not match allocation” after overlay, not “already freed.” Do not spread `get_reference` in `execute_*`. Tracker allocated list forward-only later. Do not wrap catalog qualifiers. Do not add `get_base` unless more than one product site type-switches for “entity.” Overlay `o.x = i` / `o = { n: i }` are **not** parked leak verticals — those soaks are flat.

**Nominated next eval win (withdrawn):** heap free-list mixed sizes. Re-measured on `develop` after #287: concat + integer `last_return` in the 14k nest is ~0.05s (pre-#277). Timings: [`experiment-brainstorm.md`](experiment-brainstorm.md). Do not rewrite the pool for that loop.

If a step gets clever, stop and ask.

---

## Scatter (current code vs rails)

`clone_or_reference` is a compatibility name for `get_reference`. Donate list and `isolate_mutable_default` are gone. **Locked design** wins. Do not add helpers.

**Around-assign wrap (`create_if_needed`):** moved into `script_function` `get_assignable_value` (slot_store / face overlay `set` call it). Helper remains only as the inf’s wrap. Compiler assign/return/object/array sites no longer call it.

**Compiler `wrap_literal_*` emit:** removed. Isolation is `get_assignable_value` (clone or wrapper). Permanent scalars stay as-is. LHS `reference_by_key` `get_assignable_value`s, sets, releases. Face GET/array materialize/retrieve/journal `slot_store`. Donate list removed. Unmanaged memory object store is a raw pointer (like object set).

**`compiled_value` evaluate:** pin `script_result` on dest `p` (`release_value_at_cleanup`); return as-is. Not FRV. `script_result` is **#62**.

**Donate / extra slot:** removed. `slot_store` is `get_assignable_value` then release occupant.

**Scalar managed (start 1):** in `p->managed_p`, must `release` (`free_memory`). Unmanaged scalars: `release` no-op; die with the frame tracker. Frame **names** of this `{ }` live in `scope->p` (in_pool), not on a second eval heap. Promote only when assigning **out** (`last_return` / outer frame / caller `p`).

**Current code:** pool-world dual-faces (including env-vars object, `create_embedded`) use **unmanaged** inf. Views stamp **unmanaged** so retrieve slot fill wraps. `isolate_mutable_default` deleted; array materialize/GET is `slot_store`. Reconcile diffs the **face**.

**Storeable infs with `get_reference` NULL (need a method or stay graph-only):** `script_function` implements `get_assignable_value` (`closure_binding`). `reference_by_key` is a place, not stored. Graph: `block`, `call`, `compiled_value`, `symbol_reference`, … Data-type bindings have generated methods (managed / unmanaged / slice).
