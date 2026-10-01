# Lifetime principles

**Audience:** maintainers / assistants. **Not** handbook.

This is the **value lifetime story**. Inf-method rails, two worlds, eval `p`, and pool doors are details of this story. When a pad, rule, or comment disagrees with this file, **this file wins** until we change it here.

Code and tests remain ground truth. If the tree and this story disagree, fix the tree or this story; do not add a third protocol.

**Related:** inf rails [`issue-2-hold-in-inf.md`](issue-2-hold-in-inf.md); two worlds [`experiment-brainstorm.md`](experiment-brainstorm.md) ([#277](https://github.com/afw-org/afw/issues/277)); eval `p` [`experiment-eval-p.md`](experiment-eval-p.md) ([PR #287](https://github.com/afw-org/afw/pull/287)); pool doors [`remaining-apr.md`](remaining-apr.md). History: [`issue-2-lifetime.md`](issue-2-lifetime.md), [`memory-management.md`](memory-management.md). Lab: `src/afw/tests-extra/issue-2/01-rss-hard-loops/`.

---

## Words

Say **reference** for a value or scope lifetime (`get_reference` / `release`).

**Return contract** (what the caller does with the result):

- **Caller does not release**
- **Caller releases**

Do not name that contract unmanaged, managed, temp, extra-hold, pin, or “expects unmanaged.” Those mix inf with contract and cause bad fixes.

**Inf** (how the value is built) stays:

- **Unmanaged** — lives in a `p`, dies with that `p`, no RC
- **Managed** — RC at least 1
- **Permanent** — no RC, process/env lifetime

**Obsolete for new writing** (older pads, comments, and this sitting’s drafts used them for the return contract): extra-hold, extra bump, temp (as the name of the contract), pin (as the name of the contract), bridge, dangerous crack, “caller expects unmanaged/managed.” Residual C names (`release_value_at_cleanup`, `object_hold`, `is_root`) are leftovers in the tree, not a second protocol.

**Smell:** extra-hold, special case, extra bump, `is_root`, helpers *around* assign, GET, or clone as a leak fix. If a leak needs a new flag or a third way to keep a value alive, stop.

---

## Two worlds

| | Unmanaged | Managed |
|---|---|---|
| Where | dest `p` (evaluation `{ }` is `scope->p`) | dest `p->managed_p` |
| Death | that pool bulk-frees | last RC: last-release every occupant and reference this value obtained, then `free_memory` every block it allocated, via the stored p |
| Role | values that live in dest `p`; compile-unit payloads; snapshots | anything a slot, managed property, or managed element **holds** |

Pass dest `p`. Unmanaged values live in `p`. Managed create / clone / promote use `p->managed_p`. Do not treat `xctx->p` as an implicit dest.

Last RC is not “free the header.” A managed string’s one block may be header plus bytes. A managed object’s blocks are the header, property entries, name index, and anything else it allocated. A compile unit last-releases the **pool** it owns; that pool bulk-frees the unit. Same rule: last RC drops everything this value obtained.

**Permanent** values (process or env lifetime) are a third inf policy: no RC; `get_reference` is as-is.

---

## Dual face

An object or array **instance** has an `afw_value` as an instance variable (the dual face). Script sees values.

- Managed instance: value `get_reference` / `release` bump and last-release **the instance**. Last RC of the instance last-releases occupants, names, and every block that instance allocated.
- Unmanaged instance: value `get_reference` / `release` **throw**. Isolate with `get_assignable_value`. Instance `get_reference` / `release` still pin `object->p` / `array->p`. The instance still dies with its pool if nothing else references it.

A heap wrapper around an instance is itself a managed value. Last RC of the wrapper last-releases the instance **and** `free_memory`s the wrapper. (The tree still fails this for `afw_value_object_create_managed`: wrapper RC starts at 0 and last-release at 0 returns without `free_memory`.)

---

## What a caller does

| Intent | Call |
|--------|------|
| Read a slot | the pointer |
| Keep this value alive | `get_reference` (matching `release`). `xctx` only. |
| Fill a slot (assign, param, overlay, a result that must move to another scope) | `get_assignable_value` / `slot_store`, dest `p` |

`get_assignable_value` exists so a value can move to a different scope with RC correct. Managed bump self. Unmanaged scalar promote into `p->managed_p`. Unmanaged object/array: already-managed dual-face bump; generic memory not a wrapper → `clone_managed`; else managed look-through wrapper. Permanent scalar as-is. Graph infs evaluate first.

`slot_store` = `get_assignable_value(incoming)` then `release` the previous occupant then store. Same-pointer skip stays.

---

## Birth

`create_managed` starts at RC 1. The creator owns that hold.

- **`slot_take`** (`set_property_take` / `push_value_take`): the slot takes the birth hold. No `get_assignable` bump.
- **`slot_store`**: isolate incoming, last-release the previous occupant, store.
- `get_assignable_value` of a value you **just** `create_managed` is a second own. Scope cleanup of that second own leaves RC 1 (splice **#405**).

Two jobs, two calls:

1. You created it (`create_managed`, RC 1): you own the birth.
2. You did not create it: `get_assignable_value` (bump or promote) if you need a hold you can give away.

Do not mix them (`get_assignable_value` of a fresh create is a second own).

---

## Return contract: does the caller release?

Do not describe a return as managed or unmanaged. That is the **inf** of the value, not what the caller does.

Every C function that returns an `afw_value_t *` (or a managed object/array instance) has one of two contracts:

| Contract | Meaning | Typical callers |
|---|---|---|
| **Caller does not release** | Caller must not `release` the result. | Adaptive `execute_*`, `evaluate()`, unmanaged `create_*` |
| **Caller releases** | Caller owns RC 1 and must `release`. | `create_managed`, `compile()` of a unit |

Adaptive built-ins are **caller does not release**. That is why `scope->p` exists.

**Inf** (how the value is built) is separate:

- Unmanaged lives in a `p` and dies with that `p`. No RC.
- Managed needs RC at least 1. Last RC last-releases what it obtained and `free_memory`s what it allocated.

**How the callee honors the contract**

- **Caller does not release** + unmanaged: create in dest `p`. Done.
- **Caller does not release** + managed: return managed **and** register last-release of **that one hold** on dest `p` (`afw_pool_release_value_at_cleanup`). The caller never `release`s. A later `get_assignable_value` (assign, param) can add a reference so the value outlives that registered last-release.
- **Caller releases** + managed: return managed at RC 1. Do not register last-release. The caller `release`s.
- **Caller releases** + unmanaged: do not do this. Unmanaged has no `release`.

The register lives where the managed value is created and returned. Nested occupants stay on the container’s last-RC walk. Do not register last-release on GET of an occupant the container still owns. Do not register twice.

**Evaluate of a compiled value** is **caller does not release**, dest `p` = the `p` passed to evaluate. If the result is managed, register last-release on that dest `p`. Do not isolate the result into `xctx->p`. `xctx->p` is the job heap for internals (evaluation stack, and similar). `xctx->script_result` is an internal slot for nested evaluate: save, restore, unwind. Nested evaluate deals with that slot as the stack unwinds. It is not the dest for the value the caller of evaluate receives.

The tree still `slot_store`s `script_result` with dest `xctx->p`. That is a hole against this rule, not a second dest.

---

## Last RC of any managed value

Last RC always does both of these, as applicable:

1. **Last-release** every occupant and every `get_reference` / `get_assignable_value` this value did in order to keep something.
2. **`free_memory`** every block this value allocated (header, trailing bytes, property entries, name index, element vector, wrapper allocation, binding header, …) via the stored p. If it owns a pool, last-release that pool (the pool bulk-frees what was allocated there).

A managed **container** (object, array, and anything later that holds children) is that rule with occupants. Object and array are the same:

- Occupants it `slot_store`d or `slot_take`d (properties, elements)
- Property names it `get_assignable`d (once, when the name is minted)
- A wrapped instance, if it has one
- Every block listed above

GET of an occupant is **identity** while the container still holds it. Keeping that occupant after the container dies is `get_assignable_value` into the dest slot. `pop` / `shift` transfer: the array no longer owns the occupant. Contract is caller does not release; inf is managed; register last-release of that hold on dest `p`. `remove` last-releases the occupant the array still owned.

**`compiled_value`** owns a **pool**. Script / template / test_script compile returns it managed (RC 1). Last RC last-releases that compile pool; everything allocated in the unit dies with the pool. Compile does not register the unit as a temp. Evaluate does not last-release the unit. Evaluate is caller does not release, dest `p`: if the result is managed, register last-release on dest `p`. Closures from that unit keep the unit alive through the binding.

**Closure binding** is minted at RC 0; the first `get_reference` (slot or overlay) pins the enclosing scope. Last RC last-releases that scope, last-releases a kept compile unit if present, and `free_memory`s the binding. That is the container rule. Birth at 0 is because the binding exists before a slot owns it (`o.fn = function…`).

**Managed slice** (`utf8` / `memory`): last RC last-releases the containing value and `free_memory`s the slice header.

If a later managed kind holds children, it follows this same last-RC walk. No per-kind leftover helper.

---

## Clone (Adaptive `clone()` of object/array)

Structural copy: `create_managed` the tree. Nested containers are `create_managed` then **take** into the parent (the parent owns those births). Nested scalars `get_assignable` of the source then take. Copy meta.

The value `clone()` **returns** is the root. Contract is caller does not release; inf is managed; register last-release of that one hold on dest `p`, at the execute result. Nested occupants are slots of the root. Recurse does not register last-release. GET of `clone(o).child` / `clone(o).arr` is identity. Assign of that occupant is `get_assignable_value` into the dest slot. Last RC of the root last-releases nested (occupants, names, and every block those nested containers allocated).

Product tests: `src/afw/tests/additional_test_scripts/clone.as`, `src/afw/tests/language/script/nested_occupant_share.as`. The recurse `is_root` flag in the tree is leftover naming of “this is the execute result.” Pin at `execute_clone` after the copy; drop the flag when touching that code.

---

## When leftover RC appears

Work the story, in this order:

1. Did last RC last-release every occupant and reference, and `free_memory` every block this value allocated (or last-release a pool it owns)?
2. Was `get_assignable_value` called on a fresh `create_managed` (second own)?
3. Was GET treated as isolate (the container still owns the occupant)?
4. Did a callee honor **caller does not release** with managed and forget to register last-release on dest `p`? Or register twice, or on the wrong `p` (including `xctx->p` when dest `p` is the caller)?
5. Did a callee return unmanaged on a **caller releases** contract?

If the answer is a new pin, a new flag, or a helper around assign, stop and ask.

---

## Holes the tree still has (use this list; do not invent pins)

These fail the story. A leak sitting or a full review starts here.

- **Heap wrapper last RC** (`afw_value_object_create_managed` and the array twin): wrapper RC starts at 0; last-release at 0 returns without `free_memory` of the wrapper. Last RC must last-release the instance **and** free the wrapper allocation.
- **Unused Adaptive `clone()` of nested object properties:** `clone({ child: { x: 1 } })` leftover; `clone({ a: 1 })` and `clone([{ n: 1 }])` flat. Parent last-release of the nested occupant runs at RC 1. Something that nested object obtained is not on the last-RC walk, or a side allocation is not `free_memory`d. Lab: `clone_nested_*`. Do not pin GET of `.child`.
- **`is_root` on clone recurse:** pin belongs on the execute result only.
- **`afw_xctx_malloc` for managed wrappers:** dest is `xctx->p`, not dest `p->managed_p`. Conflicts with “do not treat `xctx->p` as implicit dest.”
- **`script_result` dest `p`:** `afw_xctx_script_result_set_value` `slot_store`s with dest `xctx->p`. Evaluate’s result is a temp in the caller’s dest `p`. `script_result` is internal nested-evaluate unwind, not the job-heap dest for that value.

A full AFW review is: every `create_managed` / `get_reference` / `get_assignable_value` / `slot_store` / `slot_take` / `optional_release` / `release_value_at_cleanup` site against this story. Pool bulk-free is the unmanaged world. Dual face couples value RC and instance RC; reviewing only one misses leftover.

---

## What this file is not

Not the #2 scoreboard (that stays [`issue-2-hold-in-inf.md`](issue-2-hold-in-inf.md) live status). Not the RSS lab table. Not a license to rewrite heap free lists. Not a second `get_reference` that clones.
