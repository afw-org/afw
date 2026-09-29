# Docker cross-platform builds

**Audience:** maintainers and AI assistants. **Not** handbook or a product promise.

Live-session notes from actually running `docker buildx` multi-platform (`linux/amd64` + `linux/arm64`) builds for all 7 images `docker.py` knows about, plus a RockyLinux base bump. Things that bit us and are not obvious from the code. When this pad and the tree disagree, the tree wins.

## `afwdev build --docker` is a no-op **by design** — not a bug to fix

`src/afw_dev/_afwdev/build/docker.py`'s `build_image()` constructs the `docker buildx build …` command per image and logs it, but the execution line is commented out:

```python
msg.highlighted_info("    Running: " + cmd)
#os.system(cmd)
```

`--push` is stubbed the same way — never appended even if wired up. So `./afwdev build --docker` only **prints** commands; it builds nothing yet. **Do not wire `os.system(cmd)` back in without being asked explicitly** — when that changes, these become real, slow, multi-platform C compiles, and turning them on silently would surprise anyone who already has `--docker` in a script or habit.

**Confirmed with Jeremy (2026-09-10) and now enforced in code, not just intent:** cross-platform docker builds are slow (each image does a full native C compile per target platform — tens of minutes, more under emulation) and `--fulldev` is something many maintainers run often. So `docker` was pulled out of `--all`/`--fulldev`'s context sweep entirely — see `_BUILD_TYPE_CONTEXTS_ALL` in `build.py` (`_BUILD_TYPE_CONTEXTS` still lists `docker` for directory bookkeeping and the "was any context requested" check; the `--all`/`--fulldev` sweep uses the smaller `_BUILD_TYPE_CONTEXTS_ALL`, which excludes it). `--docker` remains a fully independent flag (`_info_build_docker` in `cli/info.py`) and still works standalone or combined explicitly with `--fulldev --docker`. **This means even if `os.system(cmd)` is un-stubbed later, `--fulldev`/`--all` alone still won't trigger a docker build** — `--docker` has to be passed.

To actually build (until `os.system(cmd)` is wired back in), run `docker buildx build` directly with the file/tag/platform values `docker.py` would compute — see `get_tags()` for the tag scheme (OS tag + `latest` on the primary variant + `<os>-<version>`).

`docker.py`'s `build()` only loops over `_docker_images` (afw-dev × {ubuntu, alpine, rockylinux, opensuse}, afw, afwfcgi, afw-admin — 7 builds). `_docker_base_images` (afw-base, afw-dev-base) is defined in the same file but **never looped over** — nothing in this repo builds those base images either (see next section).

## No base-image publish workflow in this repo

`ghcr.io/afw-org/afw-dev-base:*` and `ghcr.io/afw-org/afw-base:*` are consumed as pre-built, pre-published images in two places:

- `builds.yml`'s `build_c_*` jobs — `docker/images/builder/Dockerfile.*`'s first stage is `FROM ghcr.io/afw-org/afw-dev-base:<os>`
- `integration.yml`'s `build_test_c_*` jobs — used directly as the job's `container:` image

But `.github/workflows/` only has `builds.yml`, `docs.yml`, `integration.yml` — **none of them build or push `afw-dev-base`/`afw-base`**. That publish step happens somewhere outside this repo's tracked workflows. Practical implication: editing an `afw-dev-base/Dockerfile.*` (as with the RockyLinux bump below) does not take effect for CI, or for anyone pulling the tag, until someone rebuilds and pushes it through whatever that external process is.

**How it was actually done (2026-09-27/28):** by hand from the devcontainer, which talks to Docker Desktop's daemon (containerd image store, QEMU built in). The container needs the buildx CLI plugin (`~/.docker/cli-plugins/docker-buildx`); the default `docker` driver is fine here because the base Dockerfiles have no shared `$BUILDPLATFORM` stage. Per flavor: `docker buildx build --platform linux/amd64,linux/arm64 -f docker/images/afw-dev-base/Dockerfile.<os> -t …:<os> -t …:<os><version> --load .` (ubuntu also gets `:latest`), then `docker push` each tag after `docker login ghcr.io` with a **classic** PAT (`write:packages`). Tag pattern is `<os><base version>`: `ubuntu24.04`, `alpine3.24`, `rockylinux10`, `almalinux9`, `opensuse16.0` — not `docker.py`'s `get_tags()` (`<os>-<afw version>`), which is only right for `afw-dev`.

