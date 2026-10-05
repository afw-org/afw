# Lifetime principles

**Audience:** maintainers / assistants. **Not** handbook.

This is the **value lifetime story**. Inf-method rails, two worlds, eval `p`, and pool doors are details of this story. When a pad, rule, or comment disagrees with this file, **this file wins** until we change it here.

Code and tests remain ground truth. If the tree and this story disagree, fix the tree or this story; do not add a third protocol.

**Related:** inf rails [`issue-2-hold-in-inf.md`](issue-2-hold-in-inf.md) (**implementation order** at the top). Two worlds [`experiment-brainstorm.md`](experiment-brainstorm.md); eval `p` [`experiment-eval-p.md`](experiment-eval-p.md); pool doors [`remaining-apr.md`](remaining-apr.md). Lab: `src/afw/tests-extra/issue-2/01-rss-hard-loops/`. Tree vs this story: [#443](https://github.com/afw-org/afw/issues/443), [#445](https://github.com/afw-org/afw/issues/445), [#446](https://github.com/afw-org/afw/issues/446). n=5 dest-p honor (compile caller does not release) landed [PR #460](https://github.com/afw-org/afw/pull/460) `3fdaabdd`.

---

## Words

Say **reference** for a value or scope lifetime (`get_reference` / `release`).

**Return contract:** **caller does not release** or **caller releases**. Do not name that contract unmanaged, managed, temp, extra-hold, pin, or “expects unmanaged.”

**Inf:** **unmanaged** lives in a `p` and dies with that `p` (no RC). **Managed** needs RC at least 1. **Permanent** has no RC; `release` is a no-op.

**Obsolete for the contract:** extra-hold, extra bump, temp, pin, bridge, dangerous crack, “caller expects unmanaged/managed.” Residual C names (`release_value_at_cleanup`) are leftovers in the tree, not a second protocol.

**Smell:** extra-hold, special case, extra bump, helpers around assign/GET/clone as a leak fix, treating `xctx->p` as an implicit dest, caching dest `p` on the xctx (`script_result_p`). If a leak needs a new flag or a third way to keep a value alive, stop.

---

## Two worlds

| | Unmanaged | Managed |
|---|---|---|
| Where | dest `p` (evaluation `{ }` is `scope->p`) | dest `p->managed_p` |
| Death | that pool bulk-frees | last RC: `release` every reference this value holds, `free_memory` every block it allocated (or last-release a pool it owns) |

**Unmanaged has no references.** An unmanaged instance lives and dies with its pool. Its `get_reference` / `release` never touch a pool's RC. Anything that needs it past that pool calls `get_assignable_value` and gets a managed value. **Legacy still in the tree:** unmanaged memory objects and arrays still pin their pool on `get_reference` for C callers (adapter results, runtime `set_object`, the associative-array object store). That is a #2 follow-up, not a protocol to copy.

Pass dest `p`. Do not treat `xctx->p` as an implicit dest. `afw_xctx_*alloc`/`free` used to allocate in `xctx->p` (an older world where `xctx->p` was `managed_p`). Those macros are **gone** ([#443](https://github.com/afw-org/afw/issues/443) on `fix-443-xctx-alloc-dest-p`): `afw_pool_*` with dest `p` (managed values: `p->managed_p`; env-lifetime: `env->p`; stored on the xctx: `xctx->p`). Do not reintroduce them. `set_property_as_string_from_utf8_z` stores the pointer; the bytes must outlive the object.

---

## Dual face

An object or array **instance** has an `afw_value` as an instance variable. Script sees values. For a managed object/array, **instance RC is the managed lifetime**. The embedded value is not a second counter. Value `get_reference` / `release` last-release **the instance**. Last RC of the instance is the one walk.

Unmanaged instance: value `get_reference` / `release` throw. Use `get_assignable_value`. A separate wrapper around an instance is another managed value that references the instance.

---

## Return contract

Every C function that returns an `afw_value_t *` (or a managed object/array instance) states one of:

| Contract | Meaning | Examples |
|---|---|---|
| **Caller does not release** | Caller must not `release` the result. Result lasts for the lifetime of dest `p`. | Adaptive `execute_*`, `evaluate()`, `afw_compile_*` (value / object / array), unmanaged `create_*` |
| **Caller releases** | Caller must `release` the result. Result lasts until released. That one `release` last-releases everything the value obtained. | `create_managed`, `get_assignable_value`, `get_reference` |

Adaptive built-ins are **caller does not release**. What the function does inside is its business. It must deal with every lifetime it starts so the return matches the contract.

`afw_compile_*` is that contract. Script / template / test_script return a **managed** `compiled_value` so `get_assignable_value` references self; compile registers last-release of the birth hold on dest `p`. JSON / relaxed_json return evaluated data in dest `p`. The caller of compile does not inspect the inf to decide whether to `release`. Inf managed is not the return contract.

---

## How a function honors the contract

| Contract | Pattern |
|---|---|
| Caller does not release | Unmanaged in dest `p`. |
| Caller does not release | Managed, and register last-release of **that one hold** on dest `p` (`afw_pool_release_value_at_cleanup`). |
| Caller releases | Managed at RC 1. Do not register last-release. |
| Caller releases | Do not return unmanaged. |

If the function calls `get_assignable_value` or `create_managed`, it owns a value it must release. It honors that inside by returning it to a caller that **releases**, `release` in the function, or registering last-release of that one hold on dest `p` when the caller **does not release**.

Register last-release of the **returned** value only. The container’s last RC `release`s what it holds.

Evaluate of a compiled value is **caller does not release**. Dest `p` is the `p` passed to that evaluate. If the result is managed, register last-release of that one hold on that dest `p`. `last_statement_non_void_value` is a pointer at this frame’s last non-void statement; it requires `scope->p`. `xctx->script_result` is the managed isolate of the running script result; it does not require the current scope. Isolate dest is dest `p` of the caller that writes the slot (`script_result_set(value, p, xctx)`: `scope->p` at deactivate, `original_scope->p` at clone, evaluate dest `p` for a non-block script-function body). Nested `evaluate(compile())` parks `script_result`. Managed bytes follow that dest `p->managed_p`. There is no dest-pool field on the xctx.

`for_p_lifetime` last-releases on exact dest `p` (evaluate return, script return onto the caller). `for_scope_lifetime` is that after resolving dest to the nearest scope in dest `p`’s pool-parent chain (walk parent, not `managed_p`). Throw if dest `p` is a job heap (`p == p->managed_p`). That throw is a probe. Do not take dest off the current xctx frame.

---

## `get_assignable_value`

Always returns a value the caller of **this method** must release. That result can move: its `get_reference` / `get_assignable_value` / `release` work on other `p` / `scope->p`. How the inf does that is up to the implementation.

Read a slot: the pointer. Keep a value alive: `get_reference` (matching `release`). `slot_store` = `get_assignable_value(incoming)` then `release` the previous occupant then store. A container that takes a must-release hold it already has does not call `get_assignable_value` again (`slot_take`).

`create_managed` is already must-release (RC 1). `get_assignable_value` of it is a **second** must-release. Handle both.

What `get_assignable_value` returns, by world:

| Value | Result |
|---|---|
| Unmanaged (scope temp, unit literal, unmanaged object/array) | A **managed copy** in `p->managed_p`. A managed container copy references its managed members. |
| Managed | The same value, RC bumped. |
| Permanent scalar | Self (`get_reference` is ignored). |
| Permanent object | A managed wrapper, so Adaptive Script can modify a copy (language semantics). Permanent array: a managed copy. |

Compile-unit literals are **unmanaged in the unit** (not permanent): a store copies them, so nothing keeps a pointer into a unit that can die. Literals whose text is a registered constant (`strings.txt` → environment string literals, common integers) are permanent. A top-level object literal is unmanaged in the unit's pool, not an entity with its own pool (only `afw_compile_json_to_object` with `cede_p` makes one).

Script-built containers (object literal with expressions, construct/spread, `add_properties` with no target, `array()`, `create_array()`) are plain unmanaged values in dest `p`. Temporaries die with the scope pool; a store copies them. There are no unmanaged faces.

---

## Last RC

**One walk:** `release` every reference this value holds, `free_memory` every block it allocated, and if it owns a pool, last-release that pool. No per-kind leftover helper. Object, array, compile unit, binding, slice, wrapper, face overlay, and scope `frame_slots` are this walk.

A **managed container** holds **one reference** to each value it holds (object property and array element the same) and `release`s those when it goes. Those values are ordinary managed values. The same value can be held by more than one container. Unmanaged object/array is pointers in dest `p`, bulk-free.

Methods that return a held value are **caller does not release**. If the caller wants that value past dest `p`, they `get_reference` / `get_assignable_value`. The caller does not special-case object vs array or get vs pop. Internals (unlink, leave the reference, `remove` `release`s, `pop` / `shift` last-release dest `p`) are how-to.

---

## Pools, scopes, and cycles

**A release registered on a pool must not keep that pool alive.** "Caller does not release" by registering last-release on dest `p` is sound only if the registered value does not, directly or through what it references, keep dest `p` alive. Otherwise the cleanup waits for the pool and the pool waits for the cleanup. The pool tree is for storage and the end-of-xctx backstop; lifetime between values is references.

**A managed value references everything it points into that has its own lifetime.** A closure binding references its captured scope **and** the compile unit its definition lives in. Do not rely on some pool happening to outlive the pointer.

**Scopes.** A scope lives by its scope RC. It references its lexical parent (scope RC), never dest `p`: the scope pool's parent is `p->managed_p` (the job heap). The scope pool is the frame's temporary memory, freed in one shot at last scope RC; frame slots hold only `get_assignable_value` results. Scopes are special in one way: Adaptive `try` / `catch` / `finally`. While a throw is handled (`error_processing_count > 0`) the last release of a scope pool is delayed until the catching `ENDTRY`, so catch code can still read what the frames allocated. That delay depends only on the scope pool's own count, not on its parent.

**The error owns references** to its data and backtrace (`afw_error_set_data`, `afw_error_release_references`). They move with the error struct (`AFW_ERROR_COPY` / `AFW_ERROR_CLEAR_PARTIAL`) and are released at a caught `ENDTRY`, a new error set, or xctx release.

**Open: closure reference cycles ([#458](https://github.com/afw-org/afw/issues/458)).** A frame slot that holds a closure whose captured scope chain includes that frame is a cycle (frame → slot → binding → captured scope → … → frame). Reference counting cannot reclaim it; the frame lives until xctx teardown. Local helper functions, `const f = function…`, an object in the frame holding a closure, and an inner-block closure stored in an outer slot all do this. Decision pending in #458.

---

## When leftover RC appears

1. Did last RC complete the walk?
2. Was `get_assignable_value` called on a value already must-release, and only one hold released?
3. Did a method that returns a held value expect the caller to `release`?
4. Did a callee honor caller does not release with managed and forget to register last-release on dest `p`, register twice, or use `xctx->p`?
5. Did a callee return unmanaged on a caller-releases contract?
6. Does a release registered on a pool keep that same pool alive (directly, through a scope, or through a child pool's create reference)?
7. Does a frame slot hold a value that references that frame (a closure cycle)?
8. Does a managed value point into memory (a unit, another pool) it does not reference?

Probes that find these are in [`agent-support.md`](agent-support.md) (*Leftover RC / pool never dies*).

If the answer is a new register last-release, a new flag, or a helper around assign, stop.

---

## Using this pad

C sittings make the tree match this file. Until last RC of every managed value completes the walk, a fix in one place can show leftover in another. That is expected. Do not add extra-hold or register last-release on a method that returns a held value to hide it. Re-measure the RSS lab after each vertical. #2 stays open.

Not the #2 scoreboard. Not the RSS lab table. Not a license to rewrite heap free lists.
