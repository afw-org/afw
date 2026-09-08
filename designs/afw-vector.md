# afw_vector — C growable contiguous elements

**Audience:** maintainers. **Not** handbook.  
**Branch:** `experiment/reduce-apr`.

## Settled for this pass

- **`afw_vector`** replaces `apr_array` for AFW C: header stays put, `entries` points at the current chunk, growth copies to a larger chunk and can `free_memory` the old one on a tracker.
- **Typesafe overlay:** `AFW_VECTOR_STRUCT` + shared macros. Entry type lives on the variable. Not `APR_ARRAY_PUSH(ary, type)`, not a generated `push_Foo` family. Untyped header is `const afw_vector_t internal` (impl view). `_impl` takes `const afw_vector_t *` and casts, like an interface instance. Typed fields are what C callers write.
- **Not an XML interface.** First parameter is the vector. A vtable would fight typed `entries`.
- **Not Adaptive `afw_array`.** No `afw_value_t`, no meta, no setter. Indexes are `afw_size_t` (no negatives). Dense: insert/remove `memmove`.
- **List / extra hash-on-object:** parked. Memory objects keep a single `next`. Do not add `afw_list`.
- **Do not rewrite `afw_array` yet.** Swap leftover `apr_array` call sites onto vector first. Memory-array can sit on a vector of `const afw_value_t *` later.
- **Copy-out:** `afw_vector_copy_entries` mallocs exact `count * entry_size` in dest `p`. `afw_vector_release` `free_memory`s the work header and chunk (no `get_reference`). Compile args use `copy_entries_and_release` in `parser->p` — no child pool per list. `afw_vector_append` copies n entries.
- **Growth:** `growth` 0 = cannot grow; >0 = add that many entries per grow (at least what this call needs); <0 = multiply current allocated by `|n|` (`-2` = double). Default create is `-2`. Qualifier/eval stacks use 0 (fixed; eval allocates `evaluation_stack_maximum_count` up front).
- **`afw_stack` and `array_template` removed.** Qualifier/eval stacks and `scope_stack` are vectors on xctx. Scope stack is `create_fixed_unhandled` sized to `evaluation_stack_maximum_count` (xctx init still cannot use `AFW_TRY`).

## Later

- Remaining `apr_array`: env `registry_types` / `data_type_methods`, ldap mods, index (Jeremy #299).
- **`apr_hash`:** next C tool — [`afw-hash-table.md`](afw-hash-table.md).
- Memory-array store. Align `afw_array` methods with Adaptive Script (`push`/`splice`/`for-of`) and journal-style cursors (`get_next` / `initialize_iterator`) once the C tool is in use.