**Verify a base before pushing — building the image is not enough.** Stream a clean source tar into the image and run the real loop (`./afwdev build --cdev && afwdev test -j`). The 2026-09-28 round found a dozen real failures (next section) in images that built cleanly. The daemon is on the host, so bind-mounting a devcontainer path does not work — pipe the tar over stdin (`docker run -i … bash -c 'mkdir /src && tar -x -C /src && …' < src.tar`).

Also, as of 2026-09-10: both `builds.yml` and `integration.yml` show **zero recorded runs ever** (`gh api repos/afw-org/afw/actions/workflows/<id>/runs` → `"total_count":0` for both). `integration.yml` only triggers on PRs targeting `main` + manual dispatch; `builds.yml` is manual-dispatch-only. Neither is actively exercised today — don't assume CI will catch a Dockerfile regression.

## Fetch clean source before building — the working tree is not a safe build context

`COPY ./ /src` in these Dockerfiles naively includes whatever is sitting in the working tree, including:

- **`build/`** — local CMake output (can be ~1GB). Worse than bloat: if `build/cmake/CMakeCache.txt` already exists (from an ordinary local `afwdev build`), CMake **refuses to reconfigure** inside the container, because the cache was generated for a different absolute source path (`/workspaces/afw` vs `/src`). Hard error, not just slower.
- **`node_modules/`** (~900MB) — not needed for the C build stage at all.

The repo-root `.dockerignore` now excludes `build/` and `node_modules` (plus `linux_amd64`/`linux_arm64`), which covers the context for `-f … .` builds; the notes below still apply to any other context directory.

The tempting fix — `git archive HEAD` into a scratch directory as the build context — backfires: it strips `.git`, and the builder scripts (`docker/images/builder/builder-*.sh`) call `./afwdev --version-string`, which shells out to `git rev-parse`. Outside a git repo that prints `fatal: not a git repository` to **stdout**, and the script captures it via *unquoted* command substitution (`DEB_VERSION=${DEB_VERSION:=\`./afwdev --version-string\`}`). The diagnostic text ends up inside `$DEB_VERSION`, gets word-split on the later unquoted use, and corrupts the following command — this concretely surfaced as `cp: unrecognized option '--abbrev-ref'`, many build steps away from the real cause, which made it a confusing one to trace back.

**What actually worked:** build from a *clean, fetched* copy of the source — `tar --exclude=./build --exclude=./node_modules` of the working tree (or an equivalent fresh checkout), keeping `.git` intact. That cut the build context from ~1.9GB to ~370MB and avoided both failure modes at once. Don't just point `docker buildx build` at a live, possibly-locally-built working tree.

## buildx driver matters — a real, reproducible race with the default driver

