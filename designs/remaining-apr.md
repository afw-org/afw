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

## `get_apr_pool` callers (not the reservoir)

These still `apr_palloc` / `apr_pcalloc` through the door. Several are **xctx-init / unhandled** (no `AFW_TRY` yet) — that is why they did not become `afw_pool_calloc` in a prior sitting.

- `src/afw/environment/afw_environment.c` — env / early error
- `src/afw/xctx/afw_xctx.c` — xctx / error struct
- `src/afw/stream/afw_stream.c` — stream anchor
- `src/afw/vector/afw_vector.c` — `afw_vector_create_fixed_unhandled`
- `src/afw/os/nix/afw_os.c` (and win stub) — backtrace buffer

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
- Goal: own backing (malloc/mmap region) so AFW can account bytes and fail a request instead of APR abort-on-OOM. That is the #2 parked “non-APR heap later.”
- `get_apr_pool` can stay **lazy** until the last leftover APR API is gone, then drop from the interface.
- Unhandled xctx-init allocs: either a non-throwing AFW alloc or keep a tiny bootstrap until `AFW_TRY` exists.
- Do **not** start this in the same sitting as FRV leftover.

CMake/`afw.pc.in` still `Requires: apr, apr-util` until the reservoir and init are gone.
