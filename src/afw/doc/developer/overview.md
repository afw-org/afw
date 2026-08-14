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

| Layer | Include | Audience |
|-------|---------|----------|
| **Public call + impl** | `afw.h` (`.c`); `afw_interface.h` / `afw_minimal.h` (headers) | Extensions, commands, apps. Impl helpers (`*_impl.h`) are intentional — most C users implement interfaces. |
| **Internal** | `afw_internal.h`, `*_internal.h` | **`src/afw/` only.** Not part of the supported install set; explore via git. May change any time. |
| **Package private** | e.g. `afw_lmdb_internal.h` | That package’s sources only. |

Doxygen: Modules → **C API (public)** vs **C Internal (libafw only)** vs
**Interface implementation support**. Default build hides members tagged
`@internal` (`INTERNAL_DOCS = NO`). Details:
`designs/libafw-headers-and-api-surface.md`.

## Maintainer maps in the source tree

This handbook page and the Doxygen groups explain the **C builder surface**.
The Git repository also carries a lot of **development and support knowledge**
for people who work in the tree day to day (humans, and tools that read the
repo). None of that replaces this documentation or the tests; it is a map into
deeper notes.

| Start here | Role |
|------------|------|
| **`AGENTS.md`** (package root) | System map: mission, layout, build/test loop, where other notes live. Includes an optional short pattern for working an issue with an AI partner (“what do you think about issue #N?”). |
| **`CLAUDE.md`** (package root) | Thin stub for Claude Code — points at `AGENTS.md` and key `designs/` paths; not a second full guide. |
| **`designs/`** | Theme pads, topic atlas, support playbooks, optional lessons — *why*, open questions, and “where is X?” without rewriting the handbook. |
| **`.cursor/rules/`** | Optional always-on / area habits for some editor agents (how to work in a given tree). Not required to build AFW by hand. |

Useful entry points under `designs/` when you want to dive deeper:

- `designs/knowledge-atlas.md` — topic → rules, pads, and live probes  
- `designs/agent-support.md` — symptom-oriented playbooks  
- `designs/ai-partner-lessons.md` — **optional** lessons on pairing with an AI partner (follow or not)  
- `designs/afw-philosophy-and-core-model.md` / `designs/mantras-and-working-style.md` — design framing (reference, not always-on law)

**Ground truth** remains code, tests, this developer set, and the published handbook. When a pad and the tree disagree, trust the tree and fix the pad.

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
