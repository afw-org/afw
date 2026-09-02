# Issue #2 — hold in the inf (revised 2026-08-24)

**Audience:** maintainers / assistants. **Not** handbook.

**GitHub:** [#2](https://github.com/afw-org/afw/issues/2).  
**On `develop`:** pool two-impls + last_return frame-end store ([PR #267](https://github.com/afw-org/afw/pull/267)). `issue-2-managed-p` is gone (tests harvested; C on that branch is not the rails).

**This pad is the rails.** The 2026-08-20–21 story and archaeology stay in [`issue-2-lifetime.md`](issue-2-lifetime.md). If that file and this one disagree, **this file wins**. If a leak tempts a helper *around* assign, operators, or the compiler — **stop and ask**. That is how we dug holes.

Keep from `develop`: slot protocol, pool split, `#35` store-time bind (move *into* the inf), `#245` 0-symbol `{ }`, `#246`/`#247` honest heap/tracker.

---

## Locked design

Add a value method only when we want to do **one thing** and the code is `if` this type **then** A **else** B. Callers name the thing; each inf implements A or B. If we cannot name the thing in one phrase, split the **use**, do not add a kitchen-sink method. (`metas()` views may bite later; fix on that inf, not a new method.)

**Value infs (and optional `compiler_internal` calls) are the compiler’s runtime.** Create = pick the instruction. `optional_evaluate` / `execute_*` = run it. Graph nodes are declarative IR. A second compiler may mint the same infs and call `afw_value_evaluate()`. Prefer a new inf when it is a *kind of value* that will sit in a slot.

Callers:

- **Read** a slot → the pointer.
- **Keep alive** → `get_reference` (matching `release`).
- **Fill a slot** (assign, param, overlay set, `return` / call result) → `get_assignable_value` (matching `release`).
- **Operators / `+`** → pointer. Do not wrap at execute.
- **Call / `return`:** `get_assignable_value` **before** the callee frame dies. Everyday `evaluate()` must not leave a return temp in the caller’s hands.

**Return temp (`function_return_value`, this branch):** Script `return` / last expression wraps the occupant (`as_assignable` at create). `optional_evaluate` peeks. `get_assignable_value` is hold-inner + `release` wrapper. Public `evaluate()` consumes a temp so callers never `is_function_return_value`. Parameter window: `evaluate_for_parameter` (raw eval) then park the **occupant** in the parameter-number slot (`pop_parameter_number(VALUE)` replaces `#`). `pop_value` / rewind `release` parked occupants. One live marker pair per function while that parameter is being evaluated; after pop, `[call]` + 0 or more returns to top. Do **not** extra-push while a marker is on top. Do **not** hop `compiled_value` on assign to `unevaluated`. `meta()` does not evaluate argv first (property `key`). Script `meta()` snapshots are immutable. Array look-through: not a goal. Model mapped modify tuples: hold.

Tests: `language/script/return_temps.as`. Full suite ~4245 pass; hold 3 model modify/reconcile tuple tests; smtp `hexBinary`/`null` looks unrelated.
- **Retrieve / Adaptive `clone()`:** return the entity or structural clone. Slot fill wraps. Reconcile diffs the **face** (overlay sets); do not peel `wrapper_base`. No skip-hold. No `get_base` method until more than one product site type-switches for “entity.”
- **Script door ≠ C bag.** Adapters/conf may leave a memory object writable. Script retrieve gets a working copy (slot fill / view with no setter). **Catalog qualifiers** (`environment::`, `process::`, `request::`, `application::`, `adapter::` conf, `custom::` conf) stay **read-only in script** — SET should throw, not mint a face, even if C never called `set_immutable`. `qualifier()` / `qualifiers()` are **copies** (#9); mutating a copy is fine. `current::` is a **name set per context**, not one object: `current::object` in a model hook is a working object (retrieve-like); in auth/index it is the resource/row (read). Live catalog writes, if ever, are a **function** with execute access and authorization that actually changes the host. Do not stamp catalog objects unmanaged “for consistency” with views.

Default impl: `#define impl_afw_value_get_assignable_value impl_afw_value_get_reference` before `afw_value_impl_declares.h`. Graph infs NULL (evaluate first).

| Inf | `get_reference` | `get_assignable_value` |
|-----|-----------------|------------------------|
| Managed scalar | bump | same |
| Permanent scalar | self | same |
| Unmanaged scalar | managed copy | same |
| Unmanaged/permanent object & array (**memory bag**) | self | mint **assignable** face |
| **Assignable** object & array (new inf, like `closure_binding`) | bump | `get_reference(self)` |
| `script_function` | self | `closure_binding` |
| `closure_binding` | bump | `get_reference(self)` |
| `metas` / const view | self | self |
| Graph (`call`, `block`, `symbol_reference`, `object_expression`, …) | NULL | NULL |

No new **literal** inf. Constant `{a:1}` / `[1,2]` stay fully evaluated unmanaged (or permanent) bags. Isolation is assignable on that inf. Do not compile constants as `object_expression`. Do not promote them to permanent as the isolation fix.

`object_expression` / `object_construct` stay: evaluate mints an **assignable** face. Spread / mixed arrays often a `call` of `array(...)` / `add_properties(...)`.

`create_wrapper_*` / `create_script_wrapper` are the **create** of the assignable inf, not a wrap protocol.

`property_get` / `variable_get` default is the evaluated occupant (**identity**). No `isolate_mutable_default` in the model. Inline `{ }` is isolated because it is still a raw bag when assignable runs.

`slot_store` = `get_assignable_value(incoming)` **then** `release` occupant **then** store. Same-pointer skip stays. Clone/hold first so incoming that still points at the occupant’s bytes (`s = s + s`, substring onto self) is not memcpy’d into a reused pool block (issue #275, valgrind `Overlap`). `let y = x` shares the assignable face.

**Place (LHS) `reference_by_key`:** `get_assignable_value(evaluate(aggregate))`, set, `release`. `{}.x = 1` mints a throwaway assignable face, set, `release` tears it down.

Face GET: store `get_assignable_value` of the retrieved child locally. Face SET: same pair into overlay. No write-through. Generic objects **do not** own properties.

`freeze()`: `get_assignable_value` then freeze **that** handle. Adaptive `clone()`: structural copy, then `get_assignable_value` (no path exception).

Current code still names `clone_or_reference`. Treat it as the overloaded stand-in until the split is wired.

---

## Frame, last_return, tracker (locked 2026-08-27)

This is the path. Not `assignable_p` on create, not hopping dest `p` inside `as_assignable`, not “keep objects off the tracker so last-release is safe.” Those got tests green and flattened `i = i + 1` by **not** putting the trip on the tracker. That work was thrown away. Do not put it back.

**Evaluate `p` is the frame.** Statement-list `p` is `scope->p`. That is `x->p` in built-ins. Nested `evaluate` in that frame gets the same `p`. Temps (`1+1`, `{}`, `[]`, random mallocs) land here.

**Frame vs block.** Frame deactivate is the pool story. Block, on a **normal** fall-out, writes **last_return**. Throw/break/rewind deactivates frames and does not invent a result. Frames are one-to-one with `{ }` in the model; today zero-symbol `{ }` may skip a frame and `for (let …)` clones an extra one of **names**.

**last_return** (code today: `xctx->script_result`). Last non-void statement. Not every assign poking a slot. Walk the list with a **raw** last (skip **void** and break/continue/rethrow). **undefined is a value** and does replace. Empty if/block/loop/try/switch return void, not undefined. **One** `slot_store(&xctx->script_result, last, xctx->p)` when a **frame** (created scope) finishes, **before** tracker deactivate. Nested 0-symbol `{ }` returns that raw last to the parent list. `execute_return` boxes with `as_assignable` onto `xctx->p` and `slot_store`s when `script_result_active` (no FRV pointer-assign). as_value / template substitution clears that flag so `${return a}` vs `${return b}` do not share the slot. Skip the frame-end store if return already wrote. while/for/try/switch are UpdateEmpty: the body’s last non-void bubbles, including **across loop iterations** (empty break/continue does not wipe a prior iteration’s assign). for init/increment are not body statements. Dest for last_return is **`xctx->p`**. Dest for a frame name is **that frame’s `scope->p`**. If the incoming is void, **do not replace**. `{ let x = 1; { add(1,1) } }` last_return is `2`; `{ add(1,1); let x = 1 }` stays `2`. Empty `{ }` writes nothing. `let`/`const` are void; `x = 1` as a statement is not (completion is `1`). A call that returns undefined writes. Declared `: void` does not. Jeremy-script / ES — confirm if we ever disagree.

**`get_reference` pairs with `release`. Full stop.** Any non-permanent object/array inf honors both even if it starts at count 0. If you `get_reference`, you owe a `release`. in_pool instances still **die with their pool** if nobody extra-held them.

**Script sees values, not bags.** Value protocol is `get_reference` / `get_assignable_value` / `release`. `get_assignable_value` is what makes a temp into something that **has** those three methods (managed scalar in `xctx->p`, or a face that extra-holds). Wrappers `get_reference`/`release` the bag; they do not care in_pool vs and_pool vs permanent.

**`get_assignable_value` is the heavy lifter.** If the inf is wrong, fix **that inf**. Returns something that can be released, or that dies with a pool:

| Kind | `release` | Where it lives | Assignable |
|------|-----------|----------------|------------|
| Permanent | no-op | forever | self |
| **in_pool** object/array (old “unmanaged”) | no-op | caller’s `p` | `get_reference` — **including the frame/pool** so the frame lasts while referenced |
| **and_pool** object/array | last `release` drops the pool | own pool | bump |
| Unmanaged scalar | no-op | tracker / heap bump-alloc | managed copy in **`xctx->p`** |
| Managed scalar | `free_memory` on `xctx->p` | always `xctx->p` | bump |

**Managed never lives on a tracker.** Tracker last-release (when the **frame hold hits 0**) frees leftover **unmanaged and random** allocs. `release` of unmanaged/permanent is a no-op; the pool going away is what reclaims them.

**Natural lifetime:** allocate in the current frame; **assign** if it must outlive this frame; deactivate / last-release when the frame is unreferenced. Inner `{ i = i + 1 }` does not last-release outer `i` (different frame). Loop: replace last_return → previous in_pool hold drops → previous frame can die.

**`compiled_value` evaluate:** save last_return; run on `xctx->p` + frames (script already has a root block). **One** `get_assignable_value` of the unit result onto the **caller `p`**; restore last_return; release this unit’s tracker(s). Nested `evaluate(compile())` is the same nest. That is the evaluate **contract** clone, not a per-statement ceremony.

**Function return:** still consume last_return and wrap `function_return_value` so the callee frame can die. Caller `get_assignable_value` of the FRV. Parameter args are a **frame** (replace into param slots); rewind = deactivate that frame.

**`xctx->p`:** execution heap for that run (request, CLI, core startup on the main thread). Heap = APR plus optional free (`p` + **size**). Tracker = leftover list on that heap, header is a heap block. `server->p` / env instance stay general APR (mt, produce-once). `managed_p` hops were for the unfinished value-aware pool; stop using them as a silent rewrite of create `p`.

**Tests** `return_temps.as` / `evaluate_once.as` are **product** (don’t drop a callee result before the caller uses it; `key()` once). They are not a reason to hop create.

### Rejected (do not remember as the plan)

- `afw_pool_assignable_p` / hopping dest inside `as_assignable` or object/array **create**. Create uses the `p` the caller passed. Properties stay in `object->p`.
- `slot_store` on `script_result`, park/suppress/save-restore for increment, FINALLY `as_assignable` after the tracker is already dead.
- Frame assign onto `evaluation_heap` so overwrite recycles — that kept names off the frame tracker. Names of **this** frame live in **this** `scope->p` (in_pool / unmanaged). Promote only on assign **out**.

### Watch

- Unmanaged scalars die with the frame (`release` no-op). They become managed only via `get_assignable_value` (variable assign, last_return). `add(1,1)` as a temp is not promoted. If a loop of unassigned temps grows, revisit — do not hop create.
- in_pool `get_reference` / `get_assignable_value` must hold the **frame/pool**, not only the object header.
- An in_pool last_return holds the **frame** until the next replace (leftovers of that trip stay). That is any in_pool value, not a special `{ }`. Empty **block** `{ }` has no last_return; tracker header already recycles. Empty **object** `{ }` is a real in_pool value. No special case; if something still special-cases empty object/block, find why and prefer inf methods. Tiny workaround OK only if we come back.
- Rewind: do not leave last_return pointing into a tracker whose hold already hit 0. Either assignable already held the frame, or set undefined/void.
- Catch last_return: bind the error after the catch frame exists, then `afw_value_block_evaluate_statements` (same UpdateEmpty as evaluate_block). Empty catch writes nothing. Do not copy that loop again. Inner `AFW_TRY` shadows `this_THROWN_ERROR` — save the caught error first.
- Object and array are responsible for the lifetime of values they store. Splice copy-out `get_reference` into an unmanaged bag, then source drop, may extra-hold when assign later mints a face and materialize `slot_store`s again — leak, not crash. Clone-into-`x->p` instead would get weird for nested object/array elements. Keep on the worry list; do not "fix" with wrap-at-execute.

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
- Face vs in_pool **store fork**: face `slot_store`s; generic bag stores a raw pointer (compile literals must not wrap nested bags). Drop `release`s only if the array held (face), like object delete. Splice return is an unmanaged bag of slot copies: `get_reference` the occupant, then source drop. Assign / FRV `get_assignable_value` of that bag mints the script face (self if already a face). Do not wrap the return at execute. pop/shift transfer (no `release`).
- Compiler wrap emit as the isolation protocol.
- Teach generic memory objects to own properties.
- Put “always new overlay” on every face (`let y = x` is bump).
- Promote compiled constants to the **permanent** inf as the isolation fix.
- Add a literal inf or compile constants as `object_expression` for isolation.
- Skip-hold at retrieve or `clone()` because of path/reconcilable.
- Mint a mutable face over catalog qualifiers (`environment::`, `process::`, `request::`, …) because C left the bag writable.
- `if (is_memory_wrapper)` at call sites — that is the assignable inf.
- Use infinite no-brace `i = i + 1` RSS as proof of wrap (temps die with the pool).
- Mix `#62` `script_result` into this.
- Use `create_managed_*` from execute paths without a matching `release`.
- Continue `issue-2-managed-p` C.

Large-string RC later is **private** to the string inf. Nothing else notices.

---

## Order (re-decide after each)

Done on this branch (still named `clone_or_reference` in code): scalar managed holdables, object/array hold at unmanaged/permanent `clone_or_reference`, wrap emit gone, `script_function` → `closure_binding`, create names, harvest empty, **return temps** (`function_return_value` + park occupant).

**On develop (PR [#267](https://github.com/afw-org/afw/pull/267), 2026-08-28):** Heap and tracker are the two pool impls (plus mt lock wrappers). APR is reservoir only. `afw_pool.c` / `afw_pool_internal.h`. Heap live: no header, or `[size][pool][USER]` if `AFW_DEBUG_POOL` (always checked on free). Tracker live: links then that pair. Size is USER size. `create()` of a heap parent is a heap; of a tracker parent is a tracker (extra rule). `xctx->p` is ST heap; `env->p` is mt heap. last_return is raw last through the list (skip **void** only); one `slot_store` at created-scope finish onto `xctx->p`. Empty if/block/loop/try/switch/`rethrow` return void. Script/function door: empty void → `undefined`. Declared `: void` stays void.

**Still parked (`FIXME_GET_IT_WORKING`):** eval `p` is caller `p`, not `scope->p`. Skip `double_free_throws`. Tried ripping eval `p`: `language/script` stayed green; `comments-bmp-slash-0.as` (~14k nested `for (let …)` + `eval<script>`) climbed CPU/RSS — discarded. Double-free today throws prefix “pool does not match allocation” after overlay, not “already freed.” Do not spread `get_reference` in `execute_*`. Tracker allocated list forward-only later. Do not wrap catalog qualifiers. Do not add `get_base` unless more than one product site type-switches for “entity.”

If a step gets clever, stop and ask.

---

## Scatter (current code vs rails)

Code still uses `clone_or_reference` in places (stand-in until the split is fully named). Donate list and `isolate_mutable_default` are gone. **Locked design** wins. Do not add helpers.

**Around-assign wrap (`create_if_needed`):** moved into `script_function` `clone_or_reference` (slot_store / face overlay `set` call it). Helper remains only as the inf’s wrap. Compiler assign/return/object/array sites no longer call it.

**Compiler / script-facing wrap (isolation, not the new wrapper inf):**

| Site | What |
|------|------|
| `afw_compile_parse_value.c` | emit `wrap_literal_object` / `wrap_literal_array` |
| `afw_function_compiler_internal.c` | `execute_wrap_literal_*` → `add_reference` |
| `afw_compile_parse_script.c`, `afw_value_type_check.c` | unwrap wrap calls for type check |
| `afw_function_adapter.c`, `afw_function_journal.c` | `impl_script_face_object` on retrieve/journal |
| `afw_object_memory.c`, `afw_array_memory.c` | Face GET/array materialize `slot_store`; unmanaged bag store is a raw pointer (like object set) |

**Compiler `wrap_literal_*` emit:** removed. Unmanaged **and permanent** object/array `clone_or_reference` is `afw_value_object_hold` / `afw_value_array_hold`. Permanent scalars stay as-is. LHS `reference_by_key` holds, sets, releases. Face GET/array materialize/retrieve/journal/`wrap_literal_*` call hold/`slot_store`. Donate list removed; `slot_store` is always `get_assignable_value`.

**`compiled_value` evaluate:** no `afw_value_clone` of the result. Official `script_result` is already a hold (`return` stored it); otherwise one `clone_or_reference` onto a pool that is not the eval heap, then donate. Do not `add_reference` and `slot_store` the same pointer.

**Donate / extra slot:** removed. `slot_store` is always `get_assignable_value`. `compiled_value` wraps a running result as `function_return_value` and drops the old slot hold. `script_result` is **#62**.

**Scalar managed (start 1):** in `xctx->p`, must `release` (`free_memory`). Unmanaged scalars: `release` no-op; die with the frame tracker. Frame **names** of this `{ }` live in `scope->p` (in_pool), not on `evaluation_heap`. Promote only when assigning **out** (`last_return` / outer frame / caller `p`).

**Current code:** catalog / runtime objects (`environment::`, meta accessor, const key/value, aggregate external) stamp `managed_object_inf` so slot fill **bumps** (SET throws if no setter). That is the script-readonly door — not a leftover wrap. Views stamp **unmanaged** so retrieve slot fill wraps. `isolate_mutable_default` deleted; array materialize/GET is `slot_store`. Donate list is gone. Reconcile diffs the **face**.

**Storeable infs with `clone_or_reference` NULL (need a method or stay graph-only):** `script_function` implements it (`closure_binding`). `reference_by_key` is a place, not stored. Graph: `block`, `call`, `compiled_value`, `symbol_reference`, … Data-type bindings have generated methods (managed / unmanaged / slice).
