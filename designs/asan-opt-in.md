# AddressSanitizer — opt-in memory checking

**Audience:** maintainers / assistants. **Branch:** `feature/asan-opt-in` (2026-10).

## Decision

ASAN is an **optional, intentional** testing method, not a default (maintainer call, 2026-10). It never runs as part of `--cdev`, `--fulldev`, `--all` or the pre-PR gate; CI runs it as a non-blocking job (below). You get it only by asking for it, and an ASAN build must not change a normal build, a normal install, or the plain / valgrind test modes. Revisit only by consensus.

**CI (2026-10-05, maintainer request):** `integration.yml` has a `build_test_c_asan_ubuntu` job (`./afwdev build --cdev --sanitize address`, then `./afwdev test -j --env-mode asan`). It is **non-blocking** (`continue-on-error: true`) while the known findings are open; remove that once the ASan run is clean. Ubuntu only (the `afw-dev-base` images carry the ASan/UBSan runtimes; Alpine has none). It is still never part of `--cdev`, `--fulldev` or `--all` locally.

## Why the pools need annotations

Heaps carve mapped chunks themselves (`afw_memory_region_get` → `afw_os_map_pages`). To ASAN (and to valgrind) a chunk is one valid block, so a read of freed pool memory, an overflow into the next block, or a read of a released pool's chunk is invisible. Proven on this branch: three deliberate pool bugs (read after free, one-byte overflow, read after `afw_pool_release`) give **no report** under plain ASAN, and a use-after-poison report on the exact line with the annotations.

`AFW_DEBUG_POOL` (prefix check + fill on free) is the existing in-house net. It catches a bad free, not a bad read. ASan complements it rather than replacing it; see *Relationship to `AFW_DEBUG_POOL`*.

## What landed (step 1 of the plan: annotations)

`src/afw/memory/afw_memory_annotate_internal.h`: `AFW_MEMORY_ANNOTATE_NOACCESS` / `ACCESS` (ASAN poison / unpoison) and `ROOT` / `UNROOT` (LeakSanitizer root regions). Active only under `__SANITIZE_ADDRESS__` or clang `__has_feature(address_sanitizer)`; otherwise no-ops, so a normal build compiles to the same code.

| Where | Marks |
|-------|-------|
| `afw_memory_region.c` | `free` resets the chunk; a cached chunk is no-access past its 16-byte node; a hit is accessible again; every unmap clears marks first |
| `afw_pool_heap.c` | new chunk: usable area no-access, chunk a leak root while the heap holds it; a carved block is accessible; slack after the asked-for size is no-access; a free block is no-access past its 32-byte free-node header |
| `afw_pool_tracker.c` | slack after the block no-access; a marked free makes USER no-access (made accessible again for the destroy-time fill) |
| `afw_pool_heap_internal.h` | ASAN without `AFW_DEBUG_POOL` widens the heap prefix to a free node, so the free-list overlay never covers USER |

Deliberately left accessible: allocator headers, and a header that a coalesce absorbs — a second free of that block reads its free bit (`pool_heap` `double_free_throws` after coalesce).

**Leak roots:** LeakSanitizer does not scan mapped pages. Without the roots, C memory only pool memory points to (base thread, regions, their mutexes) reports as leaked at every exit.

**Probe:** `src/afw/tests/advanced/pool_asan/` asks ASAN which bytes are marked (`__asan_address_is_poisoned`), so it passes or fails without crashing. It skips every case against a normal `libafw`. `run_c_probe` now detects an ASAN / UBSan `libafw` (`libafw_sanitizers()`) and builds probes with the same `-fsanitize`, so any probe works against an ASAN prefix. `pool_heap` skips `debug_free_wrong_pool` and `debug_free_poisons_user` under ASAN (they read no-access memory on purpose; ASAN reports it first).

## How to run it

```bash
./afwdev build --cdev --sanitize address      # build/asan/cmake/, ~50s
./afwdev test -j --env-mode asan              # ~6 min (normal run ~20s)
```

`--env-mode asan` (`_afwdev/test/sanitize.py`, `modes/asan.py`):

