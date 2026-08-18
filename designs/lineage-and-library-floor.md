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

## Library floor (Docker)

Develop against the **published image bases**, not against “whatever this workstation or this one container happens to have.” The in-tree `docker/images/afw-dev-base/` files are the matrix:

| Dockerfile | Base |
|------------|------|
| `Dockerfile.alpine` | Alpine 3.16 |
| `Dockerfile.ubuntu` | Ubuntu 22.04 |
| `Dockerfile.rockylinux` | Rocky Linux 8.9 |
| `Dockerfile.opensuse` | openSUSE Leap 15.5 |

This development container is **Ubuntu 22.04** (ICU 70.1, APR 1.7 as of 2026-08). That is **not** the oldest base. Rocky 8 is the conservative end (RHEL 8-era ICU is about 60). An ICU or APR API that exists only on Ubuntu 22.04 can still fail the Rocky or Alpine image.

`U8_NEXT` / `U8_APPEND` (the bounded ICU macros used in `afw_utf8`) are old enough for this matrix. “A newer ICU call” means **present on the oldest base**, not present on this container.

## Where ICU belongs

Keep ICU (`unicode/*.h`, `U8_*`, `u_*`, `unorm2_*`) in **`src/afw/utf8/`** (`afw_utf8.c` / `afw_utf8.h`) except rare overrides.

On current `develop`, the only other core includes are:

| Place | Use |
|-------|-----|
| `src/afw/compile/afw_compile_code_point.c` | `u_hasBinaryProperty` / `u_charType` for identifier start/continue and whitespace |
| `src/afw/environment/afw_environment_register_core.c` | `u_errorName` for the ICU error-code decoder |

A later pass can wrap those through `afw_utf8.h` so the rest of core does not include ICU. Prefer **functions** in that header over macros that pull `unicode/*.h` into every translation unit. Not a current sitting unless asked.
