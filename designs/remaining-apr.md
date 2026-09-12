# Remaining APR — pool sitting

**Audience:** maintainers. **Not** handbook.

Open a new session with **“What do you think about taking APR out of `afw_pool`?”** Read this, then [`memory-management.md`](memory-management.md) (APR relationship — some archaeology is stale), [`issue-2-lifetime.md`](issue-2-lifetime.md) (non-APR heap parked), [`afw-vector.md`](afw-vector.md) *Later*. Do **not** mix this with FRV leftover.

Containers, strings, files, threads, getopt, curl body, LDAP setup are **off APR**. What is left **is** the pool sitting. That is what can drop `apr-1`.

## Still requires APR in-process

| Piece | Where | Notes |
|-------|--------|--------|
| Heap/tracker **reservoir** | `src/afw/pool/afw_pool.c` | `apr_pool_create` / `apr_palloc` / `apr_pcalloc` / `apr_pool_destroy`. Optional free is AFW; **new chunks** still come from APR. |
| Env bootstrap | `afw_environment_create` | `apr_initialize` / `apr_terminate` |
| `afw_pool_get_apr_pool()` | pool interface | Door for leftover APR allocs, **not** the heap store. Shrink then rip from XML when the door is unused. |
| `apr_strerror` | `afw_environment_register_core.c` | Keep while any APR `rv` remains (init, `apr_pool_create` fail). |
| Public include | `afw_common.h` | `<apr_general.h>` for `apr_pool_t` / status. `<apr_time.h>` **gone** ([PR #322](https://github.com/afw-org/afw/pull/322)). |

## `get_apr_pool` door (not the reservoir)

Production leftover `apr_palloc` / `apr_pcalloc` through the door is **gone**. Those sites use `afw_pool_malloc_unhandled` / `calloc_unhandled` (NULL on failure, no throw, no MT lock).

- Env / xctx create before `current_try` + `evaluation_stack`
- Stream anchor and `afw_vector_create_fixed_unhandled` (same window)
- OS backtrace (error path; cannot throw)

The door remains for the **reservoir** (child heap `apr_pool_create` parents on `get_apr_pool(parent)`) and the pool_heap probe. Shrink then rip from XML when the reservoir is gone.

`afw_environment_create` arms its **own** jmp buf (`AFW_ERROR_INTERNAL_ON_UNHANDLED`). The host does **not** pass one. After `xctx_internal_create_initialize`, `xctx->current_try` is that buf, so throwing `afw_pool_calloc` works for the rest of create. The caller's `AFW_TRY` (`afwfcgi` / `afw`) is after create returns. Create is **one thread** until workers start — unhandled skips the MT lock (`AFW_LOCK_BEGIN` is `AFW_TRY`).

Windows-only `apr_atomic_*` in `afw_atomic.h` is not a Linux drop-`apr-1` blocker (`afw_common.h` already `#error`s Windows). Type aliases (`apr_uint64_t` LMDB, `apr_size_t` FCGI) are cheap after the reservoir is gone.

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

- Heap/tracker **kinds stay**. APR is the reservoir, not a third pool kind (`afw_pool.h`).
- **Unhandled alloc landed** (`afw_pool_malloc_unhandled` / `calloc_unhandled`). Next is the reservoir.
- Goal: own backing so AFW can account bytes and fail a request instead of APR abort-on-OOM. That is the #2 parked “non-APR heap later.”
- **Reservoir next:** heap owns a list of **4k+ chunks**. Destroy (xctx end, any heap, including the catch around the host `AFW_TRY`) walks that list and frees every chunk. All memory that came from the heap dies with it. Optional free / tracker return-to-heap stay.
- `get_apr_pool` can stay **lazy** until the reservoir is gone, then drop from the interface.
- Do **not** start this in the same sitting as FRV leftover.

CMake/`afw.pc.in` still `Requires: apr, apr-util` until the reservoir and init are gone.
