# experiment/brainstorm — managed in `xctx->p`, unmanaged in dest `p`

**Branch only.** Do not treat this as `develop` truth until the experiment lands. Do not copy into atlas / `AGENTS.md` / `#2` pads from here.

**Purpose:** split value lifetime into two worlds, then clone **out** of evaluation.

## Two worlds (inside one evaluation)

| | Unmanaged | Managed |
|---|---|---|
| Where | caller `p` / `scope->p` (tracker) | this `xctx->p` (ST heap) |
| Death | pool bulk-free | last RC, then optional free |
| Role | temps, compile unit, snapshots | anything a slot / managed property / managed element **holds** |

- `get_reference` / `get_assignable_value` on managed scalars: **bump self**. Dest-pool copies do not belong here.
- Unmanaged scalar `get_assignable_value`: **promote** (`create_managed` in this `xctx->p`, RC 1).
- `afw_pool_release_value_at_cleanup`: extra **pin** that is not a slot. Create managed (caller owns RC 1), register that 1 on a pool (often `scope->p`). Extra holds keep it after that pool dies.

Adaptive function case: create a managed value (later: managed object/array), pin it on `x->p`, return or stash it; caller `get_assignable_value` keeps it. Headers stay in `xctx->p`, not in the dying tracker.

## Eval completion (clone out)

When an **evaluation is done**, the value that *leaves* is an **unmanaged clone in dest `p`**. Inside the eval, managed stays in this `xctx->p`.

Dest `p` is often this `xctx->p` or a tracker under it — then the result is unmanaged in a pool this xctx already owns; teardown is bulk-free.

## Foreign heap (clone in or out)

Clone when the bytes live in (or must live in) a heap that is **not this xctx’s heap or a child of it** — especially a **multithreaded** pool (`adapter->p`, `env->p`, `server->p`).

- Keep an eval result on the adapter: clone into `adapter->p` (request xctx may then die).
- Hand an mt-pool object into this eval/request as something we **keep**: clone into a pool this xctx owns.
- Look-through for the duration of a call, no stored pointer in a frame/managed bag: may not need a clone.

Adapter get/retrieve is not a blanket clone; only when the value is or must become foreign.

`get_reference` is a hold. Clone-out is eval completion (and foreign-heap I/O), not every hold.

## Later (not this vertical)

Managed **object/array as containers** (like frames): own pool under `xctx->p`, `slot_store` / `get_assignable_value` on set, last release of the bag releases properties/elements then the bag. Unmanaged bags stay “store the pointer, pool bulk-frees,” own pool or caller `p`. Dual-face `value.inf` must match (pool-owned → unmanaged face).

Today’s `afw_value_object_create_managed` / array is a **value wrapper** around some other bag. That is not “managed object as a frame.” Keep the names distinct.

Registries, runtime objects, flags: permanent or `env->p`. Not “managed in request `xctx->p`.”

## What is already on this branch

- `afw_pool_release_value_at_cleanup`
- Managed scalar (and slice) create: no `p`, alloc in `xctx->p`, RC 1, caller releases; `get_reference` bumps
- Pool-owned dual-faces that were wrongly `managed_object_inf` (aggregate, const key/value, meta accessor; embedded follows embedder)

## Known: leak vs throw

Tracker bulk-free used to hide unmanaged (and old managed-in-`p`) headers. Moving create to `xctx->p` without matching last-release **throws** (`free_memory`: pool does not match allocation) when create `xctx->p` ≠ teardown `xctx->p` (CLI eval xctx vs command `self->xctx`; journal `beginTime` is the probe). Empty `is_not_specified` is a dying `xctx->error`, not `#272`.

The leak (no optional free until the allocating heap is trusted) is accepted until scalars and eval-completion clone-out are honest. Do not put dest `p` back on the managed header as a lifetime story.

## Order

1. Managed scalars working (assign-to-slot, pin-on-scope; last-release must not throw).
2. Eval-completion / foreign-heap clone-out as the escape from this `xctx->p`.
3. Managed object/array as frames.