- Always runs against the build tree `build/asan/cmake/` (same setup as `--build-tree`, below): missing, or not an ASan build, is an error with the build command. A stamp from another commit, or from a dirty tree, is a warning with the same command (docs-only commits should not force a rebuild).
- The tree's `afw` / `afwfcgi` come first on `PATH` and every library dir is on `LD_LIBRARY_PATH` for the whole run, so `.as` tests, python tests, orchestrated `afwfcgi` and `afw --local` all use the ASan build. C probes build against the tree (`run_c_probe` adds the matching `-fsanitize`).
- `ASAN_OPTIONS` adds `detect_odr_violation=0:detect_leaks=1`; `UBSAN_OPTIONS` adds `print_stacktrace=1:halt_on_error=1`. Keys you set yourself win. Every sanitizer report fails the process that hit it; `.as` failures show a short summary (report line plus top frames).
- `afwfcgi` starts without `stdbuf` under asan: its `LD_PRELOAD` loads ahead of the ASan runtime.
- `--env-mode valgrind` refuses a sanitizer `libafw`; the `c_probe` self-test skips its valgrind cases against one.
- `pool_heap.py` finds the right `CMakeCache.txt` through the stamp (`libafw_build_cache()`).
- History and failure logs use the mode name, so asan runs never mix with afw or valgrind baselines.

## Findings so far

