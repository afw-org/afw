# Remaining APR

**Audience:** maintainers. **Not** handbook.

libafw no longer uses APR. Heap store is 4k-aligned `posix_memalign` chunks. `get_apr_pool` and `apr_initialize` are gone. Do **not** mix leftover docs with FRV leftover.

Containers, strings, files, threads, getopt, curl body, LDAP setup, and the pool store are **off APR**.

## Dropped in this sitting (`reduce-apr-pool`)

| Piece | What happened |
|-------|----------------|
| `apr_initialize` / `apr_terminate` | Removed from `afw_environment_create` |
| `afw_pool_get_apr_pool()` | Ripped from the pool interface XML |
| `apr_strerror` RV decoder | Gone with the APR `rv` source id |
| `<apr_general.h>` | Out of `afw_common.h` |
| CMake / `afw.pc` | No `Requires: apr, apr-util`; no `find_package(AFW_APR)` |
| Type aliases | LMDB `apr_uint32_t` / `apr_uint64_t` → `afw_uint*`; FCGI `apr_size_t` → `afw_size_t` |
| Windows `apr_atomic_*` | `InterlockedIncrement` / `Decrement` |

`afw_environment_release` still does not destroy the process base pool (`@fixme`). Process-lifetime chunks stay reachable via a static root, so valgrind should report them as **still reachable**, not definitely lost.

Per-eval `xctx->evaluation_heap` (and object-option heaps) are child heaps with their own chunks. When a parent heap dies, leftover child heaps are unlinked and their chunks `free()`d recursively (APR used to free those reservoirs when the parent APR pool died). AFW destroy/cleanups are not run on those leftovers. Trackers still live in the parent heap's chunks.

Tune later: mmap, chunk size, per-chunk free lists.

## Already done (do not re-litigate)

| Sitting | What |
|---------|------|
| Vector / hash | `afw_vector`, `afw_hash_table` |
| Strings / printf | AFW `%ku` / `%ks` / `%km` |
| C11 + `afw_os_*` | DSO, time, random, fnmatch, signal, cwd, realpath, UUID |
| Filepath + file I/O | `afw_file_path_*` / `afw_file_*` |
| Threads | pthreads via `afw_os_*`; public `afw_thread_*` |
| Curl body | `afw_memory_create_writer` + chunk `memcpy` + `retrieve_and_release` |
| Getopt | `afw_getopt_*` in libafw |
| LDAP setup | OpenLDAP `ldap_*` |
| Dead time | `from_apr_time` gone |
| Unhandled alloc | `afw_pool_malloc_unhandled` / `calloc_unhandled` |
| Reservoir | 4k `posix_memalign` chunks; destroy walks `first_chunk` |

`afwfcgi` argv is still a strcmp loop (never APR getopt).
