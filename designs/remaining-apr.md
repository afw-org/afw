# Remaining APR

**Audience:** maintainers. **Not** handbook.

libafw no longer uses APR. Heap store is 64k-min, 4k-aligned `posix_memalign` chunks. `get_apr_pool` and `apr_initialize` are gone. Do **not** mix leftover docs with FRV leftover.

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

`afw_environment_release` does not destroy the process base pool (intended: process lifetime; MT lock lives in it). Process-lifetime chunks stay reachable via a static root, so valgrind should report them as **still reachable**, not definitely lost.

Heap and tracker use the same parent/child RC. Last-`release` does not call `destroy`: decrement, throw if children remain, then cleanup (callbacks, unchain, free this store, `release` parent). **`destroy` is storage-only** (must not fail): unchain, leftover, free store, `release` parent. Call **`afw_pool_run_cleanups`** first if callbacks must run (`xctx_release` does both). `destroy` clears delayed last-`release` marks. Callers must own that subtree (`xctx->p`, flag/log scratch pools, …). Heap: `release` parent before `free_chunks` (`xctx` lives in `xctx->p`). `afw_pool_release_delayed()` is a postorder last-`release` of delayed pools at ENDTRY after a caught error. Child heaps keep their own chunks (`impl_reservoir_heap` stops at a heap).

## Pool doors (live)

| Door | What |
|------|------|
| `afw_pool_heap_create(parent, chunk_min, xctx)` | ST heap, `managed_p = self`. `chunk_min` 0 → 64k. xctx/thread heaps. Compile units use **4k**. |
| `afw_pool_multithread_create(env->p)` | MT job heap, `managed_p = self`. Conf, adapter, server, log. |
| `afw_pool_create(parent)` | Tracker if ST parent; MT heap if MT parent. Parent/child is **lifetime** only; store is the ancestor heap. |

One ST heap per xctx (`xctx->p`). Scopes are trackers of that heap, not of the enclosing `{ }` (closures pin the inner tracker). No `evaluation_heap`. Managed values allocate in `p->managed_p` (job heap for this eval; do not swap mid-eval). Request: `xctx->p->managed_p` is `xctx->p`. `create_managed` takes `p`. Last-release of managed object/array uses `self->pub.p`. Evaluate of a compiled value **clones onto the caller’s `p`**.

Process/server runtime objects expose live `poolBytesInUse` / `maxPoolBytesInUse` / `poolChunkBytes` / `maxPoolChunkBytes` (`env_pool_stat`).

Tune later: mmap, per-chunk free lists.

## Next: [PR #327](https://github.com/afw-org/afw/pull/327) `reduce-apr-pool` → `develop`

FRV leftover wrapping is **dropped** (squash [PR #326](https://github.com/afw-org/afw/pull/326)). Do **not** reopen unique consume, eval-stack leftover FRV, `#function_return_value`, or a call-result leftover inf. Do **not** put callbacks back on `destroy` to hide SIGSEGV.

**Verify** (2026-09-14): `./afwdev build --fulldev`, `afwdev test -j`, `afwdev test -j --env-mode valgrind` — **4484 passed**, 71 skipped.

C API notes: `whats-new.md` (`run_cleanups` / storage-only `destroy`, `register_cleanup`, `get_assignable_for_scope_lifetime` / `for_p_lifetime`, 64k-min chunks).

**Not blocking:** process base pool is process lifetime (valgrind **still reachable**). mmap / per-chunk free lists.

**Keep:** last-`release` runs callbacks then teardown. `destroy` storage-only. `xctx_release` `TRY` streams then `run_cleanups` `FINALLY` `destroy` (return before `ENDTRY`; `xctx` lives in `xctx->p`). Host FINALLY must catch adapter cache commit so `xctx_release` still runs. Mark whole subtree `destroying` before callbacks; leftover/free after; detach `first_cleanup` before walking (nested last-`release` must not re-enter). Closures are managed (`inf->is_managed`); pin on any scope `p`. Dual-face object/array: one instance RC; unmanaged **value** `get_reference` / `release` **throw**. `get_assignable` of managed is `get_reference` of self; unmanaged often `clone_managed`. Copy compile-eval results out of the unit pool before last-releasing `compiled`.

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
| Reservoir | 64k-min, 4k-aligned `posix_memalign` chunks; destroy walks `first_chunk` |

`afwfcgi` argv is still a strcmp loop (never APR getopt).
