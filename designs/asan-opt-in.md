# AddressSanitizer — opt-in memory checking

**Audience:** maintainers / assistants. **Branch:** `feature/asan-opt-in` (2026-10).

## Decision

ASAN is an **optional, intentional** testing method, not a default (maintainer call, 2026-10). It never runs as part of `--cdev`, `--fulldev`, `--all`, the pre-PR gate, or CI. You get it only by asking for it, and an ASAN build must not change a normal build, a normal install, or the plain / valgrind test modes. Revisit only by consensus.

## Why the pools need annotations

Heaps carve mapped chunks themselves (`afw_memory_region_get` → `afw_os_map_pages`). To ASAN (and to valgrind) a chunk is one valid block, so a read of freed pool memory, an overflow into the next block, or a read of a released pool's chunk is invisible. Proven on this branch: three deliberate pool bugs (read after free, one-byte overflow, read after `afw_pool_release`) give **no report** under plain ASAN, and a use-after-poison report on the exact line with the annotations.

`AFW_DEBUG_POOL` (prefix check + fill on free) is the existing in-house net. It catches a bad free, not a bad read.

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

## Manual recipe (until step 2 / 3)

```bash
S=/path/to/scratch   # anywhere outside the normal prefix
./afwdev build --cdev    # generate + normal install as usual
cmake -S . -B $S/cmake-asan \
  -DAFWDEV_C_DEFINES="AFW_DEBUG_EVALUATION;AFW_DEBUG_LOCK;AFW_DEBUG_POOL" \
  -DCMAKE_C_FLAGS="-fsanitize=address,undefined -fno-omit-frame-pointer -g" \
  -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=address,undefined" \
  -DCMAKE_SHARED_LINKER_FLAGS="-fsanitize=address,undefined" \
  -DCMAKE_MODULE_LINKER_FLAGS="-fsanitize=address,undefined" \
  -DCMAKE_INSTALL_PREFIX=$S/asan-prefix
cmake --build $S/cmake-asan --parallel && cmake --install $S/cmake-asan

# probes against the ASAN prefix
AFW_LIB_DIR=$S/asan-prefix/lib/afw AFW_INCLUDE_DIR=$S/asan-prefix/include/afw \
  ./afwdev test --test-pattern 'pool_asan|pool_heap|pool_alloc'
```

Whole suite, rough: ASAN `afw` first on `PATH`, `ASAN_OPTIONS=detect_leaks=0:detect_odr_violation=0`. First run (2026-10-03): 4429 passed, 42 failed, about 350s against 20s. Failures were harness wiring (`afwfcgi` and `c_probe` self-tests start non-ASAN binaries against the ASAN lib) plus the real findings below.

## Findings so far

Recorded with cause and fix in [`beta-backlog.md`](../beta-backlog.md) → *ASAN findings* (all diagnosed, 2026-10-03). Fixed, each on its own branch off `develop` (not yet committed): `sort()` writes one pointer past its block; pool last-release destroys children before running cleanups (double teardown on adapter restart); model `current::` runtime objects scanned the model context as a property list (generator now enforces `indirect`); UBSan `memcpy` from NULL in flag registration; and a test-runner hang when a Python test raises. Deferred to the #2 memory-management work: compile literals of a nested unit (`evaluate(compile(...))`, `eval_from_file`) outliving that unit - through a returned array, a returned scalar, and objects a model hook returns. Every pool finding also crashes a normal build once `memoryRegionFreeListMaxBytes` is 0; the default region cache is what hides them.

**Before calling a report a bug:** with `AFW_DEBUG_POOL` the 32 bytes before a block start are `[chunk][…][size][pool]`. Free bit clear plus the owning heap's self no-access means the pool was released (region-cached chunk), not an annotation slip. gdb at `__asan_report_load8` reads memory without tripping ASAN.

## Cheap check: region cache off (no ASAN)

