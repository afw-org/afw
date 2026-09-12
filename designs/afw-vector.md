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
- Remaining in-tree **`apr_array`** call sites converted ([PR #310](https://github.com/afw-org/afw/pull/310)): env `registry_types` / `data_type_methods`, LDAP mods, adapter index cursor lists. Index empty-list checks treat a **NULL** merge result as empty (APR `apr_is_empty_array`).

## Later

- Align memory-array methods with Script `push`/`splice`/`for-of` and journal-style cursors. Faces / wrapper / managed vs unmanaged stay; the store is already a vector of `const afw_value_t *`.
- **Hash table:** [`afw-hash-table.md`](afw-hash-table.md) landed ([PR #301](https://github.com/afw-org/afw/pull/301)). Last `apr_hash` (LMDB `dbi_handles`) converted.
- Cheap APR **strings** landed ([#314](https://github.com/afw-org/afw/issues/314) / [PR #315](https://github.com/afw-org/afw/pull/315)): own printf (`%ku` / `%ks` / `%km`, then `create`); `apr_strings.h` out of `afw_common.h`. Authority: `src/afw/utf8/afw_utf8.h`. `--scan` type-checks AFW printf (`printf_scan.py`, libclang ctypes, ~28s).
- Dead APR-Util includes dropped from `afw_common.h` and leftover from_value/file: `apr_hash.h` / `apr_buckets.h` were unused on the public substrate. Buckets stay on curl.
- C11 retarget of `AFW_*` integer/size/inline/endian macros; `afw_os_*` doors for DSO, cwd, random, time explode, fnmatch, signal. UUID generate/format/parse is portable C on `afw_os_random_bytes`.
- **Filepath + file/dir I/O** landed ([PR **#317**](https://github.com/afw-org/afw/pull/317)): `afw_file_path_*` (canonicalize / join-under-root / absolutize; `afw_os_realpath`); POSIX `afw_file_*` in `afw_file_io.c`; adapter in `afw_file_adapter.c`. Do not clone APR filepath flags or `apr_file_t`. `insure_full_path` realpath's if present, else lexical absolute. File adapter delete of a missing file throws (`not_found`).
- **Next APR sitting:** threads/locks (`afw_thread_mutex_*` still APR aliases → pthreads in `os/nix`; nested = recursive; pool cleanup) **or** GNU `getopt_long` (`afw` + command skeleton). `apr_strerror` is only the decoder while lock/thread still return APR `rv` — does not unlock dropping `apr-1`. C11 `threads.h` is not the path.
- Portability layer (not a drop-in like vector): pools (`apr_pool_*`, `apr_palloc` / `apr_pcalloc`), threads/locks, getopt, LDAP apr-util, curl brigades. Time/UUID/random/DSO/fnmatch/signal/cwd/realpath are `afw_os_*`. Path join is `afw_file_path_*`; host file I/O is `afw_file_*`.
