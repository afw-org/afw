# Lifetime principles

**Audience:** maintainers / assistants. **Not** handbook.

This is the **value lifetime story**. Inf-method rails, two worlds, eval `p`, and pool doors are details of this story. When a pad, rule, or comment disagrees with this file, **this file wins** until we change it here.

Code and tests remain ground truth. If the tree and this story disagree, fix the tree or this story; do not add a third protocol.

**Related:** inf rails [`issue-2-hold-in-inf.md`](issue-2-hold-in-inf.md); two worlds [`experiment-brainstorm.md`](experiment-brainstorm.md); eval `p` [`experiment-eval-p.md`](experiment-eval-p.md); pool doors [`remaining-apr.md`](remaining-apr.md). Lab: `src/afw/tests-extra/issue-2/01-rss-hard-loops/`. Tree vs this story: [#443](https://github.com/afw-org/afw/issues/443) (`afw_xctx_*alloc`), [#445](https://github.com/afw-org/afw/issues/445) (last RC / drop `is_root`), [#446](https://github.com/afw-org/afw/issues/446) (`script_result` dest `p`).

---

## Words

Say **reference** for a value or scope lifetime (`get_reference` / `release`).

**Return contract:** **caller does not release** or **caller releases**. Do not name that contract unmanaged, managed, temp, extra-hold, pin, or “expects unmanaged.”

**Inf:** **unmanaged** lives in a `p` and dies with that `p` (no RC). **Managed** needs RC at least 1. **Permanent** has no RC; `release` is a no-op.

**Obsolete for the contract:** extra-hold, extra bump, temp, pin, bridge, dangerous crack, “caller expects unmanaged/managed.” Residual C names (`release_value_at_cleanup`, `object_hold`, `is_root`) are leftovers in the tree, not a second protocol.

**Smell:** extra-hold, special case, extra bump, `is_root`, helpers around assign/GET/clone as a leak fix, **`afw_xctx_*alloc`/`free` for a value**. If a leak needs a new flag or a third way to keep a value alive, stop.

---

## Two worlds

| | Unmanaged | Managed |
|---|---|---|
| Where | dest `p` (evaluation `{ }` is `scope->p`) | dest `p->managed_p` |
| Death | that pool bulk-frees | last RC: `release` every reference this value holds, `free_memory` every block it allocated (or last-release a pool it owns) |

Pass dest `p`. Do not treat `xctx->p` as an implicit dest. `afw_xctx_*alloc`/`free` allocate in `xctx->p`; that matched an older world where `xctx->p` was `managed_p`. Never use them for a value. Job-scoped internals (evaluation stack) may stay on `xctx->p` until those macros retire ([#443](https://github.com/afw-org/afw/issues/443)).

---

## Dual face

An object or array **instance** has an `afw_value` as an instance variable. Script sees values. For a managed object/array, **instance RC is the managed lifetime**. The embedded value is not a second counter. Value `get_reference` / `release` last-release **the instance**. Last RC of the instance is the one walk.

Unmanaged instance: value `get_reference` / `release` throw. Use `get_assignable_value`. A separate wrapper around an instance is another managed value that references the instance.

---

## Return contract

Every C function that returns an `afw_value_t *` (or a managed object/array instance) states one of:

| Contract | Meaning | Examples |
|---|---|---|
| **Caller does not release** | Caller must not `release` the result. | Adaptive `execute_*`, `evaluate()`, unmanaged `create_*` |
| **Caller releases** | Caller must `release` the result. That one `release` last-releases everything the value obtained. | `create_managed`, `compile()` of a unit |

Adaptive built-ins are **caller does not release**. What the function does inside is its business. It must deal with every lifetime it starts so the return matches the contract.

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

---

## `get_assignable_value`

Always returns a value the caller of **this method** must release. That result can move: its `get_reference` / `get_assignable_value` / `release` work on other `p` / `scope->p`. How the inf does that is up to the implementation.

Read a slot: the pointer. Keep a value alive: `get_reference` (matching `release`). `slot_store` = `get_assignable_value(incoming)` then `release` the previous occupant then store. A container that takes a must-release hold it already has does not call `get_assignable_value` again (`slot_take`).

`create_managed` is already must-release (RC 1). `get_assignable_value` of it is a **second** must-release. Handle both.

---

## Last RC

**One walk:** `release` every reference this value holds, `free_memory` every block it allocated, and if it owns a pool, last-release that pool. No per-kind leftover helper. Object, array, compile unit, binding, slice, wrapper, face overlay, and scope `frame_slots` are this walk.

A **managed container** holds **one reference** to each value it holds (object property and array element the same) and `release`s those when it goes. Those values are ordinary managed values. The same value can be held by more than one container. Unmanaged object/array is pointers in dest `p`, bulk-free.

Methods that return a held value are **caller does not release**. If the caller wants that value past dest `p`, they `get_reference` / `get_assignable_value`. The caller does not special-case object vs array or get vs pop. Internals (unlink, leave the reference, `remove` `release`s) are how-to.

---

## When leftover RC appears

1. Did last RC complete the walk?
2. Was `get_assignable_value` called on a value already must-release, and only one hold released?
3. Did a method that returns a held value expect the caller to `release`?
4. Did a callee honor caller does not release with managed and forget to register last-release on dest `p`, register twice, or use `xctx->p`?
5. Did a callee return unmanaged on a caller-releases contract?

If the answer is a new register last-release, a new flag, or a helper around assign, stop.

---

## Using this pad

C sittings make the tree match this file. Until last RC of every managed value completes the walk, a fix in one place can show leftover in another. That is expected. Do not add extra-hold, `is_root`, or register last-release on a method that returns a held value to hide it. Re-measure the RSS lab after each vertical. #2 stays open.

Not the #2 scoreboard. Not the RSS lab table. Not a license to rewrite heap free lists.
