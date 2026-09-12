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

Also, as of 2026-09-10: both `builds.yml` and `integration.yml` show **zero recorded runs ever** (`gh api repos/afw-org/afw/actions/workflows/<id>/runs` → `"total_count":0` for both). `integration.yml` only triggers on PRs targeting `main` + manual dispatch; `builds.yml` is manual-dispatch-only. Neither is actively exercised today — don't assume CI will catch a Dockerfile regression.

## Fetch clean source before building — the working tree is not a safe build context

`COPY ./ /src` in these Dockerfiles naively includes whatever is sitting in the working tree, including:

- **`build/`** — local CMake output (can be ~1GB). Worse than bloat: if `build/cmake/CMakeCache.txt` already exists (from an ordinary local `afwdev build`), CMake **refuses to reconfigure** inside the container, because the cache was generated for a different absolute source path (`/workspaces/afw` vs `/src`). Hard error, not just slower.
- **`node_modules/`** (~900MB) — not needed for the C build stage at all.

There's no `.dockerignore` in the repo, so nothing strips these automatically.

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

**Not yet done / known gaps surfaced along the way:**
- `docker.py`'s `_docker_afw_dev_image_info` list doesn't include AlmaLinux at all, even though `Dockerfile.almalinux` exists — so AlmaLinux is built by nothing today (and wouldn't be even if `afwdev build --docker` were un-stubbed).
- AlmaLinux isn't published to `ghcr.io` either — the local image used for verification here (`ghcr.io/afw-org/afw-dev-base:almalinux`) only exists because it was built locally at some point; `docker pull` for that tag returns "not found."
- The RockyLinux base-image fix itself isn't live anywhere until someone pushes a new `ghcr.io/afw-org/afw-dev-base:rockylinux` (see "No base-image publish workflow" above).
