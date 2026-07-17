# AGENTS.md — Adaptive Framework (AFW)

Long-form context for humans and AI assistants. **Always-on** rules live in [`.cursor/rules/afw-project.mdc`](.cursor/rules/afw-project.mdc). Mention **`@AGENTS.md`** in chat when you need this map.

Primary development focus for Cursor agents: **C runtime** and **Python afwdev**. JS/admin work is secondary unless explicitly requested.

## Mission

AFW is **metadata-driven**: define object types, functions, data types, and C **interfaces** once, then generate headers, bindings, registration, and docs. The **runtime is C**; **afwdev** is Python.

## Main components

| Area | Role |
|------|------|
| `src/afw` | `libafw` — pools, values, xctx, compiler, objects/adapters, environment (module map: [`.cursor/rules/afw-core-layout.mdc`](.cursor/rules/afw-core-layout.mdc)) |
| `src/afw_dev` | `afwdev` (generate, build, test, docs, validate, …) |
| `src/afw_command` | `afw` CLI — compile/eval Adaptive syntaxes, conf/extensions, `--local` ([`.cursor/rules/afw-command.mdc`](.cursor/rules/afw-command.mdc)) |
| `src/afw_*` | Loadable extension DSOs — same env registries as core ([`.cursor/rules/afw-extensions.mdc`](.cursor/rules/afw-extensions.mdc): curl, ldap, lmdb, ubjson, vfs, yaml) |
| `src/afw_server_fcgi` | `afwfcgi` FastCGI server — HTTP transport over libafw request handlers ([`.cursor/rules/afw-server-fcgi.mdc`](.cursor/rules/afw-server-fcgi.mdc)) |
| `src/afw_app` | Admin React app (defer unless asked) |

Package manifest: [`afw-package.json`](afw-package.json) (`srcdirs`, `srcdirManifest`, `prefix`, `buildType`).

## Values and memory (short)

- **`afw_value_t`**: interface pointer (`inf`) + payload. Kinds include `compiled_value`, `block`, `call`, `symbol_reference`, `closure_binding`, etc. Full mental model: [`.cursor/rules/afw-runtime-model.mdc`](.cursor/rules/afw-runtime-model.mdc); compile/eval details: [`.cursor/rules/afw-script-eval.mdc`](.cursor/rules/afw-script-eval.mdc).
- **Pools**: hierarchical allocation (including per-scope subpools). Reference counting for escaping values (e.g. closures); bulk free when pools/scopes release.
- **`compiled_value`**: owns a pool; evaluation uses scope-stack discipline and `statement_flow` for leave paths.
- **Data-type value lifetimes** (inf chooses policy): **permanent** (built-in / life of AFW environment; usually const in the `.so`); **managed** (refcount or clone); **managed_slice** (utf8/memory view into a containing managed value); **unmanaged** (programmer/pool). Create APIs come from `data_type_bindings.py`. A main focus for long-running scripts is getting managed release/clone paths right and evaluating into `scope->p` — see [`.cursor/rules/afw-value-memory.mdc`](.cursor/rules/afw-value-memory.mdc).

## Interfaces vs script compiler vs environment

- **Interfaces** — Core, extensions, and commands access C capabilities through contracts defined in `generate/interfaces/*.xml` (core: `afw_interface.xml`) and generated into headers/vtables/skeletons.
- **Environment** — Process-wide keyed registries (`afw_environment_t` / `xctx->env`). Core registers at create (`afw_environment_register_core.c`: `afw_generated_register` then hand wiring — **functions before `prepare_environment`**, then conf/adapters/content types). Extensions/commands use the same registries. Details: [`.cursor/rules/afw-environment.mdc`](.cursor/rules/afw-environment.mdc).
- **Script compiler** — `src/afw/compile/` (`afw_compile.h`) turns syntaxes into `afw_value` graphs; EBNF docs in `/*ebnf>>>` comments harvested via `generate/ebnf/`. Evaluate via `value/` + `function/`. Rules: [`.cursor/rules/afw-compile.mdc`](.cursor/rules/afw-compile.mdc), [`.cursor/rules/afw-script-eval.mdc`](.cursor/rules/afw-script-eval.mdc), [`.cursor/rules/afw-function.mdc`](.cursor/rules/afw-function.mdc), [`.cursor/rules/afw-compiler-ebnf.mdc`](.cursor/rules/afw-compiler-ebnf.mdc).

Authoritative coding conventions: [`src/afw/doc/guide/developer/contributing.xml`](src/afw/doc/guide/developer/contributing.xml). Packages: [`packages.xml`](src/afw/doc/guide/developer/packages.xml).

## Metadata → generate → implement → test

```text
edit generate/ or hand C/Python  →  ./afwdev build --cdev -j  →  afwdev test -j
```

