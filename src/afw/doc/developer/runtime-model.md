Runtime model (short) {#afw_dev_runtime}
=====================

@brief Pools, values, xctx, and environment — the contract an extension
author needs, with pointers into group docs.

This page is a **map** plus the **lifetime contract** for C code that calls
or implements AFW interfaces. Invariants and details live in the Doxygen
module groups; deeper history is in `designs/lifetime-principles.md` in the source repository.

## Pools

Memory comes from pools and goes back in bulk when a pool goes. Parent/child
is lifetime only; storage comes from the nearest heap.

| Call | Makes |
|------|-------|
| `afw_pool_create(parent, xctx)` | A heap like the parent (single or multithreaded). |
| `afw_pool_heap_create(parent, chunk_min, xctx)` | A single-threaded heap with its own chunks (`0` = 64k). |
| `afw_pool_multithread_create(parent, chunk_min, xctx)` | A multithreaded heap. |
| `afw_pool_tracker_create(parent, xctx)` | A tracker: blocks marked free, returned on garbage collect. |
| `afw_pool_scope_allocate(parent, xctx)` | A blank scope pool. (The `{ }` frame itself is core only.) |

- Allocate with **`afw_pool_malloc` / `afw_pool_calloc` / `afw_pool_free_memory`**
  (and the `_type` forms) with a **dest `p`**. The old `afw_xctx_malloc` /
  `calloc` / `free` macros are gone; never treat `xctx->p` as an implicit dest.
  Freeing the same block twice throws in every build.
- **`afw_pool_release`** for ordinary lifetime. **`afw_pool_destroy`** only
  frees storage and must not fail; call **`afw_pool_run_cleanups`** first if
  callbacks must run.
- **`afw_pool_register_cleanup`** callbacks must not throw.

See group @ref afw_pool.

## Values

Public API type is always **`const afw_value_t *`**. That is a typedef/handle:
**many different C structs** implement values (string/managed/slice, block,
call, symbol, …). Use evaluate/create APIs; do not assume one struct body.
See group @ref afw_value and @ref afw_dev_types_opaques.

Every value, object, and array is one of four kinds. A caller only needs to
know whether it **owns a reference**:

| Kind | Lives in | `get_reference` / `release` |
|------|----------|-----------------------------|
| Permanent | compiled in, registered constants | no-op |
| Pooled (`create_unmanaged`) | a pool it does not own; dies with it | **throw** |
| Reference counted (`create_unmanaged_new_p` / `_cede_p`) | its own pool | count; last release frees its pool |
| Fully managed (`create_managed`) | `p->managed_p` | count; last release releases what it holds |

Values, objects, arrays, and pools extend **`afw_reference`**:
`get_reference(instance, xctx)` and `release(instance, xctx)` (void,
NULL-safe). Release through the interface you referenced through.

### Who releases

| Call | Caller releases? |
|------|------------------|
| `afw_value_evaluate`, a function's `execute_*`, `afw_compile_*` | **No** — the result lasts for dest `p`. |
| A getter that returns a held value (property, entry) | **No** |
| `create_managed` (`afw_object_create_managed`, `afw_array_create_managed`, …) | **Yes** |
| `afw_value_get_reference` | **Yes** |
| `afw_value_get_assignable_value(v, p, xctx)` — a value you own, to store | **Yes** |
| `afw_value_get_for_p_lifetime(v, p, xctx)` — lasts as long as `p` | **No** |

A function does not release its parameters. If it returns a managed value, it
registers that release on the caller's pool.

- **`_take`** setters (`afw_object_set_property_take`,
  `afw_array_push_value_take`) take your reference: do not release it after.
  The plain forms add the container's own reference.
- **`afw_value_hold_read(value, p, xctx)`**: hold an entry or property you
  read before running user code (a callback) that might replace it.
- Copies by intent: **`afw_value_clone`** (independent copy a script may
  change; caller releases), **`afw_value_to_managed`** (share or copy; caller
  releases), **`afw_value_create_pooled_copy`** (pooled copy in `p`; caller
  does not release). Same for `afw_object_*` and `afw_array_*`.
- The runtime object table **borrows**: register only const objects or objects
  in `env->p`, or remove them before their pool goes.

## Execution context (xctx)

Unit of work: scope stack, evaluation, statement_flow, qualifiers, pool. Each
`{ }` evaluates in its own scope pool. `error->backtrace` is a
`const afw_value_hexBinary_t *` (NULL if none), captured when
`response:error:backtrace` is on.
See group @ref afw_xctx.

## Environment

Process-wide registries (`xctx->env`). Core registers at create; extensions
and hosts register more (often via manifests). After load, peers share the
same lookup APIs. Bootstrap order matters (functions before operator prepare).  
See group @ref afw_environment.

## Compile / evaluate

Script, template, expression, JSON, … → value graphs → evaluate.
`afw_compile_*` is **caller does not release**: the result lasts for dest `p`.
A script, template, or test script compiles to a unit that owns its own pool;
a JSON or relaxed JSON compile is plain data in dest `p`.
See groups @ref afw_compile and Adaptive Functions under the public C API.

Most extension authors only **call** compile/evaluate APIs. People changing
the **language grammar** should read @ref afw_dev_compiler_ebnf (EBNF-in-comments
harvest into `generated/ebnf/`).

## C containers

`afw_vector` (growable contiguous elements) and `afw_hash_table` (name to
pointer) are C helpers for your own bookkeeping. They are not values; script
data is `afw_array` / `afw_object`.

## Related developer pages

- @ref afw_dev_overview  
- @ref afw_dev_interfaces  
