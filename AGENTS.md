# AGENTS.md — Adaptive Framework (AFW)

Long-form context for humans and AI assistants. **Always-on** rules live in [`.cursor/rules/afw-project.mdc`](.cursor/rules/afw-project.mdc). Mention **`@AGENTS.md`** in chat when you need this map.

Primary development focus for Cursor agents: **C runtime** and **Python afwdev**. JS/admin work is secondary unless explicitly requested.

## Mission

AFW is **metadata-driven**: define object types, functions, data types, and C **interfaces** once, then generate headers, bindings, registration, and docs. The **runtime is C**; **afwdev** is Python.

## Main components

| Area | Role |
|------|------|
| `src/afw` | `libafw` — pools, `afw_value_t`, scopes/`xctx`, compiler, environment |
| `src/afw_dev` | `afwdev` (generate, build, test, docs, validate, …) |
| `src/afw_command` | `afw` CLI |
| `src/afw_*` | Extensions (loadable), same `generate/` / `generated/` pattern |
| `src/afw_server_fcgi` | FCGI server |
| `src/afw_app` | Admin React app (defer unless asked) |

Package manifest: [`afw-package.json`](afw-package.json) (`srcdirs`, `srcdirManifest`, `prefix`, `buildType`).

## Values and memory (short)

- **`afw_value_t`**: interface pointer (`inf`) + payload. Kinds include `compiled_value`, `block`, `call`, `symbol_reference`, `closure_binding`, etc.
- **Pools**: hierarchical allocation (including per-scope subpools). Reference counting for escaping values (e.g. closures); bulk free when pools/scopes release.
- **`compiled_value`**: owns a pool; evaluation uses scope-stack discipline.

Authoritative coding conventions: [`src/afw/doc/guide/developer/contributing.xml`](src/afw/doc/guide/developer/contributing.xml). Packages: [`packages.xml`](src/afw/doc/guide/developer/packages.xml).

## Metadata → generate → implement → test

```text
edit generate/ or hand C/Python  →  ./afwdev build --cdev -j  →  afwdev test -j
```

1. **Edit** `src/<srcdir>/generate/` — e.g. `objects/_AdaptiveFunctionGenerate_/*.json`, `interfaces/*.xml` — and/or hand C under `src/afw/…`.
2. **Build (C-dev)** — from package root: `./afwdev build --cdev -j`. This is the usual C/Python loop: generate, cmake-build, and install core, extensions, and the `afwdev` Python command. It does **not** build the JS app or docs. `-j` enables parallel make.
3. **Implement** — e.g. `src/afw/function/afw_function_<category>.c` (not `generated/function_closet/`).
4. **Test** — `afwdev test -j` runs the Adaptive Script tests (judge success from command output). Narrow with `--srcdir-pattern` / `--pattern` when useful.

Use **`./afwdev`** for builds that refresh/install `afwdev` itself; use **`afwdev`** (PATH) afterward for `test`, `validate`, etc.

Optional: `generate/additional_generate/additional_generate.py` for custom codegen.

Core interface XML: `src/afw/generate/interfaces/afw_interface.xml`.

## Hand-edit vs generated

| Hand-edit | Do not hand-edit |
|-----------|------------------|
| `generate/objects/`, `generate/interfaces/`, `generate/strings/`, `generate/ebnf/` | `src/*/generated/**`, package `generated/` |
| `function/afw_function_*.c`, runtime `*/afw_*.c` | `generated/function_closet/`, binding `*.c`/`*.h` |
| `tests/**/*.as`, `tests/**/config.py` | Generated test trees if present |
| `src/afw_dev/_afwdev/**/*.py` | Output those generators write under `generated/` |

Committed `generated/` is for review/grep; **authoritative** output is the last generate.

## afwdev cheat sheet

```bash
# From package root — C/Python day-to-day (generate + cmake + install; not JS/docs)
./afwdev build --cdev -j

# After --cdev, installed afwdev is current
afwdev test -j
afwdev test --srcdir-pattern afw --pattern 'rql/.*'
afwdev validate --pattern 'src/afw/generate/objects/...'

# Full repository: cmake + docs + JS (+ docker context), with install
./afwdev build --all --install -j

# Narrow generate only (usually unnecessary if using --cdev)
afwdev generate --srcdir-pattern '*'
```

`--cdev` is a convenience shortcut (enables generate/install and related C-dev switches; cmake is the build context; no `--js` / `--docs`). CMake output lives under `build/cmake/`.

## Documentation

- Author: `src/afw/doc/` (XML, markdown, images).
- Publish: included in `./afwdev build --all --install -j`, or `afwdev build --docs` → `build/docs/` (served as `/docs/...`).
- `--cdev` installs libs/headers/`afwdev` via cmake; it does not build the handbook or admin app.

## Cursor layout

| Path | Role |
|------|------|
| `.cursor/rules/afw-project.mdc` | Always-on |
| `.cursor/rules/afw-c-runtime.mdc` | C when editing `.c`/`.h` |
| `.cursor/rules/afw-generate-metadata.mdc` | When editing `generate/` |
| `.cursor/rules/afw-afwdev-python.mdc` | When editing `src/afw_dev` |
| `.cursor/rules/afw-tests.mdc` | When editing `.as` / test `config.py` |
| `.cursor/skills/add-adaptive-function/` | Add/change Adaptive functions or data types |
| `.cursor/skills/afw-generate-build-test/` | Regenerate, build, validate, test |
| `.cursorignore` | Skips `node_modules/`, `build/`, `generated/`, binaries |

## Other repositories

External AFW packages can implement the same interfaces and add interface XML; `additional_generate/` allows bespoke codegen without forking core afwdev generators.