1. **Edit** `src/<srcdir>/generate/` — e.g. `objects/_AdaptiveFunctionGenerate_/*.json`, `interfaces/*.xml` — and/or hand C under `src/afw/…`.
2. **Build (C-dev)** — from package root: `./afwdev build --cdev -j`. This is the usual C/Python loop: generate, cmake-build, and install core, extensions, and the `afwdev` Python command. It does **not** build the JS app or docs. `-j` enables parallel make.
3. **Implement** — e.g. `src/afw/function/afw_function_<category>.c` (not `generated/function_closet/`).
4. **Test** — `afwdev test -j` runs the Adaptive Script tests (judge success from command output). Narrow with `--srcdir-pattern` / `--pattern` when useful. For memory work or before a major PR, occasionally run `afwdev test --env-mode valgrind -j` (much slower).

Use **`./afwdev`** for builds that refresh/install `afwdev` itself; use **`afwdev`** (PATH) afterward for `test`, `validate`, etc.

Optional: `generate/additional_generate/additional_generate.py` for custom codegen.

Core interface XML: `src/afw/generate/interfaces/afw_interface.xml`.

## Hand-edit vs generated

| Hand-edit | Do not hand-edit |
|-----------|------------------|
| `generate/objects/`, `generate/interfaces/`, `generate/strings/`, `generate/ebnf/*.txt` (file lists only) | `src/*/generated/**`, package `generated/` (incl. `generated/ebnf/`) |
| `function/afw_function_*.c`, runtime `*/afw_*.c`, EBNF blocks in `compile/afw_compile_*.c` | `generated/function_closet/`, binding `*.c`/`*.h`, harvested `generated/ebnf/*.ebnf` |
| `tests/**/*.as`, `tests/**/config.py` | Generated test trees if present |
| `src/afw_dev/_afwdev/**/*.py` | Output those generators write under `generated/` |

Committed `generated/` is for review/grep; **authoritative** output is the last generate.

## afwdev cheat sheet

```bash
# From package root — C/Python day-to-day (generate + cmake + install; not JS/docs)
./afwdev build --cdev -j

# After --cdev, installed afwdev is current
afwdev test -j
afwdev test --env-mode valgrind -j   # occasional; much slower (memory / major PR)
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
| `.cursor/rules/afw-project.mdc` | Always-on (generate/build focus) |
| `.cursor/rules/afw-runtime-model.mdc` | Always-on runtime mental model |
| `.cursor/rules/afw-core-layout.mdc` | `src/afw` module map and usage modes |
| `.cursor/rules/afw-headers.mdc` | Include hierarchy; hand vs generated headers |
| `.cursor/rules/afw-core-services.mdc` | Env consumers: adapter, object, request, auth, model |
| `.cursor/rules/afw-environment.mdc` | Environment registries; core/extension/command registration |
| `.cursor/rules/afw-command.mdc` | `afw` CLI host (`src/afw_command`) |
| `.cursor/rules/afw-server-fcgi.mdc` | `afwfcgi` FastCGI host (`src/afw_server_fcgi`) |
| `.cursor/rules/afw-extensions.mdc` | Loadable extensions (curl/ldap/lmdb/ubjson/vfs/yaml) |
| `.cursor/rules/afw-c-runtime.mdc` | C when editing `.c`/`.h` |
| `.cursor/rules/afw-value-memory.mdc` | Value lifetimes / pools / long-running escape |
| `.cursor/rules/afw-script-eval.mdc` | Compile/eval pipeline, scopes, statement_flow |
| `.cursor/rules/afw-compile.mdc` | `afw_compile.h` API, compile types, parser map |
| `.cursor/rules/afw-compiler-ebnf.mdc` | EBNF-in-comments harvest |
| `.cursor/rules/afw-function.mdc` | Built-in execute_*, polymorphic, compiler_* |
| `.cursor/rules/afw-generate-metadata.mdc` | When editing `generate/` |
| `.cursor/rules/afw-afwdev-python.mdc` | When editing `src/afw_dev` |
| `.cursor/rules/afw-afwdev-generate.mdc` | When editing `_afwdev/generate/` generators |
| `.cursor/rules/afw-json-schema.mdc` | JSON Schema projection / `generated/schemas` / schema tests |
| `.cursor/rules/afw-tests.mdc` | When editing tests (`.as`, Python, `config.py`) |
| `.cursor/skills/add-adaptive-function/` | Add/change Adaptive functions or data types |
| `.cursor/skills/afw-generate-build-test/` | Regenerate, build, validate, test |
| `.cursorignore` | Skips `node_modules/`, `build/`, `generated/`, binaries |

## Other repositories

External AFW packages can implement the same interfaces and add interface XML; `additional_generate/` allows bespoke codegen without forking core afwdev generators. Base-repo extension DSOs (`afw_curl`, `afw_ldap`, …) are documented in [`.cursor/rules/afw-extensions.mdc`](.cursor/rules/afw-extensions.mdc).