`memoryRegionFreeListMaxBytes = 0` makes every released heap chunk `munmap` at once, so reading a released pool faults on a normal build. Tried 2026-10-03 on `develop` with `AFW_MEMORY_REGION_FREE_LIST_MAX_BYTES` set to 0 in a scratch build (it is a plain `#define`; no `--define` override today, and tests without an `afw.conf` cannot set it): full `test -j` in about 15s. It caught every read of a released pool the ASAN run found (pool cleanup order: both authorization tests plus `catalog-value-accessors` in `afwfcgi`; all three deferred compile-literal cases) and a stale double release in the `compiled_value_managed` probe. It cannot see an overflow inside a live chunk (`sort`), a global overread (model `current::`), or UBSan findings. Expected failures: `miscellaneous/process.as` asserts the default cap and region hits. Gotcha: `pool_heap.py` reads `build/cmake/CMakeCache.txt` of the tree it runs from to pick `AFW_DEBUG_POOL`; a scratch build needs that path to point at its cmake dir. Making this a real option (an `#ifndef` around the default, or a process-level override) is a separate decision.

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
      └── install/      prefix: bin/, lib/afw/, include/afw/, and the stamp file
  ```

  The prefix is installed, not run from the build tree: binaries find their libraries through a relative rpath, and the baked-in install path makes `afw` load extensions from its own `lib/afw`.
- **Install is implied:** `--sanitize` installs into its own prefix (`--prefix` overrides). It never touches `/usr/local`.
- **Clean:** `afwdev build --clean` removes only `build/<context>/` for the contexts in that run (`build.py`), so `--cdev --clean` / `--fulldev` leave `build/asan/` alone. `--sanitize --clean` removes only `build/asan/`. A manual `rm -rf build` removes it too; the test mode then says how to rebuild.
- **Refused with it:** `--docker`, `--package`, `--scan`.
- **Never implied:** `--cdev`, `--fulldev` and `--all` do not add `--sanitize` (same rule as `--docker`). It combines with them: `./afwdev build --cdev --sanitize address` = generate, clean `build/asan/`, build, install.
- **Flags reach cmake** as `-DAFWDEV_SANITIZE=address;undefined` (like `AFWDEV_C_DEFINES`); the root `CMakeLists.txt` adds the compile and link options plus `-fno-omit-frame-pointer`.
- **Stamp file** in the prefix (sanitizers, source commit) so the test mode can refuse a missing or stale build with the exact command.
- **No runtime:** fail before cmake when the compiler cannot link a `-fsanitize=address` program (Alpine/musl has no ASan runtime).
- **Hard-coded `build/cmake` to fix:** the install-prefix helper and the header prune after install (must use the ASAN cache and prefix, never prune `/usr/local`); the clangd `compile_commands.json` symlink stays on `build/cmake/`; `pool_heap.py` `_lib_has_debug_pool()` (test mode, step 3).

## Build and test-mode compatibility

| Test run | Build it needs |
|---|---|
| `test -j` (default), `--env-mode valgrind` | normal (`build/cmake/`, `/usr/local`). Valgrind cannot run an ASan process (fails at start). |
| `--env-mode asan` (step 3) | `build/asan/install/`, required to exist first; never built implicitly |
| region cache off | normal build if it becomes a runtime switch; a separate build if it stays a compile-time default |

Rules: the default test run never picks up a sanitizer build; a mode refuses a mismatched build with one clear message; tests that are incompatible by design skip with a reason. Minimum builds for a full CI run: 2 (normal, ASan+UBSan); +1 each for TSan or a production-style build without the `AFW_DEBUG_*` defines.

## Remaining plan

Flexible order; one step, then re-decide.

1. ~~Annotations~~ (this branch).
2. **Build:** `afwdev build --sanitize address`. Decided 2026-10-04; see *Build design* below.
3. **Test:** `afwdev test --env-mode asan` (`modes/asan.py`, like `valgrind.py`): ASAN prefix first on `PATH` / lib path, `ASAN_OPTIONS` / `UBSAN_OPTIONS`, short summary of `==ERROR: AddressSanitizer` / `runtime error:`, its own history mode suffix, a clear error if the ASAN build is missing. Must also cover `afwfcgi` orchestration and the `c_probe` self-tests (both started non-ASAN binaries in the first run).
4. **Later / separate decisions:** a valgrind backing for the same header behind its own define (changes what the existing valgrind mode reports); a reuse delay (quarantine) for the heap free list so a same-size malloc does not hide a use-after-free; UBSan halt vs report.

## Footguns

- **ODR report on extension load:** every extension exports `afw_environment_extension_instance` and is opened `RTLD_NOW | RTLD_GLOBAL` (`os/nix/afw_os.c`); the loader finds it with `dlsym` on the handle, so it works, but ASAN's ODR check aborts. The test mode should set `detect_odr_violation=0` unless we change symbol visibility.
- **ASAN and valgrind do not mix.** A valgrind run against an ASAN prefix is a user error.
- **C stack headroom:** ASAN frames are larger; no early `C stack headroom exhausted` seen in the first run, but deep-recursion tests are where it would show.
- **Alpine:** no ASAN runtime for musl ([`docker-cross-platform-builds.md`](docker-cross-platform-builds.md)).
- **Non-ASAN binary + ASAN `libafw`** fails at start (`ASan runtime does not come first`). Probes get matching flags from `run_c_probe`; anything else must be built with the same flags or `LD_PRELOAD` the runtime.
- A Python test whose `run()` raises hangs `afwdev test` (pre-existing; backlog *afwdev test bugs*). Use `./afwdev` in the repo, not a stale PATH copy.