The default `docker` buildx driver (with Docker Desktop's containerd-snapshotter integration) can do local multi-platform builds + `--load` with no extra setup — no manual QEMU/binfmt install needed on Docker Desktop; it's already registered.

But it has a genuine bug/race for how several of these Dockerfiles are shaped. `afw-dev/Dockerfile.{ubuntu,alpine,rockylinux,opensuse}`, `afw/Dockerfile.alpine`, and `afw-admin/Dockerfile` all declare a JS build stage pinned with `--platform=$BUILDPLATFORM` (built once, meant to be shared across every requested target platform), then a later stage does `COPY --from=<that shared stage> /*.tar /`. When building `--platform linux/amd64,linux/arm64` together with the default driver, this `COPY` can non-deterministically drop the shared-stage files for one of the two target platforms — **silently**, because BuildKit does not error on a zero-match wildcard `COPY`; it just copies nothing. The failure only surfaces later, at whatever `RUN tar xvf …` step tries to use the now-missing file — several steps removed from the actual defect, which makes it look like a Dockerfile authoring bug at first glance. It is not: retrying the *identical, unmodified* Dockerfile with a different driver fixed it every time (see next paragraph), and a minimal repro of the underlying `COPY`-into-existing-directory pattern in isolation (single platform) worked fine — the failure is specific to the multi-platform + shared-stage + default-driver combination.

**Fix/workaround:** switch to the `docker-container` driver:

```bash
docker buildx create --name mybuilder --driver docker-container --use
docker buildx inspect --bootstrap
```

Every retry succeeded with this driver, same Dockerfiles, same context, unmodified. Prefer `docker-container` for multi-platform builds of these Dockerfiles going forward; the default driver is fine for single-platform builds or images without a shared `$BUILDPLATFORM`-pinned stage (e.g. `afwfcgi`, which has no JS stage).

**Gotcha:** the `docker-container` driver has its **own image store**, separate from the local `docker images` daemon store. A locally built-and-`--load`ed image (via the default driver) is invisible to it unless pushed through a registry. If testing against a locally-built, not-yet-published base image, use the default driver instead (it shares the daemon's store).

## Real Dockerfile bug found and fixed: `afwfcgi/Dockerfile.alpine`

Globbed `/afw-*alpine*.tar` but `builder-alpine.sh` actually produces `afw-<ver>-alpine.<arch>.tar.gz` (note the `.gz`). Zero-match `COPY` silently no-ops (see the driver note above for why BuildKit doesn't error on this); the following `tar xvf` then fails with "no such file," unrelated-looking to the real cause. Fixed the glob and added the missing `--strip-components=1` (the tar has a top-level `afw-<ver>_<arch>/` directory that the sibling `.alpine` Dockerfiles already strip). Verified: image builds both platforms, `afw --version` runs inside it.

## RockyLinux base bumped 8.9 → 9 (curl too old for `afw_curl`)

`afw-dev-base/Dockerfile.rockylinux` was pinned to `rockylinux:8.9.20231119`, shipping libcurl **7.61.1**. `src/afw_curl/afw_curl_function_curl.c` calls `curl_easy_option_next()`, added in libcurl **7.73**. The compile failure was real, not environmental — see [`lineage-and-library-floor.md`](lineage-and-library-floor.md) for how this fits the wider ICU/APR floor picture.

AlmaLinux's Dockerfile (`afw-dev-base/Dockerfile.almalinux`, `FROM almalinux:9`) already had the fix for the RHEL9-family move. RockyLinux and AlmaLinux are both RHEL9 rebuilds, so the RockyLinux changes mirror AlmaLinux's exactly:

- `powertools` repo → `crb` (RHEL9 renamed PowerTools to CodeReady Builder)
- dropped `libdb-devel` — verified via a real build (`afwdev build --prefix /usr/local --package` run inside the base image) that AFW's C build has **zero actual dependency on Berkeley DB**; `libdb-devel` isn't available the same way in RHEL9 repos anyway. (`building_on_linux.md` still labels this package "# BerkeleyDB Adapter" — that's the *only* place the string "BerkeleyDB" appears anywhere in the tree; no `CMakeLists.txt` or source file references it. Stale doc comment, not a real adapter — don't be fooled into thinking this needs a source-level fix too.)
- `pip3.6` → `pip3` (RHEL8's default Python was 3.6; RHEL9-family ships newer)

Verified end-to-end, not just by inspection: rebuilt `afw-dev-base:rockylinux` (both platforms), confirmed curl 7.76.1 inside it, ran the full C build including `afw_curl` (`Build successful`), built `afw-dev:rockylinux` multi-platform, and ran `afw --version` inside the result.

Rocky 9 and AlmaLinux 9 bases were published 2026-09-28 (first AlmaLinux publish). `docker.py`'s `_docker_afw_dev_image_info` still has no AlmaLinux entry, so `afw-dev:almalinux` is built by nothing.

## 2026-09-28 base bumps: Ubuntu 24.04, Alpine 3.24, openSUSE Leap 16.0, Rocky 10

Why: Alpine 3.16 and Leap 15.5/15.6 are EOL and could no longer install `python-requirements.txt` (Sphinx/`build` added in #283). Leap 15.6's repos are frozen out of sync with its image (zypper solver conflict on `libxml2-devel`/`libncurses6`) — go to 16.0, don't patch 15.x. Alpine went to 3.24 rather than 3.21 because 3.23 is the first Alpine whose `nodejs` is 24 (3.21/3.22 ship 22); `n 24` cannot install on musl, so Alpine takes Node from apk, not `n`. Ubuntu went to **24.04, not 26.04**: 26.04 passed everything too, but the builder's `.deb` is built on this base and a 26.04 build needs glibc 2.43, so it would not install on 24.04 (the package has no `Depends`, so it fails at run time, not install time). Rocky moved 9 → 10 (`FROM rockylinux/rockylinux:10`; the Docker Hub `rockylinux` library image has no 10). RHEL 10 clones need **x86-64-v3** (AVX2) for amd64.

Found only by running the test suite inside each base (all are real, none environmental):

- **musl thread stacks** — musl defaults new threads to ~128KiB; the C stack headroom check tripped on every `afwfcgi` worker. `afw_os_thread_create` now sizes threads to `max(2MiB, 4 × limitCStackHeadroomBytes, RLIMIT_STACK)` — the floor only matters when `ulimit -s` is lower or unlimited. Application conf `threadStackBytes` (non-zero) replaces the 2MiB/`RLIMIT_STACK` default; 4 × headroom still applies. The base/main thread is `ulimit -s`. (A full default 500-slot eval stack measured ~500KiB of C stack unoptimized; main-thread crossover from eval-limit to headroom trip was ~700KiB with the 256KiB default headroom.) Separately, musl's `pthread_attr_getstack` under-reports the **main** thread; the `RLIMIT_STACK` rebase in `afw_os_c_stack_bounds` is main-thread only (`getpid() == gettid`) — applying it to workers would put their low bound below the real stack.
- **musl `r+` streams** — switching read→write on an update stream without `fseek`/`fflush` is UB; glibc tolerates it, musl writes at its buffered position (EOF). `afw_stream_fd` repositions on direction change.
- **musl `fopen(path, "")`** succeeds (opens write-only). `open_file` validates the mode first.
- **minimal libcurl** — Rocky/Alma (`libcurl-minimal`) and Leap 16 (`libcurl-mini4`) ship curl without SMTP; the curl SMTP upload tests fail with `Error in curl_easy_setopt()`. Dockerfiles install full `libcurl` / `libcurl4`.
- **`lib64`** — RHEL-family `GNUInstallDirs` installs to `/usr/local/lib64/afw`; `c_probe.py` hard-coded `lib`.
- **Leap 16 `fcgi.pc`** says `-I/usr/include` but headers are in `/usr/include/fastcgi`; `afw_fcgi-config.cmake` now `find_path`s `fcgiapp.h` even when pkg-config succeeds.
- **libcurl error text drifts** — 8.22 + c-ares says `Could not resolve host: xyz (Domain name not found)`; the `afw_curl` bad-URL tests matched the whole message (`expect: error:<msg>` is exact). They now `try`/`catch` and check `starts_with`.
- **GCC 15** `-Werror=unterminated-string-initialization` on a 16-char hex table in `afw_ldap`.
- **Hard-coded LLVM 14** — `afwdev build --scan` ran `analyze-build-14` and the printf scan looked for `libclang-14..18`. Ubuntu ships clang tools only as `<tool>-<llvm major>` (plain `analyze-build` is a broken symlink on 22.04, absent on 24.04). `cmake.py` now picks the highest `/usr/bin/analyze-build-N`; `printf_scan.py` falls back to any `libclang-*.so.1` on disk, newest first.

Also in this round, all five images:

- **Python in a venv** at `/opt/afw-venv`, first on `PATH` (`ENV VIRTUAL_ENV`/`PATH`), instead of `pip --break-system-packages`. PEP 668 exists because pip can clobber distro-managed packages (Alpine 3.16's `Cannot uninstall 'packaging'`); the venv avoids that rather than silencing it. `afwdev` and its test runner reach Python only through `PATH` (`#!/usr/bin/env python3`, `subprocess.run(['python3', …])`), so nothing else changed. Anything that hard-codes `/usr/bin/python3` would bypass it. `.venv/`/`venv/` are git- and docker-ignored for local venvs.
- **Sanitizer packages** in each main install: ASan/UBSan runtimes for gcc and clang plus `llvm-symbolizer` (Ubuntu adds `clang llvm libasan8 libubsan1`; RHEL-family `libasan libubsan compiler-rt llvm`; Leap `libasan8 libubsan1 llvm`). Verified with a deliberate heap overflow under `-fsanitize=address` for both compilers. **Alpine has none**: neither gcc nor clang ships an ASan runtime for musl. `afwdev` has no sanitizer build/test mode yet (would need `-fsanitize` on compile+link and its own env mode — ASan and valgrind don't mix, and ASan's stack use may interact with the C stack headroom check).

Result: 4548 passed / 0 failed on Ubuntu 24.04, Alpine 3.24, Rocky 10, AlmaLinux 9, openSUSE 16.0 (arm64 native; amd64 images built under QEMU but not test-run).
