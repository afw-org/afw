# libafw headers and C API surface

**Audience:** maintainers and assistants.  
**Not user docs.** Related: [`.cursor/rules/afw-headers.mdc`](../.cursor/rules/afw-headers.mdc), `src/afw/include/afw_common.h` (major header table), `src/afw/doc/developer/overview.md`.

## Settled model (2026-08 header polish)

### Umbrellas

| Include | Who | Role |
|---------|-----|------|
| **`afw.h`** | Extensions, commands, apps (and anything using libafw from C) | **One convenient public umbrella**: call API **and** interface **impl helpers**. Fat on purpose — AFW is a framework, not a micro-library. Avoid header hell for implementers. |
| **`afw_internal.h`** | **`src/afw/**/*.c` only** | libafw implementation: `afw.h` + core `*_internal.h` + register/generated glue. May change any build. |
| **`<pkg>_internal.h`** | That extension/command package only | Same idea inside a package (`afw_lmdb_internal.h`, `afw_command_internal.h`, …). |

### Header graph (not a second public API)

| Header | Role |
|--------|------|
| **`afw_common.h`** | Early shared substrate (APR, opaques, typedefs/`#define`s needed by interfaces + peers). **Frozen placement** — some things “wanted” module headers but live here to avoid cycles. Do not reshuffle casually. |
| **`afw_interface.h`** (generated) | Contracts + call macros. Module headers typically `#include "afw_interface.h"` only. |
| **`afw_minimal.h`** | **Header bootstrap**, not a thinner app API. Included by **other headers** (and first from `afw.h`); **not** by `.c` files. Curated closed set after interfaces; membership left alone unless forced. |
| **`*_impl.h` / `*_impl_declares.h`** | Implementers of interfaces. On `afw.h` for convenience. Generated `*_impl_declares.h` from implementer `.c` only. |

```text
afw_common.h (+ opaques, early typedefs)
        ↑
afw_interface.h   ← most module .h
        ↑
afw_minimal.h     ← headers needing bootstrap set
        ↑
afw.h             ← .c convenience (call + impl)
        ↑
afw_internal.h    ← libafw .c only
```

### Version / ABI

- Host: `AFW_ENVIRONMENT_CREATE` / `afw_version_check(AFW_VERSION_HEX)`.
- Extension load: `afw_compiled_version_hex` on `afw_environment_extension_instance`.
- Pre-1.0: major+minor must match; patch free. ≥1.0: same major, runtime minor ≥ compiled minor.

### Install policy (goal)

- **Default install = supported public API** (what `afw.h` and intentional public/`*_impl` headers need).
- **Do not install** core `*_internal.h` / `afw_internal.h` by default (repo remains the place to explore internals).
- Optional later: “dev headers” install if someone asks.
- Monorepo builds still see full source includes at **build** time; filter applies to **install**.

### Doxygen

- **Public:** `afw_c_api_public` — call API for extensions/commands.
- **Impl support:** `afw_c_api_impl` — implementers (still public/supported, not “call every day”).
- **Internal:** `afw_c_api_internal` — libafw only; may change anytime.
- Default `INTERNAL_DOCS = NO` hides members marked `@internal`. Group titles must still say **libafw only** clearly for dual audience (public readers + core maintainers). Full internal member docs may need `INTERNAL_DOCS=YES` / a future docs-dev profile.

### Non-goals (this campaign)

- Split `afw.h` into call-only vs impl-only umbrellas.
- Rewrite `afw_common.h` / thin `afw_minimal.h` for purity.
- Small-PR discipline while on `mgg-develop` (large coherent branch OK; **PR only when maintainer asks**).

## Work notes

- No known current out-of-tree `#include` of core `*_internal.h`; residual risk is **symbols** that should be internal still declared on public headers, or **public-looking** APIs that are core-only.
- `AFW_DECLARE_INTERNAL` / `AFW_DEFINE_INTERNAL` = not external API (no export declspec). Prefer those symbols in `*_internal.h`, not on the `afw.h` surface. If an extension/command legitimately needs a helper, promote to `AFW_DECLARE` and document under impl/public.
- `afw_runtime_object_maps.h` stays public: extensions reference exported core `afw_runtime_inf_*` symbols.

## Naming / placement convention (internal)

- Prefer **`<module>_internal.h` next to the implementing `.c`** under the same subdirectory (`flag/afw_flag_internal.h` ↔ `flag/afw_flag.c`).
- Symbol names should keep the **module prefix** (`afw_flag_*`, `afw_stack_*`, …). Avoid parking `afw_flag_*` decls only on `afw_environment_internal.h` unless the body lives in `environment/`.
- Renaming every historical `*_internal_*` for purity is optional; **header home matching the .c** matters more than perfect names.

## Re-homes done (content cleanup)

| Symbol(s) | Public before | Internal home (with .c) |
|-----------|---------------|-------------------------|
| `afw_stack_internal_set_*` | `afw_stack.h` | `stack/afw_stack_internal.h` (`afw_stack.c`) |
| `afw_stream_internal_*` | `afw_stream.h` | `stream/afw_stream_internal.h` (`afw_stream.c`) |
| `afw_value_register_core_value_infs` | `afw_value.h` | `value/afw_value_internal.h` (`afw_value.c`) |
| `afw_flag_internal_early_register_core` | `afw_flag.h` | `flag/afw_flag_internal.h` (`afw_flag.c`) |
| `afw_lock_create_environment_nested_lock` | `afw_lock.h` | `lock/afw_lock_internal.h` (`afw_lock.c`) |

Still public with `@internal` layouts/comments (later passes): lock struct bodies, adapter id anchor, pool thread create comment, etc.

## Changelog

| Date | Note |
|------|------|
| 2026-08-11 | Initial pad from `feature-afw-polish` brainstorm (layers, install goal, frozen common/minimal). |
| 2026-08-11 | Install filter; Doxygen/overview/`afw.h` docs; first re-homes (stack/stream/value/flag/lock bootstrap). |
| 2026-08-11 | Corrected: internal decls live in module `*_internal.h` next to implementing `.c` (not env/xctx umbrellas). |
