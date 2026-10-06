# Lifetime principles

**Audience:** maintainers / assistants. **Not** handbook.

This is the **value lifetime story**. Inf-method rails, two worlds, eval `p`, and pool doors are details of this story. When a pad, rule, or comment disagrees with this file, **this file wins** until we change it here.

Code and tests remain ground truth. If the tree and this story disagree, fix the tree or this story; do not add a third protocol.

**Related:** inf rails [`issue-2-hold-in-inf.md`](issue-2-hold-in-inf.md) (**implementation order** at the top). Two worlds [`experiment-brainstorm.md`](experiment-brainstorm.md); eval `p` [`experiment-eval-p.md`](experiment-eval-p.md); pool doors [`remaining-apr.md`](remaining-apr.md). Lab: `src/afw/tests-extra/issue-2/01-rss-hard-loops/`. Tree vs this story: [#443](https://github.com/afw-org/afw/issues/443), [#445](https://github.com/afw-org/afw/issues/445), [#446](https://github.com/afw-org/afw/issues/446). n=5 dest-p honor (compile caller does not release) landed [PR #460](https://github.com/afw-org/afw/pull/460) `3fdaabdd`.

---

## Words

Say **reference** for a value or scope lifetime (`get_reference` / `release`).

**Return contract:** **caller does not release** or **caller releases**. Do not name that contract unmanaged, managed, temp, extra-hold, pin, or “expects unmanaged.”

**Kind:** every value, object, and array is one of four kinds: **permanent**, **pooled**, **reference counted**, **fully managed** (next section). Code names still say **unmanaged** (= pooled) and **managed** (= fully managed); `create_unmanaged_new_p` / `create_unmanaged_cede_p` make **reference counted** objects despite the name. Names change when a #476 step touches them.

**Obsolete for the contract:** extra-hold, extra bump, temp, pin, bridge, dangerous crack, “caller expects unmanaged/managed.” Residual C names (`release_value_at_cleanup`) are leftovers in the tree, not a second protocol.

**Smell:** extra-hold, special case, extra bump, helpers around assign/GET/clone as a leak fix, treating `xctx->p` as an implicit dest, caching dest `p` on the xctx (`script_result_p`). If a leak needs a new flag or a third way to keep a value alive, stop.

---

## Four kinds

Decided in [#476](https://github.com/afw-org/afw/issues/476) (pad [`issue-476-references-and-owners.md`](issue-476-references-and-owners.md)). A caller only needs to know whether it **owns a reference**: permanent and pooled are not counted; reference counted and fully managed are.

| Kind | Lives in | `get_reference` | `release` | Mutable after hand-off | Cycle collector |
|---|---|---|---|---|---|
| **Permanent** | compiled in (object code), registered constants | no-op, returns self | no-op | no | never walked |
| **Pooled** | a pool it does not own; dies with that pool | **error** (use `get_assignable_value` / `get_for_p_lifetime` for a copy) | **error** | only while its builder holds it | never walked |
| **Reference counted** | its own pool (`new_p` / `cede_p`); its count is that pool's count | count bump | last release destroys its pool and everything in it | **no** | dead end (holds only plain values in its own pool) |
| **Fully managed** | the owner pool (`p->managed_p`); every value it holds is itself counted | count bump | last release releases each value it holds, then frees itself | yes (replaced values are released) | walked |

- **Reference counted** vs **fully managed**: same caller contract; they differ in how deep the counting goes. Reference counted is for build, hand off, read, release (adapter results, journal entries, conf objects). Fully managed is for values that change or outlive a scope (script variables, script-built containers). A script that changes a reference-counted object gets a fully managed face (#17).
- **Mutable means `get_setter` returns a setter.** `set_immutable` turns it off. There is no other mutability mechanism.
- **Hand-off.** A reference-counted object's builder fills it (properties, meta) and hands it to its consumer; after that it is immutable and the consumer releases it. Adapter results are handed off at the end of `afw_adapter_internal_process_object_from_adapter`.
- **Owner.** `p->managed_p` is the owner of fully managed values: the job heap for a request, `adapter->p` when evaluating in adapter config. A fully managed value has one owner; counts are not atomic. Values crossing owners are copied, or borrowed when the owner outlives the borrower. (Temporary atomic counts or locks are acceptable as a bridge until worker threads, #343.)
- **`afw_reference`.** `afw_value`, `afw_object`, and `afw_array` extend the `afw_reference` interface (`extends="afw_reference"` in `afw_interface.xml`): their infs start with `get_reference` and `release`, and an instance can be used as `afw_reference_t` (`&x->ref`). `get_reference` returns a pointer typed as the interface it was called through. Both methods are mandatory; values that are not counted (permanent, compiler values) share `afw_value_not_counted_get_reference` / `afw_value_not_counted_release`.
- **Listing references (#476 step 3).** `afw_reference` also has `get_reference_count` (0 when not counted) and `for_each_reference(callback)`, which calls the callback once per reference the instance's last release would release, naming the counted instance it is held in. `afw_value` has `get_counted` (the instance whose count a reference to this value is held in, or NULL): containers list what each held value references with `afw_value_list_reference`, so no one inspects a kind. Fully managed objects list names, values, and wrapped; fully managed arrays and managed `from_values` arrays list entries; slices list their containing value; closure bindings list their compile unit; everything else lists nothing. Debug check `afw_reference_check(root)` (on at scope exit with env `AFW_REFERENCE_CHECK`) throws when something lists more references to an instance than its count.
- **One count, matching release.** An object value and its object share one count (the same for arrays): `afw_value_<object|array>_create_managed` of a fully managed instance returns its own value face. Still, **release through the interface you referenced through** (`afw_object_get_reference` → `afw_object_release`; value → value).
- **Three ways to get a value you can keep** (`afw_value`; each inf decides, no caller inspects the kind):

  | Method | Meaning | pooled | counted | permanent |
  |---|---|---|---|---|
  | `get_reference(x)` | keep this exact instance; caller releases | error | bump, self | self |
  | `get_assignable_value(x, p)` | a value I own, to store (script may change it); caller releases | fully managed copy | bump, self | scalar: self; object/array: fully managed face (ECMAScript-style mutability) |
  | `get_for_p_lifetime(x, p)` | lasts as long as `p`; caller does not release | copy, release registered on `p` | bump, release registered on `p` (once per `p`) | self, nothing registered |

  `afw_pool_release_value_at_cleanup(v, p)` ("p takes my reference") is `get_for_p_lifetime` then `release`. All three methods and `release` are mandatory; values that are not counted share `afw_value_not_counted_*`.
- **Copies by intent** (#476 step 2b):

  | Intent | Functions | Caller releases? |
  |---|---|---|
  | Independent copy a script may change (nested objects and arrays copied too) | `afw_value_clone`, `afw_object_clone`, `afw_array_clone`; Adaptive `clone()` is `afw_value_clone` | yes (`afw_object_clone_for_p`: no, registered on `p`) |
  | Fully managed version, sharing when it can (count bump if already fully managed, else a fully managed copy) | `afw_value_to_managed`, `afw_object_to_managed`, `afw_array_to_managed` | yes |
  | Pooled copy in `p` (take a value out of live data or another pool) | `afw_value_create_pooled_copy`, `afw_object_create_pooled_copy`, `afw_array_create_pooled_copy` | no |

  Not value copies: `afw_pool_scope_clone` (loop frame), `afw_utf8_clone`, `afw_object_meta_clone_and_set`. The per-type `afw_value_clone_<type>_{managed,unmanaged}` are data-type internals behind `to_managed` / `create_pooled_copy`.
- **Copies by intent** (#476 step 2b):

  | Intent | Functions | Caller releases? |
  |---|---|---|
  | Independent copy a script may change (nested objects and arrays copied too) | `afw_value_clone`, `afw_object_clone`, `afw_array_clone`; Adaptive `clone()` is `afw_value_clone` | yes (`afw_object_clone_for_p`: no, registered on `p`) |
  | Fully managed version, sharing when it can (count bump if already fully managed, else a fully managed copy) | `afw_value_to_managed`, `afw_object_to_managed`, `afw_array_to_managed` | yes |
  | Pooled copy in `p` (take a value out of live data or another pool) | `afw_value_create_pooled_copy`, `afw_object_create_pooled_copy`, `afw_array_create_pooled_copy` | no |

  Not value copies: `afw_pool_scope_clone` (loop frame), `afw_utf8_clone`, `afw_object_meta_clone_and_set`. The per-type `afw_value_clone_<type>_{managed,unmanaged}` are data-type internals behind `to_managed` / `create_pooled_copy`.
- **Closures are made in one place:** storing a script function (`get_assignable_value` of the definition) makes a binding of the definition, the captured frame (if any), and the compile unit.
- **A compiled value has one inf** and is counted: compile returns it at RC 1 and registers that release on dest `p`.
- **`is_managed` is a capability.** The `is_managed` inf variable on `afw_value`, `afw_object`, and `afw_array` (`afw_*_is_managed()`) says "this implementation is fully managed", so any implementation, including future ones, can declare it. A module's own identity check (`afw_object_is_memory_managed`) is only for guarding a cast to that module's private struct.
- **`_take` means the callee takes ownership of the caller's reference.** The caller owns one reference to the value (for example a `create_managed` result) and must not release it after the call. Same convention as GLib (`g_value_take_string()` beside `g_value_set_string()`), GObject Introspection `(transfer full)`, and CPython's "steals a reference". The plain form (`set_property`, `push_value`) adds the container's own reference; the caller keeps and releases its own.
- **Taking a reference into a container:** setter methods `set_property_take` (object) and `push_value_take` (array) store a value and take the caller's reference. A fully managed implementation stores it; others copy it into their pool (`afw_object_setter_set_property_take_by_copy` / `afw_array_setter_push_value_take_by_copy`) and release it. `afw_object_set_property_take` / `afw_array_push_value_take` go through the setter, so they work for any object or array. The generated `*_internal` helpers use the `is_managed` capability to pick the cheap path (pooled value in the object's pool) for pooled objects.
- **Faces by kind.** A pooled object's or array's value face is `afw_value_unmanaged_*_inf` (`get_reference` / `release` throw). One that owns its pool (`new_p` / `cede_p`, and objects embedded in it) has `afw_value_counted_*_inf`: references go to the instance, and `get_assignable_value` gives a fully managed face (object) or copy (array), so a script change never touches the original (a `get_object` result changed by a script does not change the next `get_object`). A fully managed one has `afw_value_managed_*_inf` (one count with the instance).
- **Runtime object table borrows.** `afw_runtime_env_set_object` / `afw_runtime_xctx_set_object` take no reference: a registered object is const, built in `env->p`, or removed before its pool goes. The table owns only the indirect objects it makes itself (`afw_runtime_env_create_and_set_indirect_object`, `owned_by_table`): replacing or removing one releases its pool. Get, retrieve, and foreach of an indirect object return a copy. Runtime objects' `get_reference` / `release` are self / no-op.
- **No exceptions.** Each inf enforces its kind's rules. Code outside an inf does not inspect the inf or `is_managed` to decide what to do.
- **Every create function's doc comment says which kind it makes and who releases.**

**Tree catching up (#476):**


- The value wrapper type (`afw_value_<object|array>_managed_t`) is now only used for an object or array that has no value face; it can be deleted once no implementation lacks one.
- Reference-counted adapter results are not yet made immutable at hand-off (#476 S6 showed nothing changes them after).

---

## Two worlds

| | Unmanaged (pooled) | Managed (fully managed) |
|---|---|---|
| Where | dest `p` (evaluation `{ }` is `scope->p`) | dest `p->managed_p` |
| Death | that pool bulk-frees | last RC: `release` every reference this value holds, `free_memory` every block it allocated (or last-release a pool it owns) |

**Pooled has no references.** A pooled instance lives and dies with its pool. Its `get_reference` / `release` never touch a pool's RC. Anything that needs it past that pool calls `get_assignable_value` and gets a fully managed value. **Legacy still in the tree:** pooled memory objects and arrays still pin their pool on `get_reference` for C callers (adapter results, runtime `set_object`). #476 S5 found ~13k such pins in the suite and **no** releases; removing them breaks only the model adapter `returnObject` path, which needs a copy, not a pin. Not a protocol to copy.

A **reference-counted** object (`new_p` / `cede_p`) is not pooled: its own pool is its lifetime, and `get_reference` / `release` on it are correct. Its pool's parent is `p->managed_p`, so a missing release keeps it until the owner dies (#476: `get_object` never released its `journal_entry`).

Pass dest `p`. Do not treat `xctx->p` as an implicit dest. `afw_xctx_*alloc`/`free` used to allocate in `xctx->p` (an older world where `xctx->p` was `managed_p`). Those macros are **gone** ([#443](https://github.com/afw-org/afw/issues/443) on `fix-443-xctx-alloc-dest-p`): `afw_pool_*` with dest `p` (managed values: `p->managed_p`; env-lifetime: `env->p`; stored on the xctx: `xctx->p`). Do not reintroduce them. `set_property_as_string_from_utf8_z` stores the pointer; the bytes must outlive the object.

---

## Dual face

An object or array **instance** has an `afw_value` as an instance variable. Script sees values. For a managed object/array, **instance RC is the managed lifetime**. The embedded value is not a second counter. Value `get_reference` / `release` last-release **the instance**. Last RC of the instance is the one walk.

Pooled instance: value `get_reference` / `release` throw. Use `get_assignable_value`. A separate wrapper around an instance is another managed value that references the instance.

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

**Pools (#476 step 4).** `afw_pool` extends `afw_reference`: `get_reference` returns the pool, `release` is void, `get_reference_count` is the pool's count. One parent rule for every pool: a pool holds **one** reference on its parent while someone other than its creator references it (its count is above 1); it takes it when the count goes from 1 to 2 and gives it back when the count returns to 1 (`holds_parent`). A pool only its creator references is part of its parent and dies with it (`destroy_children`, the backstop for children never released); a referenced one keeps its parent alive (a tracker's memory comes from an ancestor heap). `destroy` kills the subtree. No other pool holds its parent from create.

**Scopes.** A scope's count is its scope pool's count. Its last release releases the frame slots and its lexical parent (a real reference, separate from the pool tree), then the pool. Its `for_each_reference` lists the frame slots and the lexical parent; a closure binding lists its captured scope. The scope pool's parent is `p->managed_p` (the job heap), never dest `p`. The scope pool is the frame's temporary memory, freed in one shot at its last release; frame slots hold only `get_assignable_value` results. No special case for `try` / `catch`: at every throw and rethrow the error makes itself own everything it points to (`afw_error_own_pointers`: message, source, rv strings, parser source copied into one block the error owns; a reference to the compile unit behind `contextual`), so scopes release normally while a throw unwinds. `AFW_ERROR_MOVE` hands that ownership from one `afw_error_t` to another.

**The error owns references** to its data and backtrace (`afw_error_set_data`, `afw_error_release_references`). They move with the error struct (`AFW_ERROR_COPY` / `AFW_ERROR_CLEAR_PARTIAL`) and are released at a caught `ENDTRY`, a new error set, or xctx release.

**Reference cycles: cycle collection ([#458](https://github.com/afw-org/afw/issues/458), [#476](https://github.com/afw-org/afw/issues/476) step 5).** Reference counting alone never frees a loop of references (closure in its own frame, `o.self = o`, `a -> b -> c -> a`, object -> closure -> frame -> object). Each owner (xctx) collects them by synchronous trial deletion (Bacon & Rajan 2001), using only the `afw_reference` interface:

- **Possible roots.** A release that leaves a cycle-capable instance (fully managed object / array, managed `from_values` array, closure binding, scope) above 0 records it, if this xctx owns it and that owner is single-threaded (`afw_reference_possible_root`, owner check `p->managed_p == xctx->p`, not a multithreaded pool). Last release forgets it; xctx release drops the list. Values owned by a multithreaded pool (`env->p`, conf, adapter) can be last-released on another thread, so they are not collected until worker threads give every owner one thread (#343).
- **Safe point:** scope exit (`afw_reference_safe_point`), when the possible roots reach the threshold: env `AFW_REFERENCE_COLLECT` (default 50; 0 off), raised after each collection to the number of still-alive instances it walked, so the cost per root stays constant even when a large live structure keeps becoming a possible root.
- **Collect** (`afw_reference_collect`): from the roots, walk `for_each_reference`, subtracting each listed reference from a trial copy of `get_reference_count`. Anything still above 0 is referenced from outside and kept with everything it reaches; the rest only reference each other. Those are held (`get_reference`), emptied (`release_references`), then released normally. A negative trial count (a listed reference that is not held) aborts the round.
- **Leaves stay out:** scalars and slices cannot lead back, so `get_counted` returns NULL for them; they are freed when the garbage holding them is emptied. Reference-counted (own-pool) objects list nothing (dead ends).

---

## When leftover RC appears

1. Did last RC complete the walk?
2. Was `get_assignable_value` called on a value already must-release, and only one hold released?
3. Did a method that returns a held value expect the caller to `release`?
4. Did a callee honor caller does not release with managed and forget to register last-release on dest `p`, register twice, or use `xctx->p`?
5. Did a callee return unmanaged on a caller-releases contract?
6. Does a release registered on a pool keep that same pool alive (directly, through a scope, or through a child pool's create reference)?
7. Does a counted value reference itself through other values (a cycle: closure in its own frame, `o.self = o`, …)?
8. Does a managed value point into memory (a unit, another pool) it does not reference?

Probes that find these are in [`agent-support.md`](agent-support.md) (*Leftover RC / pool never dies*).

If the answer is a new register last-release, a new flag, or a helper around assign, stop.

---

## Using this pad

C sittings make the tree match this file. Until last RC of every managed value completes the walk, a fix in one place can show leftover in another. That is expected. Do not add extra-hold or register last-release on a method that returns a held value to hide it. Re-measure the RSS lab after each vertical. #2 stays open.

Not the #2 scoreboard. Not the RSS lab table. Not a license to rewrite heap free lists.
