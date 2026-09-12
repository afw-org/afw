# Remaining APR — pool sitting

**Audience:** maintainers. **Not** handbook.

Pool store is off APR on `reduce-apr-pool`. Next sitting: drop `apr_initialize` and the lazy `get_apr_pool` door. Read this, then [`memory-management.md`](memory-management.md) (APR relationship — some archaeology is stale), [`issue-2-lifetime.md`](issue-2-lifetime.md), [`afw-vector.md`](afw-vector.md) *Later*. Do **not** mix this with FRV leftover.

Containers, strings, files, threads, getopt, curl body, LDAP setup, and the **pool store** are **off APR**. What is left is `apr_initialize` + the lazy `get_apr_pool` door. That is what can drop `apr-1`.

## Still requires APR in-process

| Piece | Where | Notes |
|-------|--------|--------|
| Env bootstrap | `afw_environment_create` | `apr_initialize` / `apr_terminate` |
| `afw_pool_get_apr_pool()` | pool interface | Lazy door for leftover APR. Heap store is 4k-aligned chunks. Rip from XML when unused. |
| `apr_strerror` | `afw_environment_register_core.c` | Keep while any APR `rv` remains (init, door `apr_pool_create` fail). |
| Public include | `afw_common.h` | `<apr_general.h>` for `apr_pool_t` / status. `<apr_time.h>` **gone** ([PR #322](https://github.com/afw-org/afw/pull/322)). |

## `get_apr_pool` door (not the reservoir)

Production leftover `apr_palloc` / `apr_pcalloc` through the door is **gone**. Those sites use `afw_pool_malloc_unhandled` / `calloc_unhandled` (NULL on failure, no throw, no MT lock).

- Env / xctx create before `current_try` + `evaluation_stack`
- Stream anchor and `afw_vector_create_fixed_unhandled` (same window)
- OS backtrace (error path; cannot throw)

The door remains for the pool_heap probe (and any leftover APR call). Shrink then rip from XML when `apr_initialize` is gone.

`afw_environment_create` arms its **own** jmp buf (`AFW_ERROR_INTERNAL_ON_UNHANDLED`). The host does **not** pass one. After `xctx_internal_create_initialize`, `xctx->current_try` is that buf, so throwing `afw_pool_calloc` works for the rest of create. The caller's `AFW_TRY` (`afwfcgi` / `afw`) is after create returns. Create is **one thread** until workers start — unhandled skips the MT lock (`AFW_LOCK_BEGIN` is `AFW_TRY`).

Windows-only `apr_atomic_*` in `afw_atomic.h` is not a Linux drop-`apr-1` blocker (`afw_common.h` already `#error`s Windows). Type aliases (`apr_uint64_t` LMDB, `apr_size_t` FCGI) are cheap after init/door are gone.

## Already done (do not re-litigate)

| Sitting | What |
|---------|------|
| Vector / hash | `afw_vector`, `afw_hash_table` |
| Strings / printf | AFW `%ku` / `%ks` / `%km` |
| C11 + `afw_os_*` | DSO, time, random, fnmatch, signal, cwd, realpath, UUID |
| Filepath + file I/O | `afw_file_path_*` / `afw_file_*` |
| Threads | pthreads via `afw_os_*`; public `afw_thread_*` |
| Curl body | `afw_memory_create_writer` + chunk `memcpy` + `retrieve_and_release` (UBJSON shape). Not `afw_utf8_writer`. [PR #319](https://github.com/afw-org/afw/pull/319) |
| Getopt | `afw_getopt_*` **in libafw** (table + help + switch). Skeleton `find_package(afw)` picks **newest** cmake config (`lib/cmake/afw-*` accumulate). [PR #320](https://github.com/afw-org/afw/pull/320) |
| LDAP setup | OpenLDAP `ldap_url_parse` / `ldap_initialize` / `ldap_set_option`. Search/bind/CRUD were already `ldap_*`. `ldap_free_urldesc` after parse; `LDAP_DEPRECATED` for `ldap_*_s`. [PR #321](https://github.com/afw-org/afw/pull/321) |
| Dead time | `from_apr_time` and `parser->apr_p` gone. [PR #322](https://github.com/afw-org/afw/pull/322) |

`afwfcgi` argv is still a strcmp loop (never APR getopt).

## Pool sitting rails (discuss first)

- Heap/tracker **kinds stay**. Not a third AFW pool kind (`afw_pool.h`).
- **Unhandled alloc landed** (`afw_pool_malloc_unhandled` / `calloc_unhandled`).
- **Reservoir is AFW chunks** (on `reduce-apr-pool`): `posix_memalign` 4k-aligned regions (size a 4k multiple), bump then free-list, no coalesce across chunks. Destroy (xctx end, any heap, including the host `AFW_FINALLY`) walks `first_chunk` and `free()`s every chunk. Child heaps are on `first_child` (own chunks; `parent` stays NULL). The heap self lives in a chunk. `malloc` NULL is Adaptive `memory` / unhandled NULL, not APR abort.
- `get_apr_pool` is a **lazy independent door**. Drop from the interface after `apr_initialize` is gone.
- Do **not** mix with FRV leftover.

CMake/`afw.pc.in` still `Requires: apr, apr-util` until init and the door are gone. Tune later: mmap, chunk size, per-chunk free lists.
