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
- **Copy-out:** `afw_vector_copy_entries` mallocs exact `count * entry_size` in dest `p`. `afw_vector_release` `free_memory`s the work header and chunk (no `get_reference`). Compile args use `copy_entries_and_release` in `parser->p` — no child pool per list. `afw_vector_append` copies n entries. First `apr_array` swaps after compile args: utf8 writer/stream, compiler listing, query-criteria select/list values.

## Later (not this pass)

- Remaining `apr_array`: xctx `scope_stack`, env registry/methods, ldap mods, index (Jeremy #299). Hashes still use `parser->apr_p`.
- Fold `afw_stack` onto vector if eval/qualifier stacks can live with relocating `entries` (today they cannot if callers hold entry pointers).
- Memory-array store. Align `afw_array` methods with Adaptive Script (`push`/`splice`/`for-of`) and journal-style cursors (`get_next` / `initialize_iterator`) once the C tool is in use.
