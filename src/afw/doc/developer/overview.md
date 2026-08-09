Developer overview {#afw_dev_overview}
==================

@brief Who the C API docs are for and how AFW is structured for builders.

## Audience

This material is for **people who build on Adaptive Framework in C**:

- Core `libafw` developers  
- **Extension** authors (adapter types, content types, functions, …)  
- **Command** and host authors (`afw`, `afwfcgi`, custom commands)  
- Anyone implementing or calling **core interfaces**

It is **not** the primary guide for Adaptive Script application authors or
REST-only users. For product overview and language docs, see the published
[handbook](https://afw-org.github.io/afw).

## Big pieces

| Piece | Role |
|-------|------|
| **Interfaces (XML → C)** | Contracts for adapters, streams, logs, extensions, … |
| **Call macros** | Day-to-day C API: `afw_<iface>_<method>(…)` |
| **Environment** | Process-wide registries; extensions register as peers of core |
| **Values / pools / xctx** | Runtime data model and memory/lifetime |
| **afwdev** | Generate, build, test, and **scaffold** new packages/impls |

## Public vs internal

- **Public (extensions/commands):** `afw.h`, generated interface macros,
  environment register APIs, documented groups under the public C API.
- **Internal (libafw only):** `afw_internal.h`, `*_internal` groups — may change
  without notice.

## Rebuilding this documentation

```bash
# Preferred: afwdev docs pipeline (handbook + Doxygen under build/docs/)
./afwdev build --docs -j

# After changing Doxygen comments / groups / these pages, force Doxygen again
./afwdev build --docs --clean -j
```

afwdev invokes `doxygen Doxyfile` (settings in the package-root `Doxyfile`).
It skips Doxygen when `build/docs/doxygen` already exists; `--clean` with
`--docs` removes only the docs build tree (intentional scoped clean).

HTML look (light/dark slate skin): `src/afw/doc/doxygen-extra.css` — see
@ref afw_dev_doxygen_skin before editing.

## Where to go next

- @ref afw_dev_interfaces — macros, XML, generation  
- @ref afw_dev_implementing — afwdev make/add, skeletons, impl_declares  
- @ref afw_dev_extending — end-to-end extension sketch  
- @ref afw_dev_writing_tests — adding regressions for `afwdev test`  
- @ref afw_dev_runtime — pool, value, xctx, environment pointers  
- @ref afw_dev_types_opaques — `afw_*_t` vs multi-layout values  
- @ref afw_dev_compiler_ebnf — grammar comments / EBNF harvest (compiler only)  
- @ref afw_dev_doxygen_skin — Doxygen HTML light/dark stylesheet (maintainers)  
- Doxygen **Modules** list — group essays in `afw_doxygen.h`  
- Doxygen **Related Pages** — this set of developer pages