Recorded with cause and fix in [`beta-backlog.md`](../beta-backlog.md) → *ASAN findings*. Fixed and on `develop` (2026-10-05): `sort()` overflow (#462), flag `memcpy` (#463), test-runner hang (#464), model `current::` metadata (#465), pool cleanup order (#466), stale probe releases (#467), and the compile-literal lifetime cases (#470). Open: three UBSan findings (`memcmp` with NULL in the memory data-type compare, a misaligned LMDB journal key load, signed overflow before the check in integer `multiply`). After rebasing on `develop` `34f4eed9`, `./afwdev test -j --env-mode asan` fails exactly those 7 tests (4456 passed). Every pool finding also crashed a normal build once `memoryRegionFreeListMaxBytes` was 0; the default region cache is what hid them.

**Before calling a report a bug:** with `AFW_DEBUG_POOL` the 32 bytes before a block start are `[chunk][…][size][pool]`. Free bit clear plus the owning heap's self no-access means the pool was released (region-cached chunk), not an annotation slip. gdb at `__asan_report_load8` reads memory without tripping ASAN.

## Cheap check: region cache off (no ASAN)

`memoryRegionFreeListMaxBytes = 0` makes every released heap chunk `munmap` at once, so reading a released pool faults on a normal build. Tried 2026-10-03 on `develop` with `AFW_MEMORY_REGION_FREE_LIST_MAX_BYTES` set to 0 in a scratch build (it is a plain `#define`; no `--define` override today, and tests without an `afw.conf` cannot set it): full `test -j` in about 15s. It caught every read of a released pool the ASAN run found (pool cleanup order: both authorization tests plus `catalog-value-accessors` in `afwfcgi`; all three deferred compile-literal cases) and a stale double release in the `compiled_value_managed` probe. It cannot see an overflow inside a live chunk (`sort`), a global overread (model `current::`), or UBSan findings. Expected failures: `miscellaneous/process.as` asserts the default cap and region hits. Gotcha: `pool_heap.py` reads `build/cmake/CMakeCache.txt` of the tree it runs from to pick `AFW_DEBUG_POOL`; a scratch build needs that path to point at its cmake dir. Making this a real option (an `#ifndef` around the default, or a process-level override) is a separate decision.

## Relationship to `AFW_DEBUG_POOL`

Question (2026-10-04): does ASan replace `AFW_DEBUG_POOL`, so the macro and its code can go? Answer: **no, only one of its three features overlaps.** Keep it; ASan complements it.

| `AFW_DEBUG_POOL` feature | ASan equivalent? |
|---|---|
| **1. `{size, pool}` prefix checked on every free** (`afw_pool_internal_debug_check_prefix`, `pool/afw_pool.c`): `afw_pool_free_memory` throws "pool does not match allocation" / "size does not match allocation", a catchable error in every `--cdev` run at normal speed | **No.** ASan does not know the size or pool passed to `afw_pool_free_memory`. A free into the wrong heap or with the wrong size corrupts free lists while every byte involved still looks valid to it. |
| **2. Freed USER filled with `0x0BADF00D`**: a dangling `inf` faults on first use; the pattern is recognizable in gdb and core dumps | **Only in ASan runs**, where it is strictly better (exact read reported, plus slack and released chunks). ASan is opt-in and ~17× slower, so the fill is still the fast signal in everyday `--cdev` runs. |
| **3. `debug:pool` / `debug:pool:detail` trace flags** (create / release / destroy, plus every alloc / free at `:detail`, with `in_use`, totals, RSS, refs, parent) | **No.** ASan finds errors; it does not trace pool lifetime or accounting. The #2 RSS lab depends on these lines (`tests-extra/issue-2/01-rss-hard-loops/README.md`, `_trace.py`). |

The prefix also made ASan's reports usable. ASan reports a bad read in pool memory as `use-after-poison` with no allocation or free stack. Every diagnosis in *Findings so far* came from reading `[chunk][size][pool]` before the block in gdb: live or freed, and whether the owning heap had been released. Without `AFW_DEBUG_POOL` that would have been much harder.

If anything is trimmed, only the fill (2) is a candidate, and only if ASan becomes routine rather than opt-in.

## Build design (`afwdev build --sanitize`)

Decided with the maintainer, 2026-10-04.

- **Flag:** `--sanitize <variant>`. `address` is the only value accepted now and builds `-fsanitize=address,undefined` (UBSan combines with any variant, so it always rides along; LeakSanitizer comes with `address` and is switched at run time). Other values are rejected with the reason. Accepting a value promises a working path (build, run, reports you can trust), not just compiler flags.
- **Variants:** `thread` maybe later, after a trial run shows TSan is usable on AFW (its own `build/tsan/`). `memory` is not planned: MSan needs every dependency rebuilt with it, and AFW takes OpenSSL, ICU, libxml2, curl, LMDB, LDAP and yaml from the OS. Valgrind on the normal build already covers uninitialized reads. `address` works with uninstrumented OS libraries (no false reports; it just does not see bugs inside them).
- **Layout** (one directory per variant, a sibling of `build/cmake/`):

  ```
  build/
  ├── cmake/            normal build tree (--cdev); installs to /usr/local or --prefix
  └── asan/
      ├── cmake/        build tree
      └── install/      prefix, only with an explicit --install
  ```

- **No install by default** (changed 2026-10-04): `--env-mode asan` tests `build/asan/cmake/` directly, so `--sanitize` installs only with an explicit `--install` (not the one `--cdev` implies), into `build/asan/install/` or `--prefix`. It never touches `/usr/local`. The prefix is still configured: it is the baked-in fallback for extension loading, so a missing `LD_LIBRARY_PATH` entry fails loudly instead of loading a non-ASan extension from `/usr/local`.
- **Clean:** `afwdev build --clean` removes only `build/<context>/` for the contexts in that run (`build.py`), so `--cdev --clean` / `--fulldev` leave `build/asan/` alone. `--sanitize --clean` removes only `build/asan/` (the cmake dir, and the install dir unless `--prefix` was given; a custom prefix is never removed). A manual `rm -rf build` removes it too; the test mode then says how to rebuild.
- **Refused with it:** `--fulldev`, `--all`, `--docs`, `--js`, `--docker`, `--package`, `--scan`. It is the C (cmake) context only; `--fulldev` would also turn on `--scan` and every context.
- **Never implied:** `--cdev`, `--fulldev` and `--all` do not add `--sanitize` (same rule as `--docker`). It combines with `--cdev`: `./afwdev build --cdev --sanitize address` = generate, clean `build/asan/`, build, install (about 50s).
- **Flags reach cmake** as `-DAFWDEV_SANITIZE=address;undefined` (like `AFWDEV_C_DEFINES`); the root `CMakeLists.txt` adds the compile and link options plus `-fno-omit-frame-pointer`.
- **Stamp file** `afwdev-sanitize.json` in `build/asan/cmake/` (sanitizers, source commit, dirty, build time, build dir); `--env-mode asan` warns when it is from another commit or a dirty tree.
- **No runtime:** fail before cmake when the compiler cannot link a `-fsanitize=address` program (Alpine/musl has no ASan runtime).
- **Hard-coded `build/cmake` to fix:** the install-prefix helper and the header prune after install (must use the ASAN cache and prefix, never prune `/usr/local`); the clangd `compile_commands.json` symlink stays on `build/cmake/`; `pool_heap.py` `_lib_has_debug_pool()` (test mode, step 3).

## Build and test-mode compatibility

| Test run | Build it needs |
|---|---|
| `test -j` (default), `--env-mode valgrind` | normal (`build/cmake/`, `/usr/local`). Valgrind cannot run an ASan process (fails at start). |
| `--env-mode asan` | `build/asan/cmake/` (always the build tree), required to exist first; never built implicitly |
| region cache off | normal build if it becomes a runtime switch; a separate build if it stays a compile-time default |

Rules: the default test run never picks up a sanitizer build; a mode refuses a mismatched build with one clear message; tests that are incompatible by design skip with a reason. Minimum builds for a full CI run: 2 (normal, ASan+UBSan); +1 each for TSan or a production-style build without the `AFW_DEBUG_*` defines.

## Testing from the build tree (`afwdev test --build-tree`)

Decided 2026-10-04. Tests normally run whatever was last installed (`/usr/local`), which can lag the source. `--build-tree` runs any `--env-mode` against a cmake build tree instead: `build/cmake/` for the default and valgrind modes, `build/asan/cmake/` for asan. Each mode is then tied to its own build and no install is needed to test.

What it sets for the whole run (children inherit it):

- `PATH`: the tree's directories holding `afw` / `afwfcgi`, plus a link to the repo's `./afwdev` (python tests run `afwdev` by name).
- `LD_LIBRARY_PATH`: every tree directory holding a `lib*.so`. **Required**, not optional: `afw_environment_load_extension` tries a bare `dlopen` first and then the baked-in `AFW_CONFIG_INSTALL_FULL_LIBDIR`, so without it a build-tree `afw` silently loads extensions from the installed prefix (seen with `afw_lmdb`).
- C probes: `AFW_LIB_DIR` (the tree's `libafw`), `AFW_LIB_DIRS` (every library dir, for `-l<extension>`), and `AFW_INCLUDE_DIRS` (the `-I` dirs from the tree's `compile_commands.json`; a tree has no flattened include dir).

Landed 2026-10-04 (`_afwdev/test/build_tree.py`). Results: default mode 4591 passed, 0 failed, about 31s (same as the install); `--env-mode asan --build-tree` 4361 passed, 23 failed, about 357s, where the 23 are exactly the known findings; valgrind mode uses `build/cmake/` and passes. The tree must be a match for the mode: asan needs `AFWDEV_SANITIZE` with `address` in the tree's `CMakeCache.txt`; any other mode refuses a sanitizer tree.

Gotcha: the repo's `./afwdev` only works from the repo root (it runs `src/afw_dev/afwdev.py` by relative path). Tests run `afwdev` from other directories (the `commands_test1.txt` group builds a throwaway package in `/tmp`), so `--build-tree` writes a wrapper, `build/<tree>/afwdev-bin/afwdev`, that runs `afwdev.py` by absolute path. A plain symlink to `./afwdev` made those commands fail and the parallel run hang.

**Decided (2026-10-04):** the default stays as before: `afwdev test` runs the installed binaries and libraries from the system path. `--build-tree` is opt-in only. `--env-mode asan` always uses its build tree (decided 2026-10-04): the ASan build is never installed system-wide, so `build/asan/install` is no longer made by default.

## Remaining plan

Flexible order; one step, then re-decide.

1. ~~Annotations~~ (this branch).
2. ~~Build:~~ `afwdev build --sanitize address` landed (2026-10-04); see *Build design*.
3. ~~Test:~~ `afwdev test --env-mode asan` landed (2026-10-04); see *How to run it*. First full run: 4346 passed, 29 failed. The failures were harness gaps (since fixed), the known findings (#466, #467, the deferred compile-literal cases) and three new UBSan findings (backlog).
4. **Later / separate decisions:** a valgrind backing for the same header behind its own define (changes what the existing valgrind mode reports); a reuse delay (quarantine) for the heap free list so a same-size malloc does not hide a use-after-free. (UBSan halts: decided with step 3.)

## Footguns

- **ODR report on extension load:** every extension exports `afw_environment_extension_instance` and is opened `RTLD_NOW | RTLD_GLOBAL` (`os/nix/afw_os.c`); the loader finds it with `dlsym` on the handle, so it works, but ASAN's ODR check aborts. The test mode should set `detect_odr_violation=0` unless we change symbol visibility.
- **ASAN and valgrind do not mix.** A valgrind run against an ASAN prefix is a user error.
- **C stack headroom:** ASAN frames are larger; no early `C stack headroom exhausted` seen in the first run, but deep-recursion tests are where it would show.
- **Alpine:** no ASAN runtime for musl ([`docker-cross-platform-builds.md`](docker-cross-platform-builds.md)).
- **Non-ASAN binary + ASAN `libafw`** fails at start (`ASan runtime does not come first`). Probes get matching flags from `run_c_probe`; anything else must be built with the same flags or `LD_PRELOAD` the runtime.
- A Python test whose `run()` raises hangs `afwdev test` (pre-existing; backlog *afwdev test bugs*). Use `./afwdev` in the repo, not a stale PATH copy.
