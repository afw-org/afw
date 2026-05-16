# AGENTS.md — Adaptive Framework (AFW)

This file is **long-form context** for humans and AI assistants. **Short, always-on rules** live in **`.cursor/rules/afw-project.mdc`**. Package structure is also in **`afw-package.json`**. In Cursor chat, you can mention this file with **`@AGENTS.md`** when you need the full picture.

## Mission

AFW is a **metadata-driven** framework: define object types, functions, data types, and C **interfaces** once, then generate headers, bindings, registration, and docs. The **runtime is C**; **afwdev** is Python.

## Main components

| Area | Role |
|------|------|
| `src/afw` | `libafw` — core runtime (pools, `afw_value_t`, scopes/`xctx`, compiler, environment). |
| `src/afw_dev` | `afwdev` command (generate, build, test, docs, validate, …). |
| `src/afw_command` | `afw` CLI executable. |
| `src/afw_app` | Admin web app (React); docs UX + links to static handbook under `/docs/...`. |
| `src/afw_server_fcgi` | FCGI server deployment. |
| `src/afw_*` (e.g. curl, ldap, …) | Extensions (loadable), same `generate/` / `generated/` pattern. |

## Values and memory (core mental model)

- **Value graph:** `afw_value_t` = interface pointer (`inf`) + implementation payload. Many **inf** kinds: `compiled_value`, `block`, `call`, `symbol_reference`, `closure_binding`, etc.
- **Allocation:** **Hierarchical memory pools** (including per-scope subpools). **Reference counting** is used for **escaping** values (e.g. closures holding lexical scopes); bulk free happens when pools/scopes are released.
- **`compiled_value`:** Owns a pool; evaluation uses scope-stack discipline; long-running / escape semantics are still being refined (`optional_release`, `clone_or_reference` on some value kinds).

## Metadata and code generation

1. **Per srcdir:** `src/<name>/generate/` — `objects/` (JSON under `_Adaptive*_/`), optional `interfaces/*.xml`, `manifest/`, `strings/`, `ebnf/`, optional `external/`, optional **`additional_generate/additional_generate.py`**, etc.
2. **`afwdev generate`** (or **`afwdev build --generate`**) **wipes** `src/<name>/generated/` and regenerates C, CMake fragments, bindings, copied `generated/objects/`, etc.
3. **Package root `generated/`** — from **`generate.root_generate()`**: shared schemas, root CMake includes (e.g. `afw_package_basic_build.cmake`), `generated/schemas/afw/`, etc.
4. **`afw-package.json`** — `srcdirs`, `srcdirManifest`, `prefix`, `buildType`; **`buildType: afwmake`** srcdirs that contain `generate/` are processed by the generate subcommand (subject to `--srcdir-pattern`).

**Committed `generated/`:** Often kept in Git for **diffs and grep**; **authoritative** output is still from the **last generate** — regenerate before relying on a clean build.

**Core interface XML:** `src/afw/generate/interfaces/afw_interface.xml`.

## Documentation

- **Authoring:** `src/afw/doc/` — XML guides/references, markdown, images. This is **not** the final HTML site by itself.
- **Published handbook:** `afwdev build --docs` → **`build/docs/`**, typically served as **`/docs/...`** (e.g. nginx in Docker). Placeholders in `doc/index.xml` expand data type / function / object type reference navigation from the **same JSON metadata** as the rest of the stack.
- **Admin app:** “Living” reference views use **runtime objects** (`retrieve_objects`, etc.); handbook links point at **built** HTML. Some requests use paths like `/doc/...` (served or proxied next to the app).

## Build, install, CI

- **`afwdev build`** — typically CMake under `build/cmake/`; **`--docs`** runs the Python doc builder; **`--js`** builds JS apps under `build/js/`.
- **`afwdev build --install`** runs **`cmake --install`** (libs, headers, `afwdev`, commands, per CMake). **HTML docs are not assumed** to be installed unless CMake explicitly adds them.
- **Multi-distro CI:** Builder images (Alpine, Ubuntu, Rocky, openSUSE, …) validate full builds. Example JS image path: `afwdev build --js --docs`, then **`tar`** `build/js/apps` → `afw-apps-*.tar` and `build/docs` → `afw-docs-*.tar` for nginx-backed images.

## Useful commands (from package root)

```bash
afwdev generate --srcdir-pattern '*'
afwdev build --generate          # generate then cmake
afwdev build --docs              # handbook → build/docs
afwdev build --js --docs         # apps + handbook (e.g. Docker builder)
```

## Using this repo with Cursor

- **Short rules:** `.cursor/rules/afw-project.mdc` (`alwaysApply: true`).
- **Long context:** `@AGENTS.md` in chat when you want this narrative loaded.
- **Do not hand-edit** `generated/`; after changing **`generate/`**, run **`afwdev generate`** or **`afwdev build --generate`** before trusting build/test results.

## Other repositories

External packages can implement the same interfaces and add more interface XML; **`additional_generate/`** allows Python hooks for bespoke codegen without forking core afwdev generators.