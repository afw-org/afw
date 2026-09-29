# Lineage, base vs private packages, library floor

**Audience:** maintainers and AI assistants. **Not** handbook or a product promise.

Facts that are easy to lose and that change how you write C, pick APIs, and where unfinished work lives. When this pad and the tree disagree, the tree wins.

## Layers

Three different things, easy to flatten:

| Layer | What it is |
|-------|------------|
| **Adaptive concepts** | The high-level Adaptive things inside Adaptive Framework (object, value, adapter, mapping, layout, interface, environment, …). Canonical list: [`afw-philosophy-and-core-model.md`](afw-philosophy-and-core-model.md) (*Adaptive concepts*). Handbook architecture / glossary use the same names. |
| **Core (libafw)** | The C implementation of most of that model, in **`src/afw`**. |
| **Base (this repository)** | Public **`afw`**: libafw **plus** `afwdev`, shipped commands (`afw`, `afwfcgi`), shipped extensions (`src/afw_*`), and the admin app. Srcdir map: [`AGENTS.md`](../AGENTS.md) *Main components*. |

Not-yet-public extensions live in **`inter-afw-private`**. If one becomes part of the base, it is **promoted into this repository**. That is not a schedule.

A predecessor system was an **XACML** implementation in C. AFW is not that engine renamed. Some trees in `inter-afw-private` still reflect a possible later mapping (XACML function names onto AFW built-ins, XACML policy onto an AFW authorization-policy shape). **That is context only** — not a commitment to finish, ship, or schedule that work.

## Other repos (work placement)

| Repo | Role |
|------|------|
| Public **`afw`** | Base. Issues/PRs that belong in the open. |
| **`inter-afw`** | Whole-project private AFW (pre-public history; private boards and similar). Not where new extension code is added. |
| **`inter-afw-private`** | Not-yet-public **extensions** and related trees (examples already cited elsewhere: Oracle, Berkeley DB). Includes `src/afw_xacml`, `src/afw_xacml_pdp`, and `src/afw_authorization_policy`. New work of that kind goes here, not into a separate XACML repo. |

If something in `inter-afw-private` is ready to be part of the base, it is **promoted into public `afw`**. Do not assume that will happen, and do not start private-repo work unless asked.

When working in `inter-afw-private`, check out **`afw` as a sibling directory** (same parent). How the private tree should include or reference the base can wait until that work starts.

Any other AFW package next to this repo uses the same sibling layout. To drop the Grok/Cursor write wall and thin `AGENTS.md` into that package, see [`sibling-afw-package.md`](sibling-afw-package.md) (*When asked to prime*).

## Library floor (Docker)

Develop against the **published image bases**, not against “whatever this workstation or this one container happens to have.” The in-tree `docker/images/afw-dev-base/` files are the matrix:

| Dockerfile | Base | ICU | APR | GCC | libcurl | libxml2 | Python |
|------------|------|-----|-----|-----|---------|---------|--------|
| `Dockerfile.almalinux` | AlmaLinux 9 | 67.1 | 1.7.0 | 11.5 | 7.76.1 | 2.9.13 | 3.9 |
| `Dockerfile.ubuntu` | Ubuntu 24.04 | 74.2 | 1.7.2 | 13.3 | 8.5.0 | 2.9.14 | 3.12 |
| `Dockerfile.rockylinux` | Rocky Linux 10 | 74.2 | 1.7.5 | 14.3 | 8.12.1 | 2.12.5 | 3.12 |
| `Dockerfile.opensuse` | openSUSE Leap 16.0 | 77.1 | 1.7.5 | 15.3 | 8.14.1 | 2.13.8 | 3.13 |
| `Dockerfile.alpine` | Alpine 3.24 | 78.1 | 1.7.6 | 15.2 | 8.22.0 | 2.13.9 | 3.14 |

Versions verified live (`pkg-config --modversion`, `apr-1-config --version`, `gcc -dumpfullversion` inside each `afw-dev-base` image) as of 2026-09-28, after the distro bumps in [`docker-cross-platform-builds.md`](docker-cross-platform-builds.md) (*2026-09-28 base bumps*).

**AlmaLinux 9 is now the conservative end on every axis** (ICU 67.1, APR 1.7.0, GCC 11, libcurl 7.76, libxml2 2.9, Python 3.9) — Leap 15.5 and Alpine 3.16 held that spot until they were bumped. The development container is **Ubuntu 24.04**, in the middle of the matrix; Alpine and Leap carry the newest toolchains (GCC 15, libxml2 2.13) and catch new warnings and API removals first. An API that exists on Ubuntu 24.04 can still fail the AlmaLinux image; one that works there can still trip a new-compiler warning on Alpine or Leap.

Ubuntu stays on an LTS that is one release back (24.04, not 26.04) on purpose: the builder's `.deb` is built on this base, and a 26.04 build needs glibc 2.43, so it would not install on 24.04.

`U8_NEXT` / `U8_APPEND` (the bounded ICU macros used in `afw_utf8`) are old enough for this matrix. “A newer ICU call” means **present on the oldest base**, not present on this container.

## Where ICU belongs

Keep ICU (`unicode/*.h`, `U8_*`, `u_*`, `unorm2_*`) in **`src/afw/utf8/`** (NFC, to_lower, UTF-8 walk) and **`src/afw/code_point/`** (identifier / whitespace / Cc — encoding-neutral). Do not pull `unicode/*.h` into every TU.

| Place | Use |
|-------|-----|
| `src/afw/utf8/afw_utf8.c` | `U8_*`, `unorm2_*`, `u_str*`, `u_tolower`, NFC, `u_errorName` wrap |
| `src/afw/code_point/afw_code_point.c` | `u_hasBinaryProperty` / `u_charType` |

Env ICU decoder calls `afw_utf8_icu_error_name_z()`. [#206](https://github.com/afw-org/afw/issues/206).
