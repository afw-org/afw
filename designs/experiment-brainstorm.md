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

Registries, runtime objects, flags: permanent or `env->p`. Not “managed in request `xctx->p`.”

Eval-completion / foreign-heap **clone-out** is still the escape from this `xctx->p` (unmanaged clone in dest `p`). Do not retarget Adaptive `clone()` yet.

`afw_value_object_create_managed` / array (value wrapper around some other bag) is not the same as **`memory_managed` frames** below. Keep the names distinct.

## What is already on this branch

- `afw_pool_release_value_at_cleanup`
- Managed scalar (and slice) create: no `p`, alloc in `xctx->p`, RC 1, caller releases; `get_reference` bumps
- Pool-owned dual-faces that were wrongly `managed_object_inf` (aggregate, const key/value, meta accessor; embedded follows embedder)
- Per-type `clone_value_unmanaged` / `clone_value_managed` on `afw_data_type_t`; public `afw_value_clone_unmanaged` / `afw_value_clone_managed`
- CLI print uses the eval `xctx` (`3967854c`)
- **Managed object/array frames** (`memory_managed` inf, uncommitted as of this note): see below

## Three lifetimes (the click)

Managed frames **do not have a pool**. They live in this `xctx->p` and **manage their own bytes**: slots, RC, last-release of names/values/elements then `free_memory` of the header. No child pool.

Unmanaged bags are the pool world, two flavors:

| | Lives in a pool | Has its own pool |
|---|---|---|
| Create | `unmanaged` option (`create_in_pool`) | old `managed` / `managed_cede_p` (`and_pool_create`) |
| Death | that pool bulk-frees | object `get_reference` / `release` control **the pool** |

The old option name **`managed` means “owns a pool.”** The new inf **`memory_managed` means “no pool; the bag is the manager.”** Do not mix them.

Better names for the create options (not renamed yet): `OPTION_managed` ≈ **new child `p`**, `OPTION_managed_cede_p` ≈ **cede this `p`**. Hold is not in the option; it is the inf.

**Value `get_reference` / `release`:** only permanent (as-is) and `memory_managed` (bump / last-release). Unmanaged object/array **value** infs throw, like scalars. Isolate with `get_assignable_value`. To drop an own-pool bag, `afw_pool_release` that pool. Object/array **interface** `get_reference` is still a pin for now (adapters, faces).

Tripwire is in: `afwdev test -j` → **166 failed**, almost all `release of unmanaged object/array`. Cause: `object_hold` / wrapper `get_assignable` returns the **unmanaged dual-face**, so slots store an unmanaged value inf and later `optional_release` throws. Next: isolate must leave **assignable or managed** inf in the slot (not unmanaged). Eval clone-out is still later; this is the slot protocol. Better option names (`new_p` / `cede_p`) not renamed yet.

## Managed object/array frames

Separate inf (`memory_managed`), alloc in `xctx->p`, RC 1. Unmanaged creates unchanged.

**Slots**

- New property: name is `get_assignable_value` **once** (names do not change). Value is `slot_store`.
- Replace: name stays; only the value is `slot_store`d.
- Array push/set/insert: `slot_store` the element.
- Last bag release: release remaining names (objects) / elements, then `free_memory` the header.

Do not `slot_store` the name — that API is “replace an occupant.” The name slot is empty on calloc.

**Unmanaged `get_assignable_value` (object/array value)**

| Internal | Result |
|---|---|
| already `memory_managed` | bump; return the occupant’s dual-face (`obj->value` / `array->value`) |
| generic `"memory"` bag, not a wrapper | `clone_managed` (deep copy into a new frame) |
| view / wrapper / meta / other | `object_hold` / `array_hold` (overlay) |

Typed `map()` boxes the occupant in a reusable unmanaged value. If that internal is already managed, **return the dual-face**. `object_hold` would mint a **new face**, so `o.n = 99` would miss `def[0]`.

**Deep `create_managed_from`**

- Nested objects: `create_managed_embedded` + recurse (path/id compose). Nested arrays: `create_managed_from`. Scalars: dest `set_property` promotes.
- Sideband `object_uri` / `id` / `object_type_uri`: utf8-clone into `xctx->p`.
- Meta delta (parentPaths, reconcilable, …): copy properties onto a fresh delta. **`afw_object_meta_clone_and_set` throws** if the instance is `memory_managed`.
- Walk `first_property` only when the inf is this memory bag (not meta, not wrapper). Anything else: `get_next_property`.
- Stop a `first_property` walk if a name is not an evaluated string. `qualifier("current")` snapshots have been seen with a garbage tail (`impl_afw_pool_inf` as a name). Open: why the list is not NULL-terminated; do not quietly drop this.

**Wrong paths**

- Treat a view/meta as a memory name/value list (`first_property` is garbage).
- `meta_clone_and_set` onto a managed instance.
- `object_hold` of a managed occupant that is only boxed in an unmanaged value.
- Clone-out on every `afw_value_evaluate` (wrong gate; host boundary / foreign heap).
- Inner `release` of the source after a clone that was not a full graph copy.

**Verified (Mike):** `./afwdev build --fulldev`, `afwdev test -j`, and `afwdev test -j --env-mode valgrind` all green after the frames + name-slot + dual-face get_assignable work. `--cdev --scan` clean (managed `remove_all_values` must not `APR_RING_INIT` a NULL ring).

## Known: leak vs throw

Tracker bulk-free used to hide unmanaged (and old managed-in-`p`) headers. Moving create to `xctx->p` without matching last-release **throws** (`free_memory`: pool does not match allocation) when create `xctx->p` ≠ teardown `xctx->p` (CLI eval xctx vs command `self->xctx`; journal `beginTime` is the probe). Empty `is_not_specified` is a dying `xctx->error`, not `#272`.

The leak (no optional free until the allocating heap is trusted) is accepted until scalars and eval-completion clone-out are honest. Do not put dest `p` back on the managed header as a lifetime story.

## Order

1. Managed scalars working (assign-to-slot, pin-on-scope; last-release must not throw). Leak on last-release still accepted until alloc-pool is trusted.
2. Eval-completion / foreign-heap clone-out as the escape from this `xctx->p`. **Still open.**
3. Managed object/array as frames. Unmanaged **value** `get_reference` / `release` throw for object/array too (same as scalars). Object/array **interface** `get_reference` still pins (next).

Stay an experiment until this managed/unmanaged story is one we still believe. Do not merge as “the new #2 rails” on the strength of scalars alone.

## Parked (other conversation / branch) — object/array meta

Meta on objects/arrays is considered screwed up. A later pass may store **type with the property** (tuple closer to name / type / value) and give the object a pointer to an object-type struct. Do not freeze lifetime work on “property is only name+value” or on today’s meta dual-face / `_meta_` layout. Lifetime can still treat a property **value** as unmanaged-pointer vs slot-held; the tuple can grow a type without changing that.
