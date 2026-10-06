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

### Four kinds (decided 2026-10-05)

"Managed" / "unmanaged" described how memory is freed and covered two different things. These names describe what a caller needs to know.

| Kind | Lives in | `get_reference` | `release` | Mutable after hand-off | Cycle collector |
|---|---|---|---|---|---|
| **Permanent** | compiled in, registered constants | no-op, returns self | no-op | no | never walked |
| **Pooled** | a pool it does not own (dies with that pool) | returns a fully managed **copy** | **error** | only while its builder holds it | never walked |
| **Reference counted** | its own pool (`new_p` / `cede_p`); count is the pool's count | count bump, returns self | last release destroys its pool and everything in it | **no** (immutable once handed to its consumer) | dead end: holds only plain values in its own pool |
| **Fully managed** | owner pool (`managed_p`); every value inside is itself counted | count bump, returns self | last release releases each value it holds | yes (replaced values are released) | walked |

- Reference counted and fully managed have the same caller contract. They differ in how deep the counting goes.
- **Mutable means `get_setter` returns a setter.** `set_immutable` turns it off. No other mutability mechanism.
- Typical uses. Reference counted: adapter results, journal entries, conf objects (build, set meta, hand off; the consumer releases). Fully managed: script variables, script-built containers, values that outlive a scope. A script that changes a reference-counted object gets a fully managed face (#17).
- `create_unmanaged_new_p` / `create_unmanaged_cede_p` create **reference counted** objects despite the name. Renames happen in the step that touches each function.

**No exceptions.** Once the rules are written, there are no per-kind special cases outside an inf. Each inf enforces its kind's rules (throw on `release` of pooled, no setter when immutable, copy on `get_reference` of pooled). Callers never inspect an inf or `is_managed`.

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

Until then: a value in a multithreaded owner is borrowed across threads (S2 found no cross-thread references). Where that is not enough, **temporary atomic counts are acceptable** (user, 2026-10-05), as are temporary locks around the few cross-owner evaluates. Both are bridges, not the design, and are deleted when worker threads land.

### Unmanaged = lifetime of p (decided 2026-10-05)

- **`get_reference` of an unmanaged value returns a managed copy** (today's `get_assignable_value` behavior). The caller owns and releases the copy, never the unmanaged original.
- **`release` of an unmanaged value is a caller bug.** Debug builds throw (generated scalars already do: "release of unmanaged scalar"). No `release` touches a pool's reference count on behalf of an unmanaged value.
- **No pool pins from value references.** Today an unmanaged object pins its pool on `get_reference` (`afw_object_memory.c`, legacy C protocol for adapter results and runtime objects). That goes away.
- **An object that owns its own pool** (`new_p` / `cede_p` style) is a managed object whose last release releases its pool. That is an implementation detail behind its inf, the same as `compiled_value` with `unit_owns_p`. The caller only knows "I own a reference; I release it."

Callers never need to know which kind they hold: `get_reference` gives a pointer you own; `release` gives it back.

### Threads in this plan

The reference code is **single-owner only**: no atomics, no locks, no multithreaded cases. Values in multithreaded owners (conf, adapter) are borrowed across threads, never referenced. Where another thread must evaluate or tear down in a multithreaded owner (S2: `service_stop` from a request destroys the adapter), add a **temporary lock** around that evaluate. These are deleted when worker threads land (#343): work for an owner is scheduled onto the thread that owns it, and multithreaded pools go away.

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

| **S6** | Call `set_immutable` on each adapter result at the end of `afw_adapter_internal_process_object_from_adapter` (last pipeline step before the consumer). Run the suite. | Does anything change a result after hand-off? If not, that is the one enforcement point. |
| **S5** | Make `get_reference` / `release` of an unmanaged object or array throw. Run the suite. | List every site that relies on the pool pin. For each: does it need a reference at all (it is within p's lifetime), should it take a managed copy, or should the creator make an object that owns its pool? |

If any experiment fails, change the design here before writing the real code.

**Thread track (alongside, not blocking):** T1 temporary locks around the cross-owner evaluates S2 found (and any S5 finds). T2 worker threads and owner scheduling (#343); delete T1 locks and multithreaded pools.

### Results

**S2 owner probe (2026-10-05).** Debug check in every managed `get_reference` / `release` (generated scalars and slices, object, array, closure binding, compiled_value); patch kept in `git stash` ("S2 owner probe"). Full suite 4609 passed with the probe on.

- Managed values **do** live in multithreaded owners: strings, integers, objects, arrays, closure bindings and compiled values in conf / adapter pools. All of them were referenced and released on the owner thread (base, at startup).
- **No request thread took a reference** to a value another thread owns.
- **One cross-thread release path:** `service_stop` called from a request (afwfcgi orchestration `catalog-value-accessors`, `hold_metrics_across_stop.as`) destroys the adapter on the request thread. The adapter's multithreaded pool runs its cleanups there and releases managed strings and a compiled_value that the base thread owns (`afw_adapter.c` `impl_set_instance_active` → `impl_adapter_release_reference` → adapter destroy → pool cleanup `impl_release_value_at_cleanup`). The base thread is idle by then, so there is no concurrent count change, but it breaks the owner rule.
- Coverage caveat: few multithreaded tests (one orchestration with one request thread).

Decision proposed: **interim is borrow-only** across threads; no atomic count variant. Owner teardown (service stop) is the one exception until stop is scheduled onto the owner (#343). Step 1 adds a debug-build check (reference / release on a thread that does not own the value is an error, except during owner teardown).

Also found while probing (special cases to list for step 1): unmanaged objects pin their pool on `get_reference` (`afw_object_memory.c`); embedded objects forward to their entity; `new_p` / `cede_p` object release is a pool release; associative arrays and object views use atomic counts while every other kind uses plain counts; `afw_value_slot_store` checks for `afw_value_compiled_value_inf`.

**S5 unmanaged references (2026-10-05).** Log every `get_reference` / `release` of an unmanaged memory object or array, and every call to a no-op `get_reference` (const, aggregate, meta, from_values). Patch in `git stash` ("S5 unmanaged").

| Site | Calls in full suite |
|---|---|
| unmanaged object `get_reference` (pins `object->p`) | 13138 |
| — `afw_runtime_env_set_object` (runtime registry; released only by `afw_runtime_remove_object`) | ~13050 (env create, os env, conf types) |
| — `afw_adapter_internal_process_object_from_adapter` (get / retrieve results; `@fixme Need to add releases`, never released) | 82 |
| unmanaged object `release` | **0** |
| unmanaged array `get_reference` / `release` | 0 |
| `afw_object_const_key_value` no-op `get_reference` | 1619 |
| `afw_object_aggregate_external` no-op `get_reference` | 15 |

Every pin taken is held until the pool's parent dies; nothing gives one back.

Remove both pins (unmanaged `get_reference` / `release` do nothing): full suite with the region free list at 0 is 4605 passed, **1 failed**: `model_adapter/onRetrieveObjects.as` SIGSEGV. The model adapter's `returnObject` thunk hands the adapter an object that lives in the `onRetrieveObjects` script's scope pool; the adapter pin was what kept that pool alive. Under the new rule the receiver takes `get_reference` (a managed copy for unmanaged). So the pin stands in for step 2's contract; nothing needs a pin.

**Leak found:** `get_object` in a loop grows ~2.4 KiB/call on develop. ~0.5 KiB is the adapter pin; ~1.9 KiB is the `journal_entry` from `afw_object_create_unmanaged_new_p(x->p)` that is never released. A `new_p` object's own pool is a child of `p->managed_p` (the job heap), not `p`, so an unreleased one lives until the job ends. Releasing it plus dropping the pin: flat. 75 `new_p` / `cede_p` create sites need the same audit. The name `create_unmanaged_new_p` is misleading: that object owns its pool, so it is managed in this pad's terms.

**S6 immutable adapter results (2026-10-05).** `afw_object_set_immutable(object)` at the end of `afw_adapter_internal_process_object_from_adapter`. Full suite: 4608 passed, 1 failed, `miscellaneous/process.as` (`assert(p.peakPoolBytesInUse >= p.poolBytesInUse)`). Not an S6 result: the assertion reads the peak before the live in-use count, and the allocation for the first read can raise in-use past the peak already read whenever usage is at its peak (reproduced in a loop: first read in-use − peak = +112). S6 only moved allocations. Test fix: read in-use first. **Nothing changes an adapter result after hand-off**, so `process_object_from_adapter` is the one enforcement point. Adapters change results only before hand-off (build, set meta, `impl_special_object_handling_cb` sets `conf` meta type). Patch in `git stash` ("S6 immutable results").

**S1 cycle collector (2026-10-05).** Hand-written synchronous trial deletion for managed objects, managed arrays, closure bindings and scopes (~650 lines; patch in `git stash`, "S1 cycle collector"). Possible root = a release that leaves the count above 0; collect at scope deactivate when the root set reaches `AFW_S1_CC`. A value wrapper (`afw_value_object_managed_t`) is treated as the object it wraps: each wrapper reference is one object reference.

| Shape (max RSS 2000 → 8000 calls) | off | on (256) |
|---|---|---|
| local `function f(){}` | ~8.3 KiB/call | flat |
| `const f = function…` | ~8.3 KiB/call | flat |
| `o.f = function(){ return o; }` | ~8.6 KiB/call | flat |
| `o.self = o` | ~0.4 KiB/call | flat |
| `p = {o}; o.p = p` | ~0.8 KiB/call | flat |
| `a → b → c → a` | ~1.0 KiB/call | flat |
| `a.x.y.top = a` | ~1.2 KiB/call | flat |
| inner-block closure into outer slot | ~12.5 KiB/call | flat |
| no cycle (control) | flat | flat |

Lab 15 s: `eval_object_rebind` off 11.46 MiB/s in_use, 708 MB RSS; on **flat**, 38 MB. `eval_closure_rebind`, `closure_rebind`, `object_rebind` unchanged.

Correctness at threshold 1 (collect at almost every deactivate): full suite 4609 passed; with `AFW_MEMORY_REGION_FREE_LIST_MAX_BYTES=0` 4609 passed; valgrind 4609 passed. **No negative trial count** anywhere: the listed edges matched real references for all four kinds.

Cost (200k calls, best of 3): threshold 256 adds ~5–10 % to loops with no cycles (0.58 → 0.63 s, 0.40 → 0.43 s), ~35 % to a loop that makes and frees a cycle every call (0.52 → 0.70 s), and makes the local-function loop faster (2.21 → 2.06 s, less memory). Threshold 1 is ~10× slower, mostly clearing whole tables each round (an S1 artifact: clear cost is table capacity, not live entries).

Learned for step 5:
- **Roots belong to the owner.** S1 kept them per thread; afwfcgi request threads reuse the xctx address, so stale roots from the last request crashed the walk. Fixed by clearing at `afw_xctx_release`. In the real design the root buffer is an owner (xctx) field and dies with it.
- A node must leave the root buffer when it is freed. Fully managed values are only freed by last release, so the hook is enough; anything freed in bulk by a pool must never be a root.
- The wrapper-is-the-object rule works but is a wrinkle; step 1 should decide whether an object value and its object share one count.

**S3 afw_reference inheritance (2026-10-05).** Works. Patch in `git stash` ("S3 afw_reference inheritance").

- **XML:** new `afw_reference` interface (first in `afw_interface.xml`, `instance_member="ref"`) with `get_reference` (returns `const afw_reference_t *`) and `release`. `afw_value` has `extends="afw_reference"`; its own `get_reference` and `optional_release` methods are removed.
- **Generator (`interfaces.py`):** `resolve_extends()` copies the base methods to the front of each derived interface in memory, replacing `afw_reference_t` with the derived `_t` in parameter and return types. Every existing emitter (inf struct, call macros, impl declares, skeletons, `_AdaptiveInterface_` objects) then works unchanged. The derived instance struct gets `union { const afw_value_inf_t *inf; afw_reference_t ref; }`. Methods marked `null_safe="true"` get a NULL-safe call macro (`optional="true"` also checks the slot; S3 only, since the design makes `release` mandatory).
- **Result:** `afw_value_inf_t` starts `rti, get_reference, release, …`, the same layout as `afw_reference_inf_t`. Generic code `afw_reference_release(&v->ref, xctx)` and the derived `afw_value_release(v, xctx)` both compile with `-Wall -Wextra -Werror` (probe with a `_Static_assert` on the offsets). The hand-written NULL-safe `afw_value_release()` function is replaced by the generated macro.
- **C fallout:** rename `optional_release` → `release` in 26 hand files and `data_type_bindings.py` (mechanical). Build clean (core and extensions), suite 4609 passed.

Learned for step 1:
- 54 value infs set `release` to NULL; making `release` mandatory means a no-op for permanent / IR values (one shared function), and drops the `optional` slot check.
- Four places outside an inf test `inf->optional_release` to decide what to do (`afw_pool.c` ×2, `afw_pool_scope.c`, `afw_value_slot_take`). They go away with a mandatory `release`.
- Only one level of `extends`, resolved within one package's XML. An extension interface extending `afw_reference` needs the core XML at generate time (later).
- Data-type value structs (`afw_value_<type>_s`, union `inf` / `pub`) can add `afw_reference_t ref` to the same union in `data_type_bindings.py`.

**S4 caller audit (2026-10-05, read only).** Step 2 is small.

- **Values:** every caller of `afw_value_get_reference` (2) and `afw_value_get_assignable` / `get_assignable_value` (11) already uses the returned pointer. `afw_value_add_reference` has no callers.
- **Objects / arrays:** `get_reference` returns `void`, so all 16 object and 4 array sites ignore the pointer by construction. With "returns a pointer you own (self or a copy)":
  - store the returned pointer: associative-array object store (`afw_object_memory_associative_array.c` ×3), environment-variables object (holds `properties`), object view (holds `origin`), managed wrappers (`create_wrapper_managed` holds `wrapped`, object and array);
  - forward inside an inf (no change): meta object → embedding object, embedded (`managed_by_entity`) → entity;
  - bump-and-return-self paths in `create_managed_clone` (object, array): already use the same pointer;
  - remove (S5): runtime registry pins (`afw_runtime.c` ×2), adapter result pin (`afw_adapter.c`); these borrow instead;
  - test probes (2).
- **Inf inspection outside an inf** (to remove in steps 1–2): `inf->optional_release` checks in `afw_pool.c` ×2, `afw_pool_scope.c`, `afw_value_slot_take`; `inf->is_managed` in `afw_value_slot_take`; `afw_object_is_managed` in `afw_error.c`; `afw_object_is_memory_managed` in `afw_object_meta.c`; `afw_value_slot_store`'s `afw_value_compiled_value_inf` check.

**Step 0 status (2026-10-05, uncommitted on `issue-476-references`).** `lifetime-principles.md` has *Four kinds* plus updated *Two worlds*, cycles, and checklist. `Kind:` line on every create function in `afw_object.h`, `afw_array.h`, `afw_value.h`, `afw_compile.h`, and the generated `afw_value_<type>_*` (via `data_type_bindings.py`). Exceptions found while writing them (fix in step 2): `afw_value_closure_binding_create_if_needed` returns the pooled definition unchanged when there is no current block scope, but its only caller is `get_assignable_value` (caller releases); `afw_compile_json_to_object` said "caller does not release" even with `cede_p` (corrected in its doc).

### Decisions after the experiments (2026-10-05)

- **`release` is mandatory** on every `afw_reference` inf. Permanent and compiler-IR values share one no-op `release` and a `get_reference` that returns self. No NULL slot; no outside check for one.
- **`get_reference` returns the interface it was called through:** `afw_object_get_reference` → `const afw_object_t *`, `afw_array_get_reference` → `const afw_array_t *`, `afw_value_get_reference` → `const afw_value_t *` (from S3's type substitution).
- **One count** for an object value and its object (same for arrays), but **release through the interface you referenced through**. Documented on the methods and in `lifetime-principles.md`.
- **`release` is `void` everywhere.** The pool's pointer-returning release is reconciled at step 4, ideally by removing the parent pins (step 6) at the same time. A different method with its own name only if something still needs "did this destroy it?".
- **Clone names by intent.** `clone` = independent, changeable, fully managed copy (Adaptive `clone()` already is this; the C name matches it, one implementation). "Keep it safely" = `get_reference`. Pooled copy (today's `afw_value_clone`, `afw_object_create_clone`, `clone_*_unmanaged`, `afw_array_create_or_clone`) is renamed or removed after checking callers. `afw_pool_scope_clone`, `afw_utf8_clone`, `afw_object_meta_clone_and_set` are not value clones and stay.
- **Closures stay simple:** one binding (definition, captured scope, unit), made in one place when a function value is stored. Fold away `create_if_needed` (and its exception) and extra closure paths in step 2.
- **Built-in rule (unchanged):** anything a built-in creates that needs a release is released inside, usually by registering last-release on `x->p`.
- **Extensions:** low priority. afwdev may keep the core XML (or a resolved form) so another package's interface can extend `afw_reference`.
- **T1** (cross-thread release at `service_stop`) is fixed alongside step 1 or 2.

**Step 1 status (2026-10-05, uncommitted on `issue-476-step1-reference`).**

- 1a: `afw_reference` in `afw_interface.xml` (first interface, `instance_member="ref"`); `resolve_extends()` and NULL-safe macros in `interfaces.py`; `afw_value` extends it; `optional_release` → `release` (mandatory); 54 NULL `release` and 16 NULL `get_reference` slots now `afw_value_not_counted_*`. `afw_value_add_reference` removed (only the generated string slice used it; it now stores `get_reference`'s result). Hand-written NULL-safe `afw_value_release()` replaced by the generated macro.
- 1b: `afw_object` and `afw_array` extend it; 23 `get_reference` implementations return their own pointer type. Hand-written positional infs (`AFW_RUNTIME_OBJECT_INF`, `afw_runtime_const_meta.c`) reordered. Static initializers of `afw_object_t` / `afw_array_t` need `{&inf}` now that `inf` is in a union (`const_objects.py`, `data_type_bindings.py`, `function_bindings.py`, `afw_environment_registry_object.c`).
- 1c: `afw_value_<object|array>_create_managed` of a fully managed instance returns its own face (one count); wrappers remain only over pooled instances (step 2 removes them).
- 1d (partial): the four outside "has a release method" checks become `afw_value_is_not_counted()` with "#476 step 2 removes this check"; `afw_value_slot_take` needed it immediately (permanents now have a `release`).
- Gates: build clean (core and extensions), suite 4609 passed, region free list 0: 4609 passed.

**Step 2 status (2026-10-06, uncommitted on `issue-476-step2-get-reference`).** `get_assignable_value` stays (values only, takes `p`, ECMAScript-style face for permanent objects); `get_reference` takes no `p`.

- New `afw_value` method `get_for_p_lifetime(x, p)` with shared `afw_value_{not_counted,counted,pooled}_get_for_p_lifetime`; every value inf chooses one. `afw_pool_scope_get_assignable_for_p_lifetime` calls it; `afw_pool_release_value_at_cleanup` is `get_for_p_lifetime` then `release` (fixes a second hand-off of the same value on the same `p`, which used to leak one reference). `afw_value_is_not_counted` and all its uses are gone.
- `get_assignable_value` mandatory (16 IR infs use `afw_value_not_counted_get_assignable_value`); `afw_value_get_assignable` no longer checks the slot. `afw_value_slot_store` drops its compiled-value special case and no longer leaks when `get_assignable_value` returns the current occupant; `afw_value_slot_take` drops its check.
- Adapter result pin removed (`afw_adapter_internal_process_object_from_adapter`). Model adapter `returnObject` hands the callback an owned reference (`get_assignable_value` into the request `p`).
- Built-in adapter functions make their journal entry pooled in `x->p` (was `new_p` and never released): `get_object` loop flat (develop ~2.4 KiB/call).
- Closures: one place. `afw_value_closure_binding_create_if_needed` removed; a script function's `get_assignable_value` always binds (captured frame, or none, plus the unit).
- Compiled value: one counted inf from parser create; the unmanaged and "assignable" (pool-pin) infs are gone.
- `is_managed` kept as a capability flag (any implementation can declare it; `afw_*_is_managed()`). `afw_error.c` no longer forks on it. The memory module's identity check (`afw_*_is_memory_managed`) only guards its own casts.
- New setter methods `set_property_take` / `push_value_take`: fully managed memory setters store (the old take bodies); every other setter shares `*_take_by_copy` (copy into its pool, release). Public `afw_object_set_property_take` / `afw_array_push_value_take` call the setter. Generated `*_internal` helpers use `afw_*_is_managed` (capability) and the setter take.
- Gates: suite 4609 passed; region free list 0: 4609 passed.

**Step 2, second part (2026-10-06).** Runtime object table borrows; it owns only its own indirect objects (`owned_by_table`; replace / remove releases their pool). Pooled memory object / array `get_reference` and `release` throw. Objects and arrays that own their pool get `afw_value_counted_*_inf` (references go to the instance; `get_assignable_value` gives a fully managed face / copy). `afw_value_<object|array>_create_managed` of a non-managed instance uses its face's `get_assignable_value`. `_take` kept and documented (the callee takes ownership of the caller's reference; GLib convention); `afw_value_slot_take` releases the caller's reference when it is already the occupant. Found by removing the registry pins: `afw_environment_load_extension` released `env->p` on its two error paths (left from when `p` was the extension's own pool); the ~100 registry pins per process had been absorbing it.

**Step 2b status (2026-10-06, branch `issue-476-step2b-clone`).** Clone names by intent (table in `lifetime-principles.md`, *Copies by intent*). `afw_value_clone` now means the independent copy (moved from Adaptive `clone()`); its old meaning is `afw_value_create_pooled_copy` (all callers renamed; `afw_value_clone_unmanaged` merged into it). `afw_object_create_clone` / `afw_array_create_or_clone` → `*_create_pooled_copy`. `afw_*_create_managed_clone` / `afw_value_clone_managed` → `afw_*_to_managed`. `afw_object_create_managed_snapshot` removed (deep `afw_object_clone` replaces it); `afw_object_managed_clone_for_caller` → `afw_object_clone_for_p`. Dead `afw_value_clone_or_reference` macro removed. Suite 4609 passed (both modes).

**Step 2b status (2026-10-06, branch `issue-476-step2b-clone`).** Clone names by intent (table in `lifetime-principles.md`, *Copies by intent*). `afw_value_clone` now means the independent copy (moved from Adaptive `clone()`); its old meaning is `afw_value_create_pooled_copy` (all callers renamed; `afw_value_clone_unmanaged` merged into it). `afw_object_create_clone` / `afw_array_create_or_clone` → `*_create_pooled_copy`. `afw_*_create_managed_clone` / `afw_value_clone_managed` → `afw_*_to_managed`. `afw_object_create_managed_snapshot` removed (deep `afw_object_clone` replaces it); `afw_object_managed_clone_for_caller` → `afw_object_clone_for_p`. Dead `afw_value_clone_or_reference` macro removed. Suite 4609 passed (both modes).

**Step 3 status (2026-10-06, branch `issue-476-step3-for-each-reference`).** `afw_reference` gains `get_reference_count` and `for_each_reference`; `afw_value` gains `get_counted`. Callback type `afw_reference_cb_t` (`afw_common.h`). Shared implementations for kinds that are not counted or hold nothing. New module `reference/` with `afw_reference_check()` (debug; `AFW_REFERENCE_CHECK` runs it at every scope exit from each counted frame slot). Suite 4609 passed with and without the check; a deliberate double listing is caught ("afw_object implementation 'memory_managed' is listed more times than its count 1"). Closure binding's captured scope is listed once scope is an interface (step 4).

**Steps 4 + 6 status (2026-10-06, branch `issue-476-step4-scope-pool`; one branch from here on).**

- **Scope count = pool count.** The separate scope `reference_count` is gone; the scope pool's last release releases frame slots and the lexical parent, then the pool (`releasing_frame` guards re-entry).
- **One parent rule** (`holds_parent`, replaces `parent_pins` and its per-release cascade): a pool holds one parent reference while its count is above 1. The scope and thread pools' "hold the parent from create" special cases are gone. `pool_heap` probe updated to the rule.
- **`afw_pool extends afw_reference`.** `release` is void (each inf wraps its internal release, which keeps its pointer result; `AFW_POOL_INTERNAL_REFERENCE_WRAPPERS`), `get_reference` returns the pool, shared `get_reference_count`; the scope pool's `for_each_reference` lists frame slots and lexical parent; others list nothing. Own-pool object / array release is a plain `afw_pool_release` (their `wrapped` check was dead: only fully managed wrappers have one).
- **Closure binding lists its captured scope.** `AFW_REFERENCE_CHECK` is rooted at the scope at every scope exit.
- **Throw-path delay kept (new step 6a).** Removing it broke 13 tests: the error holds raw pointers into pools (`message_z` from compile / query criteria / curl, `parser_source`, `contextual`, `rv_*` strings), and the delay is what keeps those alive until the catch. Step 6a: the error owns everything it points to (copy strings at throw into storage the error owns, reference the compile unit behind `contextual`, single ownership across `AFW_ERROR_COPY`), then remove the delay and the adapter's `error_processing_count` hold.
- Gates: suite 4609 passed (plain, `AFW_REFERENCE_CHECK`, region free list 0); lab 15 s 44 passed (`eval_object_rebind` still climbs: the cycle). One lab run showed a garbage first `in_use` sample for `function_return` (5.28e18); not reproduced in three reruns. The lab reads `env->pool_bytes_in_use` with gdb at an arbitrary instant under 16 parallel workloads; treated as a sampler artifact.

**Step 5 status (2026-10-06, branch `issue-476-step4-scope-pool`).** Cycle collector in `reference/afw_reference.c` (see `lifetime-principles.md`, *Reference cycles*). New `afw_reference` method `release_references` (managed object / array / `from_values`, closure binding, scope pool; shared no-op elsewhere). Possible roots per xctx (`xctx->reference_collector`), safe point at scope exit, default threshold 50, adaptive. Scalars and slices out of the graph (`get_counted` NULL).

- Shapes (2000 -> 8000 calls): every #458 shape flat; off: 0.5–12.5 KiB/call.
- Lab 15 s: 50 passed (44 + 6 new #458 workloads, all flat); `eval_object_rebind` flat (was 8.76 MiB/s in_use); 60 s in_use flat, RSS ~0.2 MiB/s (heap reuse, not in_use).
- Cost (200k calls, best of 3, threshold 50 vs off): no-cycle loops +2–3 %; a self-cycle every call +12 %; local-function loop 6x faster (less memory); a 2000-object live structure re-rooted every call +5 % (adaptive threshold).
- Found and fixed: pool children were a singly linked list (unlink O(n), quadratic when many pools die at once); now doubly linked.
- Follow-up (not fixed here): the heap free list is first-fit with a linear scan; bulk frees (and some ordinary loops, e.g. `keep.x = o` each call, ~4.8 s / 200k with or without collection) spend most time there. Size-segregated free lists would fix it generally.
- Gates: suite 4609 passed (default; `AFW_REFERENCE_COLLECT=1`; `=1` with `AFW_REFERENCE_CHECK` and region free list 0); valgrind with `AFW_REFERENCE_COLLECT=1` 4609 passed.

### Steps (each a small branch off `develop`, merged when green)

| # | Step | Touches pool? |
|---|---|---|
| 0 | **Docs first.** `lifetime-principles.md`: the four kinds (what each is, who frees it, `get_reference` / `release`, mutable after hand-off, collector). Doc comment on every create function saying which kind it makes and who releases: hand-written headers (`afw_object.h`, `afw_array.h`, `afw_value.h`, closure / compiled_value) and `data_type_bindings.py` for generated `afw_value_<type>_create_*`. No renames. | No |
| 1 | `afw_reference` interface; `afw_value`, `afw_object`, `afw_array` adopt it. Decide object value vs object count. | No |
| 2 | Fold `get_assignable_value` into `get_reference`; remove `is_managed` checks outside infs; simplify closures. | No |
| 2b | Clone names by intent: `clone` = independent copy (one implementation, shared with Adaptive `clone()`); rename or remove pooled copies after checking callers. | No |
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
