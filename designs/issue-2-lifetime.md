# Issue #2 — lifetime working story (2026-08-21)

**Audience:** maintainers and AI assistants. **Not** handbook. **Not** shipped API.

**GitHub:** [#2 Memory management](https://github.com/afw-org/afw/issues/2).

**Status:** Working story recorded 2026-08-20–21. Slot protocol **landed**. Pool split **landed**. Closures / throw-path rewind (**#35**) store-time bind **landed**. Script-evaluation-aware wrapper holds **landed** on `issue-2-script-wrapper-holds`: overlay store is a slot; last-release walk is a pool cleanup (C-style `for` clone can drop instance RC to zero while the value is still in use); generic memory objects still do not own property values. Closure create-at-0 **landed**. No let/const hoisting. P3 first-fit still parked. Heap/tracker are single-thread only.

**This file is the campaign map.** Older notes, phase archaeology, and rejected experiments stay in [`memory-management.md`](memory-management.md). When that pad and this file disagree, **this file wins** until we change it on purpose.

**Lab probes (opt-in, not `test -j`):** [`src/afw/tests-extra/issue-2/`](../src/afw/tests-extra/issue-2/) — hard-loop Adaptive Scripts (`i = i + 1`, `o.x = i`, …) with `/proc` RSS sampling and gdb helpers. Expected red until optional `free` / same-size reuse is real. `afwdev test -T src/afw/tests-extra/issue-2 --show-all`.

**Whole-story framing** (why AFW is shaped this way): [`afw-philosophy-and-core-model.md`](afw-philosophy-and-core-model.md). Isolation faces (look-through, landed): [`issue-17-mutable-object-faces.md`](issue-17-mutable-object-faces.md). Payloads (`afw_utf8_t` / `afw_memory_t`): [`c-naming-and-payloads.md`](c-naming-and-payloads.md).

---

## Why this session

Umbrella **#2** is many feature branches. This one was: nail **managed / unmanaged / permanent** for **objects and arrays**, and the same words for **values**, with **pools** as the glue. We had discussed this before; it was not tidy. We brainstormed until the holes were tweaks, then recorded it here so it does not live only in chat or a container home directory.

**Do not call objects “bags.”** That word is the old XACML collection. Structured values are **object** (named **properties**) and **array** (ordered). When we need one noun for both as things that hold other values, say **object or array** or **value container** (not a scope).

Prefer **add_reference** / **release** / **reference count** in this pad. Do not write **RC**.

---

## The picture in one pass

**Destroy of a pool is lifetime. Optional `free` is reuse.** Forget a `release` and the **xctx** still **destroys** `xctx->p` when the request or `afw` command ends. That is the safety net.

**Managed** means the value or instance is in the hold protocol: every `add_reference` is matched by a `release`. Managed objects and arrays have **their own general pool** whose parent is **`p->managed_p`** (heap during eval, xctx/adapter/env job pool otherwise).

**Unmanaged** means you do **not** `release` unless you `add_reference`d. Count back to **zero does not destroy** the instance. Unmanaged objects and arrays live **in** the `p` passed (often a **heap tracker** / scope pool during eval).

**Permanent** (and **compiled-unit literals**, which should act the same): immutable; `add_reference` / `release` are no-ops; no pool on the value inf. Built-ins, `afw_v_*`, typed empty arrays, const objects. Everything **in a compiled unit** is immutable. Eval must not write it.

**Functions** (built-in `execute_*` and script bodies) almost never `add_reference` / `release`. Exceptions are rare and obvious (`object_retrieve_to_response`, shipping a value out of eval). Eval **scratch** dies with the **scope pool**.

**Assign** in `afw_function_compiler_internal.c` is the script hold site: `release` the old slot if needed, `add_reference` the new. **Read** is a pointer copy. Scope **last `release`** (not `}`) walks the frame. Closures keep the scope.

**Scalars** are not a managed/unmanaged inf pair. Temps live in whoever’s pool. `add_reference` on a temp makes (or, if already boxed, holds) a little **wrapper in `p->managed_p`**. After a variable holds that wrapper, other scopes use the same pointer. Generated **`managed_slice` can go away**. The method name we want is **`add_reference`** (today’s `clone_or_reference` was a memory-jog).

**Objects and arrays:** `add_reference` holds the **instance**, not a second value header in `xctx->p`. Generic memory objects **store and return** property pointers; they do not own those values. The value must **last at least as long as the object** (caller’s job). Script mutates a **script-evaluation-aware wrapper** (the faces from #17, possibly a dedicated impl). That wrapper **may** own **overlay** holds. Arrays grow the **same instance story as objects**. `create_array` is on the #2 on-ramp.

**`return`** writes this frame’s **hidden result slot** and ends the block. Assign into the **caller** happens **when the block ends**, then the callee last-`release`s.

**Pools:** general `afw_pool_create*` (APR, destroy-is-lifetime) vs **heap** + **heap tracker** (script eval, single-thread, one compiled_value wrap). Job pools set `managed_p = self`. No reparent on destroy; parent cascade destroys children. Optional first-fit tuning is later.

---

## Vocabulary

### Two layers, same three words

| Word | On an **object or array instance** | On a **value** (any data type) |
|------|-----------------------------------|-------------------------------|
| **Permanent** | Static / env / compiled-unit storage. No real holds. Immutable. | Inf has no count. `add_reference` returns the same pointer. `release` is nothing. |
| **Managed** | Instance is in the hold protocol. Own pool, parent `xctx->p` (or `cede_p`). Last hold can let that pool go. | In the hold protocol. Scalar **wrapper** after `add_reference` is this, and lives in `xctx->p`. Dual face of a managed object/array is the instance’s name, not a second header. |
| **Unmanaged** | Lives in someone else’s pool. Holds optional: only if you `add_reference`d. Zero does **not** destroy. `add_reference` holds **the pool**. | Temp in a pool. No self-destroy at zero. Escape through `add_reference` (scalars: box; objects/arrays: hold the instance). |

**`cede_p`** is not a fourth word. It is **managed**, with the pool handed in.

**`managed_slice`:** generated utf8/memory view that holds a containing managed value. **Target: remove.** A window into bytes is a normal utf8/memory whose pointer aims at existing memory (legal as a temp). `add_reference` **copies** the octets. Nothing in hand C calls `create_managed_*_slice` today.

Do **not** flatten “managed object” and “managed object value” when it could be either layer. Say which.

### Three things people called “wrapper”

| Thing | Job |
|------|-----|
| **Dual face** | `instance->value` — the Adaptive **name** of this instance. Identity. Never free that embedded header. |
| **Scalar `add_reference` box** | New value in `xctx->p` holding a copy of a scalar. Normal reference count. |
| **#17 / script face** | Look-through object/array: local sets, get can see the base, do not mutate compiled / adapter / runtime bases. Isolation, then also overlay holds. **Not** the scalar box. **Not** dual face. |

---

## Pools

### Destroy is lifetime

Every pool we care about in this pass has a **parent**. The ultimate ancestor in this discussion is **`xctx->p`**. Creating a child **`add_reference`s the parent**. When a pool’s reference count hits zero, it is **destroyed**. When the xctx is created, a try/finally **destroys** `xctx->p` at the end of the request or `afw` command. Leftover holds do **not** keep that request alive. Destroy is the hammer.

### Optional free is reuse

`free` does not have to be called. If it is, the block can go on a **list of optionally freed memory** so a loop of little scalars can reuse same-sized blocks. If it is not, the block still dies with the pool.

**First cut** (when we change pools): keep it simple; **combine adjacent** free blocks. **Later**, still only inside the pool: size-class lists for fixed-size scalars vs strings. Callers of allocate / optional `free` / destroy must not care.

**Current pools can host the value protocol.** Prefixes, first-fit, P3 livelock — do not gate #2 values on rewriting that. Get **`add_reference` / assign / walks** right first. Then swap the pool impl behind the same surface if we still want wrap-APR (no prefixes on live allocations).

**Do not** invent reuse policy by patching the current prefix `else` (P3). That livelocks first-fit.

### Object / array pools vs `xctx->p`

- Managed object/array: **own** pool, parent **`xctx->p`**.
- Unmanaged object/array: caller’s `p` (scope, helper, `xctx->p`, …).
- Scalar wrappers: **always `xctx->p`** (they are managed).
- Scope pool: eval **scratch** and the frame struct. Not where escaped scalar wrappers live.

---

## Unmanaged objects and arrays (holds)

Unmanaged instances **do** have a reference count. **Zero is the normal idle state.**

- Usual path: nobody `add_reference`s, nobody `release`s. Lifetime = the pool.
- `add_reference`: increment the instance **and** `add_reference` its pool.
- `release`: must match. Landing on **zero is not an event** for a **plain** unmanaged instance: no destroy, no teardown. The instance is still there until **the pool** goes away.
- If you referenced it, **you** `release`.

An object **may** register a callback for “pool is starting to go away.” Noted; not required to explain managed vs unmanaged. Useful later to clear pointers.

---

## Scalars and `add_reference`

**No managed/unmanaged scalar inf pair** as the product story. Permanent, or a temp in a pool, or the **`xctx->p` wrapper** after `add_reference`.

**Generated `add_reference`:**

- Permanent: return the same pointer; `release` is nothing.
- Already the wrapper: bump the count; return it.
- Pool temp: **new independent wrapper** in `xctx->p` (copy the scalar; copy utf8/memory **bytes**). Call as many times as you want on a temp: **independent** boxes. No intern table.

**Once assigned to a variable,** script copies that pointer and `add_reference`s **it**. Evaluating a variable must return the **slot pointer**, not mint a new box.

**`clone_or_reference`:** rename to **`add_reference`** when we change the bindings. Same name as pools and objects.

Over-cloning utf8/memory is the honest first cut. Hold/slice of a huge string is **later**, and would be new, not today’s generated slice infs.

---

## Assign, scopes, return, `for`

### Slots

A **slot** is a named variable, a **parameter**, or the **hidden result** on a frame. Protocol:

1. If the slot is not empty / not `undefined` / not `NULL`, `release` what is there.
2. Store `add_reference(incoming)`.

**Read** = copy the pointer. Do not `add_reference` on read. `let b = a` holds because **`b` is assigned**.

### When the scope walks

A scope **has** a reference count (tree today: `afw_xctx_scope_t`). Create starts at 0. Activate bumps it. A **closure** `add_reference`s the enclosing scope. A child holds its **parent lexical** scope. Block end / deactivate is **one** `release`.

**Walk slots on the scope’s last `release`, not on `}`.** Then the scope pool can go. Today the last `release` mostly releases the pool and does **not** walk slots — that walk is what this story adds.

### Functions and the scope pool

Temps from `1+2`, intermediate strings, unused unmanaged objects: **scope pool**. No hold protocol on every evaluation-stack push. When the scope’s last `release` destroys that pool, that scratch goes.

If the result is **assigned**, **assign** (or the wrapper’s `set`) holds it **before** that pool is allowed to kill the pointer. Functions do not “return managed”; they return a pointer.

### Hidden result and `return`

**Every frame has a hidden result slot** (today this is approximately `xctx->script_result`, saved/restored around calls — not yet a real frame slot).

- During the block, assignment and `return` write that saved value.
- **`return`** sets it and ends the block **early**.
- **Assign into the caller happens when the block ends**, then the callee last-`release`s.

Today deactivate runs **before** the caller stores the C return pointer. With holds, that order is the race. Target order: **assign to caller first, then walk the callee.**

`let x = f()` is then another assign from the caller’s hidden result to `x` (or the call writes `x` if that is already the destination). `f();` as a statement still uses the hidden slot.

This is the same idea as #62’s running result, not a second mechanism.

### `for` clone

Keep the clone: each increment needs its **own** bindings so a closure from the body sees **that** iteration.

**Clone = per-slot `add_reference`** into a new scope (same as assign). `undefined` is a no-op. Then deactivate the previous iterator scope as today. Do **not** only copy pointers (today’s FIXME). Do **not** reuse one frame (breaks closures). Do **not** copy the hidden result; the new clone starts `undefined`. Check `for_of` and friends when implementing.

---

## Objects and arrays

### Instance `add_reference`

Object/array **value** `add_reference` is `add_reference` on the **instance** (embedded name). **No** second header in `xctx->p`. That second header is the double-wrap `create_unmanaged_object` / `create_unmanaged_array` still do.

Arrays **match objects** for lifetime. Memory arrays today ignore create options; `release` is a no-op; no child pool. **`create_array` is on the #2 on-ramp.** We wanted this during #17; it was deferred as too much for that patch.

### Create starts at 0

Create / wrap leaves count **0**. **`add_reference` is always “caller now owns one hold.”** Script assign is that call. Functions still return a pointer. C that keeps an object `add_reference`s itself. Forgotten create lives until xctx destroy (one small child pool if managed). Fine as the safety net.

### Property values — generic objects

Whoever **sets** a property must make that value last **at least as long as the object**. Ways:

- Already longer-lived (permanent, scalar wrapper in `xctx->p`, another managed instance).
- Allocated in **the object’s pool**.
- Const objects have **no pool**; stored values must already last.
- A given implementation **may** copy into its pool on `set`. Memory objects today just store the pointer.

Generic memory objects **do not** walk properties on `release`. Adapter / runtime / compiled literals are not “script owns these values.”

### Script-evaluation-aware wrapper

**Why wrap exists:** object/array **literals belong to the compiled value**, not this evaluation. Programmers still **set** properties and elements. Mutating the compiled instance poisons every later eval. The face is this evaluation’s **overlay**.

**Invariant we want:** script cannot change stuff **outside** the script. Literals, `retrieve_objects*` / `get_object*` (today `impl_script_face_object` → memory `create_wrapper_unmanaged`), `isolate_mutable_default`, and other script-mutable results should be (or become) this wrapper. If script can mutate a qualified-variable object that is **not** a face, we **missed a wrap** — not a second lifetime model. Same check for `object()` / `create_array` / `clone` returning an unwrapped instance.

**Unmanaged face (literals, retrieve today):**

- Create: count 0, **borrow** the base. `wrap_literal_*` returns a pointer; does not `add_reference`.
- Born in the **eval / scope pool** (natural: this evaluation’s overlay).
- First `add_reference` (**0 → 1**): take **one** hold on the **wrapped** instance (permanent-like compiled base: that hold is a no-op). Later `add_reference`s only bump the **face**.
- `release`s must match. When the face returns to **0**, `release` the wrapped (the hold from 0 → 1). The face instance still does not destroy itself.
- Managed vs unmanaged **of the face** means the same as any object: managed expects a `release` for its life; unmanaged only for `add_reference`s taken.

**Escaping an unmanaged face can keep the whole eval/scope pool** (like a **closure** holding the scope). We accept that. We do **not** pay a **child pool per literal** to avoid it. Loop of `o = {}` in **one** long scope can grow that pool because unmanaged zero does not free the old face — watch it; optional `free` later if it hurts.

**Compiled base:** should **act permanent** for holds (`add_reference` / `release` no-ops) so wrapping does not pin the whole compiled unit. Tree today: unmanaged in `parser->p` (compiled-value pool); arrays `set_immutable`; object literals not frozen in that parse path. **Everything in the compiled unit should be immutable regardless.** True process-permanent intern is optional later.

**#17 vs #2:** look-through isolation is not the hold protocol. Same face implementation can carry **both**: isolation (landed) and overlay holds (this story). We can keep using the memory wrapper or split a **dedicated script-aware impl** (`.c` location TBD: `object/` / `array/`, `compile/`, or `function/` as **source home**, not as a built-in function). Retrieve can keep the memory wrapper until we switch.

**Overlay `set` (sharp 4):** the wrapper’s `set` does `release` of the **stored overlay** pointer (not look-through get of the compiled base), `add_reference` of the new value, then store. Loop `o.x = i` can reuse. Exact get-local mechanics wait for implementation.

**Managed face** remains available for C that wants a face with its own pool (parent `xctx->p`) and a matching `release`.

### Runtime / env / threads

Same protocol, odd implementations. Many runtime objects are grab-and-read, often **unmanaged** (or permanent-like). A built-in that **creates** a runtime object is another object implementation: instance in the `p` passed to create (often the scope pool). Properties may be computed or copied into that instance’s pool. **Threads:** more than one request can look at the same env object — access, not a new lifetime kind. Prefer read-only or copy-on-get into the caller’s `p`.

This pass is specified with **`xctx->p` as the usual ancestor**. Adapter caches, compiled units kept across requests, `cede_p` whose parent is not `xctx->p` still **behave** like managed/unmanaged in whatever pool they have. Not a separate #2 model.

---

## What the tree does today (so we do not lie)

| Area | Today (after #235) | Notes |
|------|-------------------|--------|
| Memory objects | Managed/unmanaged/`cede_p`; dual-face inf matches | Generic store-as-is; faces overlay-`slot_store` |
| Memory arrays | Options honored; `get_reference`/`release`; script constructors are faces | Remaining-element walk is pool cleanup |
| Value create object/array | Dual face preferred (`as_value`); extra header still exists | Unmanaged object/array values hold the instance |
| `wrap_literal_*` | Unmanaged memory face | 0→1 holds base; overlay walk on pool cleanup |
| Adapter get/retrieve | `impl_script_face_object` → unmanaged memory face | Memory face is the script-aware stand-in |
| Assign | `slot_store` | Scope last-`release` walks slots then pool |
| `script_result` | xctx pointer, save/restore | Still not a real frame slot |
| `for` clone | Per-slot `add_reference` | C-style `for` clone can drop instance RC to 0 while live — do not walk overlay on that event |
| Pools | General APR + heap/tracker | First-fit later |

---

## Tweaks, not new models

- Missed wrap on some C `create_unmanaged_object` returns (worry list). `create_array`, `clone()`, get/retrieve materialize use dual face / `as_value`. Object-literal `{ get: function()… }` store-time bind landed with **#35**.
- `.c` home for a dedicated script wrapper; retrieve still on the memory face until switch.
- Forgotten C create until xctx destroy; unmanaged face pinning a scope (like closures).
- Dead unmanaged faces in one long scope on overwrite (optional `free` later).
- Get-local overlay vs look-through inside wrapper `set`.
- Pool rewrite, size-class free lists, interned compile literals as true `permanent_*` infs. Discuss before step 5; start from wrap-APR `4ecc2b3c` on `main`, not current `afw_pool.c`.
- Escape **past** one xctx (env, adapter cache, compiled units reused across requests) — same words, not this pass’s safety net.
- `#35` store-time bind uses the slot protocol (defining scope hold, not hoisting).
- Nested **`compile()`** assigned to a variable is **unevaluated** (the graph). It does not run and does not touch `script_result`. `evaluate()` of that value (and model-adapter `on*` compiled at model load, run later on a request) goes through `compiled_value` evaluate, which already save/restores `script_result` / active / written and pushes a NULL scope-stack sentinel. The donated-return list on xctx is **not** a second running result: pointer-matched hold transfer for a C return until `slot_store` takes it. Permanents are no-op holds if the same singleton is stored while a donate is pending. Model `current::useDefaultProcessing` is an **unmanaged null sentinel** (pointer identity, not `afw_value_null`); do not box unmanaged null.
- Wrap-APR vs develop pool: request/object vs **script scope** (APR child per block/`for-of` clone is expensive). Parked until we design; may be our own bump/region for scopes. `create_unmanaged_object` / `_array` stay for scalars / data-type create.

---

## Implementation order (when we start)

Do **not** treat this as a commit plan. When we execute:

1. **Slot protocol** — landed.
2. **Scalar `add_reference`** — landed (box in `xctx->p`; no bindings rename; no first-fit free of boxes). Unmanaged **null** not boxed (`useDefaultProcessing`).
3. **Object/array instance holds** — landed for memory arrays (`get_reference`/`release`, options, wrapper pin). `create_generic` unmanaged. `create_array` dual face / script wrapper.
4. **Overlay `set`** — landed on **faces** and array elements. Generic objects store as-is. `as_value` is the instance→value door. `clone()` is a deep independent graph; script clones without an entity path get a face.
5. **Pools** — **landed** on `issue-2-pool-heap`.
6. **Closures / throw rewind (#35)** — store-time bind; nested assign walks to defining depth; throw-path tests pin original error (not leftover scopes).
7. **Wrapper teardown + create-at-0** — landed on `issue-2-script-wrapper-holds`. Script-mutable creates (`add_properties` no target, object construct/expression, `create_array`, `array()`) return a face. Unmanaged object/array values `add_reference`/`release` the instance. Overlay/element walk is pool cleanup. Closure create starts at 0; 0→1 pins the defining scope. Gate: `language/script/wrapper_property_holds.as`.

Current pools the whole way through 1–4.

---

## Decisions (this session)

| # | Decision |
|---|----------|
| 1 | Scope slot walk on **last `release`**, not `}`. Closures keep the scope. |
| 2 | Functions almost never hold/release. Temps = scope pool. Assign in compiler-internal. Generic objects/arrays store pointers; values last ≥ object. |
| 3 | Create/wrap at **0**. Forgotten C create until xctx destroy. |
| 4 | Scalar `add_reference` **copies** into a **managed** wrapper in **`xctx->p`**. Never pin the donor pool (compiled unit, helper). |
| 5 | Object/array `add_reference` = hold the **instance**. No second header. |
| 6 | Arrays match objects. `create_array` on the on-ramp. |
| 7 | Unmanaged face: borrow at create; **0→1** holds wrapped; back to 0 releases wrapped. |
| 8 | Read = pointer. Assign to a slot (variable, parameter, hidden result) = `add_reference`. `return` sets this frame’s hidden slot and leaves. Assign to **caller when the block ends**, then callee walk. |
| 9 | Runtime/env: same protocol, often unmanaged grab-and-read. Threads = access. |
| 10 | #17 is isolation; #2 is holds. Same face can do both. Script-aware wrapper allowed. |
| S1 | Scalar wrapper uses **`p->managed_p`** (heap during eval). |
| S2 | `for` clone = per-slot `add_reference`. Hidden result not copied. |
| S3 | Script literals = unmanaged faces in **eval/scope pool** (eval vs compile). Do not add a child pool per `{}`. Compiled unit **immutable**; holds on the base are no-ops. |
| S4 | Overlay overwrite reuses via the **wrapper’s `set`**, not “objects own all properties.” |

**Parked / later:** first-fit / prefixes (P3); interned true-permanent compile literals; huge-string slice optimization; nested compile+evaluate reusing the outer heap.

---

## Related

| Doc | Role |
|-----|------|
| [`memory-management.md`](memory-management.md) | Archaeology, old phases, P3, candidate wrap-APR notes. **Superseded as the campaign map by this file.** |
| [`afw-philosophy-and-core-model.md`](afw-philosophy-and-core-model.md) | Whole-story framing; lifetime paragraph points here |
| [`issue-17-mutable-object-faces.md`](issue-17-mutable-object-faces.md) | Landed look-through faces |
| [`issue-2-property-name-values.md`](issue-2-property-name-values.md) | Names as values (landed); different #2 slice |
| [`c-naming-and-payloads.md`](c-naming-and-payloads.md) | utf8/memory have no pool |
| [`runtime-objects-and-environment.md`](runtime-objects-and-environment.md) | Env as runtime objects |
| [`.cursor/rules/afw-value-memory.mdc`](../.cursor/rules/afw-value-memory.mdc) | **Current tree** work rule; campaign target is this pad |
| Atlas §3 | [`knowledge-atlas.md`](knowledge-atlas.md) |
| RSS / gdb lab | [`src/afw/tests-extra/issue-2/`](../src/afw/tests-extra/issue-2/) — opt-in hard loops for optional `free` |
