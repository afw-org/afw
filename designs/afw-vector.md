# afw_vector — C growable contiguous elements

**Audience:** maintainers. **Not** handbook.

## Settled for this pass

- **`afw_vector`** for AFW C: header stays put, `entries` points at the current chunk, growth copies to a larger chunk and can `free_memory` the old one on a tracker.
- **Typesafe overlay:** `AFW_VECTOR_STRUCT` + shared macros. Entry type lives on the variable. Not a generated `push_Foo` family. Untyped header is `const afw_vector_t internal` (impl view). `_impl` takes `const afw_vector_t *` and casts, like an interface instance. Typed fields are what C callers write.
- **Not an XML interface.** First parameter is the vector. A vtable would fight typed `entries`.
- **Not Adaptive `afw_array`.** No `afw_value_t`, no meta, no setter. Indexes are `afw_size_t` (no negatives). Dense: insert/remove `memmove`.
- **Memory-array store:** `src/afw/array/afw_array_memory.c` is a vector of `const afw_value_t *`. Faces / wrapper / managed vs unmanaged stay. `get_next_value` cursor is the next index (not an `entries` pointer). New slots are NULLed before `slot_store` so pop/shift reuse does not release a transferred pointer.
- **List / extra hash-on-object:** parked. Memory objects keep a single `next`. Do not add `afw_list`.
- **Copy-out:** `afw_vector_copy_entries` mallocs exact `count * entry_size` in dest `p`. `afw_vector_release` `free_memory`s the work header and chunk (no `get_reference`). Compile args use `copy_entries_and_release` in `parser->p` — no child pool per list. `afw_vector_append` copies n entries.
- **Growth:** `growth` 0 = cannot grow; >0 = add that many entries per grow (at least what this call needs); <0 = multiply current allocated by `|n|` (`-2` = double). Default create is `-2`. Qualifier/eval stacks use 0 (fixed; eval allocates `evaluation_stack_maximum_count` up front).
- **`afw_stack` and `array_template` removed.** Qualifier/eval stacks and `scope_stack` are vectors on xctx. Scope stack is `create_fixed_unhandled` sized to `evaluation_stack_maximum_count` (xctx init still cannot use `AFW_TRY`).

## Later

- Align memory-array methods with Script `push`/`splice`/`for-of` and journal-style cursors. Faces / wrapper / managed vs unmanaged stay; the store is already a vector of `const afw_value_t *`. Stay off **#299**.
- Env `registry_types` / `data_type_methods`, ldap mods, index (Jeremy #299) still need a growable array pass.
- **Hash table:** [`afw-hash-table.md`](afw-hash-table.md) landed on `develop` ([PR #301](https://github.com/afw-org/afw/pull/301)); LMDB `dbi_handles` left while #299.
