# Issue #476 — references and owners

**Audience:** maintainers / assistants. **Not** handbook.  
**GitHub:** [#476](https://github.com/afw-org/afw/issues/476) — open. Child of umbrella [#2](https://github.com/afw-org/afw/issues/2).  
**Related:** [#458](https://github.com/afw-org/afw/issues/458) (reference cycles; closes with step 5), [#343](https://github.com/afw-org/afw/issues/343) (threads, workers), [#342](https://github.com/afw-org/afw/issues/342) (owner heaps).  
**Lifetime story:** [`lifetime-principles.md`](lifetime-principles.md). This pad proposes changes to it; that pad wins until a step lands.

---

## Why

AFW started request based: a request pool, sometimes a subpool for "all of something", thrown away at the end. Adaptive Script compiled to `afw_value` and ran short scripts (mapping adapter properties, …), so pools still worked. #2 is long-running scripts: values must go away when nothing references them.

What is in place works well: a scope's pool (`scope->p`) holds temporary memory and dies with the scope; frame slots and script results hold managed values in `p->managed_p` whose life is a reference count; `get_assignable_value` turns an unmanaged value into a managed one (or references a managed one) for a slot. What is left:

| Problem | Where |
|---|---|
| Reference cycles are never freed (closure, object, indirect, mixed) | #458 body has the measured shapes |
| Reference methods are ad hoc per interface | `afw_value`: `optional_release`, `get_reference`, `get_assignable_value`, inf variable `is_managed`. `afw_object`, `afw_array`, `afw_pool`, `afw_object_associative_array`: `release` + `get_reference` at different inf positions |
| `release` means two things | Counted kinds: drop my reference. `afw_stream`, `afw_writer`, `afw_request`, `afw_adapter_transaction`, …: single owner, destroy |
| Managed value counts are not thread safe | Generated bindings and `afw_array_memory.c` use plain `++`/`--`. A multithreaded pool locks allocation, not a later `get_reference` / `release` |
| Pool workarounds | Parent pins (`afw_pool.c` get_reference / release_common, heap and tracker teardown); scope throw-path last-release delay (`afw_pool_scope.c`, `afw_error.c`) |

## Model

### Two kinds of memory

- **Temporary (pool).** Unchanged. Unmanaged values live in a pool and die with it. A scope's pool is thrown away when the scope deactivates. Request-based code keeps working the old way.
- **Managed (referenced).** Only what escapes a scope: frame slots, script results, values stored in containers, closure bindings. Life is a reference count, plus cycle collection.

Permanent values (compiled into object code, registered constants) are neither: `get_reference` and `release` are no-ops.

### `afw_reference`

A base interface for **counted** things only.

- `get_reference(instance, xctx)` returns a pointer the caller owns one reference to. It may be `self` (count bump) or a copy (unmanaged → managed in the owner). **Always use the returned pointer.**
- `release(instance, xctx)` drops one reference. Last release releases everything the instance owns.
- `for_each_reference` (name TBD) lists exactly the references last release would release. Used by the cycle collector. Debug builds check that listed == released.

Interfaces that are counted extend it: `afw_value`, `afw_object`, `afw_array`, `afw_pool`, scope. The derived inf starts with the base inf, so an instance can be used as `x->ref`:

```c
union { const afw_value_inf_t *inf; afw_value_t pub; afw_reference_t ref; };
```

Single-owner "destroy" interfaces do not extend it. If they want a common base, it gets a different name (for example `afw_disposable`).

`get_assignable_value` folds into `get_reference`. `is_managed` should go away (callers outside an inf should not ask).

**Scalars:** a managed scalar is a count bump (cheaper than allocate + copy); an unmanaged scalar is copied; large strings are never copied. Immutable scalars cannot be part of a cycle, so the collector treats them as leaves.

**Object values:** a value is its inf followed by an internal C type; for an object, a pointer to the `afw_object_t` (see the `afw_data_type_generate` objects). Today a managed object value has its own count and the object has another. The object is the node the collector walks; the value either borrows it or holds one reference. Decide in step 1.

### Scope

Scope stays a kind of pool (`afw_pool_scope.c`), so callees find their scope by walking `p` parents. It becomes an interface that extends `afw_pool` for the unwind behavior (try/catch/finally) and so it can list its frame slots to the collector.

### Owners

`p->managed_p` is **the owner** of managed memory: the job heap for a request, `adapter->p` when evaluating a template in adapter config (so results last as long as the adapter).

Rule: **a managed value belongs to exactly one owner.** Counts are not atomic. The collector runs per owner and its walk stops at anything another owner holds. Values crossing owners are copied, or are read-only once published and borrowed (the owner outlives the borrower, for example config built at startup, by service stop order).

Future (thread work, #343): multithreaded pools go away. Work for an owner (an adapter) is scheduled onto the thread that owns it; `adapter->p` may become `adapter->xctx`. Structs shared across threads use the env mutexes (`AFW_LOCK_BEGIN` / `AFW_LOCK_END`, read/write variants via `xctx->env`) and `afw_atomic_*` for counters. Only where the owner comes from changes; the reference rules do not.

Until then: a managed value in a multithreaded owner must be borrowed only across threads, or get an atomic count variant. S2 decides which. The interim path is deleted when multithreaded pools go away.

### Cycle collection

Trial deletion, synchronous (Bacon & Rajan, *Concurrent Cycle Collection in Reference Counted Systems*, ECOOP 2001; earlier Martínez, Wachenchauzer & Lins 1990, "local mark-scan"). PHP uses it; CPython's `gc` uses the same idea.

1. **Possible roots.** A release that leaves a count above 0 records the instance on a per-owner list. A release that reaches 0 frees normally.
2. **Subtract internal references.** From each root, walk `for_each_reference`; decrement a trial copy of each target's count.
3. **Keep what is still referenced.** Trial count above 0 means an outside reference. Keep it and everything reachable from it; restore their trial counts.
4. **Free the rest.** Each releases what it owns and is freed, without cascading into nodes already being freed (pin all, release owned references, drop pins).

References the walk cannot see (pending dest-p cleanups, the scope stack, C locals) look like outside references, so the walk keeps what they point at. It frees only what plain reference counting would free if the cycle were not there.

Safe points: frame deactivate, end of evaluate, and possibly when the root list passes a threshold (open).

### Simplified principles (target)

1. Three lifetimes: permanent, pool (unmanaged), referenced (managed).
2. One way to keep a value: `get_reference` returns a pointer you own (maybe a copy); `release` gives it back.
3. Each method says caller releases or caller does not release.
4. Every counted thing lists what it owns, exactly what its last release releases.
5. Cycles are freed by the collector. No call-site workarounds.
6. A managed value has one owner.

## Plan

### Experiments (throwaway branches, never merged)

| | Experiment | Gate / question |
|---|---|---|
| **S1** | Hand-written collector for object, closure binding and scope only. Touches `afw_pool_scope.c`; never merges. | `eval_object_rebind` and the #458 shapes flat; full suite green with `AFW_MEMORY_REGION_FREE_LIST_MAX_BYTES=0`; valgrind clean on closures / `pool_eval_lifetime.as` |
| **S2** | Owner probe: debug check that the thread referencing or releasing a managed value owns it. | Are managed values already shared across threads? Borrow-only or atomic interim? |
| **S3** | Generator: `afw_value` extends `afw_reference` in `afw_interface.xml`; `interfaces.py` emits the nested inf and macros. | Is the inheritance and union layout workable with skeletons and macros? |
| **S4** | Audit callers that ignore the pointer `get_reference` / `get_assignable_value` returns (14 `afw_object_get_reference` sites, …). No code. | How big is step 2 |

If any experiment fails, change the design here before writing the real code.

### Steps (each a small branch off `develop`, merged when green)

| # | Step | Touches pool? |
|---|---|---|
| 1 | `afw_reference` interface; `afw_value`, `afw_object`, `afw_array` adopt it. Decide object value vs object count. | No |
| 2 | Fold `get_assignable_value` into `get_reference`; remove `is_managed` checks outside infs. | No |
| 3 | `for_each_reference` + debug check (listed == released) for values, objects, arrays, closure bindings. | No |
| 4 | Scope interface extends `afw_pool`; `afw_pool` adopts `afw_reference`; scope lists its frame slots. | **Yes** |
| 5 | Cycle collector (from S1), per owner. Lab workloads for every #458 shape, all flat. Closes #458. | **Yes** (scope release hook) |
| 6 | Remove pool parent pins and the scope throw-path delay (the error owns its data since PR #470). | **Yes** |

`afw_memory_region` is not touched. Jeremy finished his `afw_pool` / `afw_memory_region` work (2026-10-05), so pool steps are free to start.

Each step: `./afwdev build --cdev`, `afwdev test -j`; before merge `./afwdev build --fulldev`, `afwdev test -j --env-mode valgrind`, and the lab (`src/afw/tests-extra/issue-2/01-rss-hard-loops/`). Update `lifetime-principles.md` with each step that changes a rule.

## Open questions

- Object value and `afw_object_t`: one count or two?
- Does `get_reference` need an optional dest `p` for a copy into a different owner, or is the owner always derivable?
- Collector safe points: deactivate only, or a root-list threshold too?
- Name of the listing method.
- Interim for multithreaded owners: borrow-only or atomic counts (S2).
