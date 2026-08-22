# AGENTS.md — Adaptive Framework (AFW)

Long-form context for humans and AI assistants. **Always-on** rules live in [`.cursor/rules/afw-project.mdc`](.cursor/rules/afw-project.mdc). Mention **`@AGENTS.md`** in chat when you need this map. Claude Code users: thin stub [`CLAUDE.md`](CLAUDE.md) points here (do not fork a second handbook there).

Primary development focus for Cursor agents: **C runtime** and **Python afwdev**. JS/admin work is secondary unless explicitly requested.

Maintainer **beta brain dump** (not user docs): [`beta-backlog.md`](beta-backlog.md). Per-issue design pads: [`designs/`](designs/README.md). User-facing branch notes on `develop`: [`whats-new.md`](whats-new.md). Open issues live on GitHub. **Optional team lessons** (AI partnering): [`designs/ai-partner-lessons.md`](designs/ai-partner-lessons.md). **Topic atlas** (where is X?): [`designs/knowledge-atlas.md`](designs/knowledge-atlas.md). Support playbooks: [`designs/agent-support.md`](designs/agent-support.md). **Whole story** (why AFW is shaped this way, how the internal parts relate — keep and correct, do not trim maps): [`designs/afw-philosophy-and-core-model.md`](designs/afw-philosophy-and-core-model.md). **#2 lifetime working story:** [`designs/issue-2-lifetime.md`](designs/issue-2-lifetime.md). Mantras: [`designs/mantras-and-working-style.md`](designs/mantras-and-working-style.md) (reference — not always-on).

## Mission

AFW is **metadata-driven**: define object types, functions, data types, and C **interfaces** once, then generate headers, bindings, registration, and docs. The **runtime is C**; **afwdev** is Python.

**Agent role:** act as an **ongoing support and development partner** — accurate mental models, careful implementation, live debugging, and knowledge that survives sessions. **Beta** is a quality bar and current campaign (stabilize, ship, fewer landmines), not an end date for the partnership. Product direction and hard “should we?” choices are grown by **consensus** among maintainers and collaborators — surface options and tradeoffs; do not treat any one voice as decree.

## Knowledge map (where truth lives)

Prefer **git** for durable shared truth (designs, rules, handbook, tests, living issue bodies). Session / harness memory is a sticky index + preferences + thin resume only — **not** the product archive and not a second handbook. After deep sessions, promote maps into the tree so other sessions and people do not depend on local flush.

| Layer | Role |
|-------|------|
| Code + tests | Ground truth; re-verify live when teaching or debugging |
| [`.cursor/rules/*.mdc`](.cursor/rules/) | How to *work* in each area (always-on + area rules) |
| This file (`AGENTS.md`) | System map and agent mission |
| [`designs/`](designs/README.md) | Why, history, open questions; **whole-story** philosophy pad, knowledge atlas, playbooks, mantras |
| [`whats-new.md`](whats-new.md) | User/operator-facing notes on `develop` |
| Handbook / Doxygen | Authors and builders (`src/afw/doc/`, `build/docs/`) |
| Project memory (Grok workspace `MEMORY.md`) | Sticky prefs + resume; **maps over ticket status** |

## How we learn

1. **Consensus first:** open with “what do you think?”, discuss, allow pushback, guide with why and live probes — either side can be wrong; shared understanding beats winning. Same spirit as long-standing human partner work. Detail: [`designs/mantras-and-working-style.md`](designs/mantras-and-working-style.md) (*How consensus is grown*).
2. Issue and PR work builds deep knowledge; **promote maps** (concept cards, symptom→layer→probe→code entry) into git after deep threads — issue ids as pointers only.
3. After a deep session, capture when useful: mental model change, wrong path never to take, live probe, optional user-facing sentence. **Ah-ha → map** (the env/runtime loop is the model case).
4. **Live verify** when teaching or support-debugging (`afw` / `afwfcgi`, runtime objects on `adapterId=afw`, etc.).
5. **Widen goals, not volume** — no dumping every PR status into long memory or long pads.
6. Two tempos: thin session resume vs durable AFW models in rules / designs / this map.

## Possible pattern: work an issue with an AI partner

**Optional** — one way that has worked well for development and support. Not required; not tied to one product. Models and harnesses differ; the pattern is what transfers. Fuller write-up: [`designs/ai-partner-lessons.md`](designs/ai-partner-lessons.md) (*If you try it*).

1. Open your usual harness on this repo (so it can see the tree, and ideally this file).  
2. Start with something like: **“What do you think about issue #N?”** (or paste the issue text / a short symptom). Do **not** lead with “implement a fix” unless you already share the plan.  
3. Discuss: keep asking what the partner thinks; push back; guide with **why** when you disagree.  
4. When chat is stuck, **close the loop live** (`afw`, `afwfcgi`, runtime objects on `adapterId=afw`, the code the partner named).  
5. If you decide to work it: ask for a **feature branch** off the right base (often `develop`), then implement in **small verticals** you both agree (build/test as you go). A useful cadence for wrap-ups and exploratory edges: **flexible plan, one step, then re-decide** — share a living candidate order; finish one step; then **“what do you think we should do next?”** and adapt (see [`designs/mantras-and-working-style.md`](designs/mantras-and-working-style.md)).  
6. **Commit / push when you ask** for a checkpoint — not as a silent default after every edit (unless you agree otherwise for a stretch).  
7. When ready: **“Open a PR”** against the target branch; you still own review. **Name the partner** on the commit and PR (`Co-Authored-By` trailer, a Generated-with line) — same visibility a human co-author would get. Silent human-only authorship of partner work is the wrong default.  
8. After a real click, leave a **thin map** in git if it will help the next person (atlas row, playbook note, or short pad) — issue ids as pointers only.

Support without an issue: steps 1–4 are enough (symptom → discuss → live probe → optional map).

A long crash / use-after-free / overflow review is **not** a public issue list. Keep who/status on a private org Project; open a public `afw` issue only when you start that item and disclosure is OK. See [`designs/agent-support.md`](designs/agent-support.md) (*Disclosure-sensitive C review*).

Deeper optional read: [`designs/ai-partner-lessons.md`](designs/ai-partner-lessons.md). Support-shaped maps: [`designs/agent-support.md`](designs/agent-support.md), [`designs/knowledge-atlas.md`](designs/knowledge-atlas.md).

## Main components

| Area | Role |
|------|------|
| `src/afw` | **libafw core** — pools, values, xctx, compiler, objects/adapters, environment (module map: [`.cursor/rules/afw-core-layout.mdc`](.cursor/rules/afw-core-layout.mdc)) |
| `src/afw_dev` | `afwdev` (generate, build, test, docs, validate, …) |
| `src/afw_command` | `afw` CLI — compile/eval Adaptive syntaxes, conf/extensions, `--local`, optional interactive libedit ([`.cursor/rules/afw-command.mdc`](.cursor/rules/afw-command.mdc)) |
| `src/afw_*` | Loadable extension DSOs — same env registries as core ([`.cursor/rules/afw-extensions.mdc`](.cursor/rules/afw-extensions.mdc): curl, ldap, lmdb, ubjson, vfs, yaml) |
| `src/afw_server_fcgi` | `afwfcgi` FastCGI server — HTTP transport over libafw request handlers ([`.cursor/rules/afw-server-fcgi.mdc`](.cursor/rules/afw-server-fcgi.mdc)) |
| `src/afw_app` | Admin React app + Fiddle (`src/afw_client`) — **POST `/afw` actions**, same env as `afw`/`afwfcgi`. How it talks to core: [`designs/knowledge-atlas.md`](designs/knowledge-atlas.md) §16. Implement JS only when asked. |

This repository is **AFW base** (core + shipped commands/extensions). **`src/afw/`** is core; other `src/<srcdir>/` trees should stay **as self-contained as practical** (movable to another package repo) while using **public** core APIs freely. Core may mention base extensions lightly; detailed extension docs live in that srcdir. See [`.cursor/rules/afw-extensions.mdc`](.cursor/rules/afw-extensions.mdc).

Package manifest: [`afw-package.json`](afw-package.json) (`srcdirs`, `srcdirManifest`, `prefix`, `buildType`).

## Values and memory (short)

- **`afw_value_t`**: interface pointer (`inf`) + payload. Kinds include `compiled_value`, `block`, `call`, `symbol_reference`, `closure_binding`, etc. Full mental model: [`.cursor/rules/afw-runtime-model.mdc`](.cursor/rules/afw-runtime-model.mdc); compile/eval details: [`.cursor/rules/afw-script-eval.mdc`](.cursor/rules/afw-script-eval.mdc).
- **Pools**: general pools (`afw_pool_create*`) are destroy-is-lifetime (parent decides mt vs thread-specific). **Heap / heap tracker** are single-thread only (one compiled_value evaluate). Scopes use trackers. `add_reference` / `release` for escaping values; bulk free when the pool or xctx is destroyed.
- **`compiled_value`**: owns a pool; **everything in that unit is immutable**. Evaluation uses scope-stack discipline and `statement_flow` for leave paths.
- **#2 working story:** [`designs/issue-2-lifetime.md`](designs/issue-2-lifetime.md) — destroy is lifetime; optional `free` is reuse. Slot protocol **landed**. Pool split **landed** (`issue-2-pool-heap`): general APR pool vs evaluation heap/tracker; `managed_p`; compiled_value evaluate creates a heap, clones the result, **releases** the heap. Closures / throw-path rewind (**#35**): store-time bind (literals + nested assign). Archaeology: [`designs/memory-management.md`](designs/memory-management.md). Bindings still generate managed/unmanaged/slice infs — [`.cursor/rules/afw-value-memory.mdc`](.cursor/rules/afw-value-memory.mdc).
- **Terminology:** structured values are **object** (named **properties**) and **array** (ordered). Do **not** call objects “bags” — **bag** is historical (XACML-style bag → list → array). Object **meta** is sideband, not a normal property; map content types may use wire key `"_meta_"`. Always-on detail: [`.cursor/rules/afw-project.mdc`](.cursor/rules/afw-project.mdc).

## Interfaces vs script compiler vs environment

- **Interfaces** — Core, extensions, and commands access C capabilities through contracts defined in `generate/interfaces/*.xml` (core: `afw_interface.xml`) and generated into headers/vtables/skeletons.
- **Environment** — Process-wide keyed registries (`afw_environment_t` / `xctx->env`). Core registers at create (`afw_environment_register_core.c`: `afw_generated_register` then hand wiring — **functions before `prepare_environment`**, then conf/adapters/content types). Extensions/commands use the same registries. Details: [`.cursor/rules/afw-environment.mdc`](.cursor/rules/afw-environment.mdc).
- **Script compiler** — `src/afw/compile/` (`afw_compile.h`) turns syntaxes into `afw_value` graphs; EBNF docs in `/*ebnf>>>` comments harvested via `generate/ebnf/`. Evaluate via `value/` + `function/`. Rules: [`.cursor/rules/afw-compile.mdc`](.cursor/rules/afw-compile.mdc), [`.cursor/rules/afw-script-eval.mdc`](.cursor/rules/afw-script-eval.mdc), [`.cursor/rules/afw-function.mdc`](.cursor/rules/afw-function.mdc), [`.cursor/rules/afw-compiler-ebnf.mdc`](.cursor/rules/afw-compiler-ebnf.mdc).

### Interfaces, macros, scaffolds, Doxygen

C interfaces are an **XML IDL + Python generator** story so AFW can stay C-efficient while giving implementers a clean call surface:

| Layer | Role |
|-------|------|
| **Call macros** (`afw_<iface>_<method>(…)`) | **Real developer API** — document these (via XML + `interfaces.py`) |
| **`inf` / arrow forms** | Wiring and GDB; not what extension authors should learn first |
| **Closet skeletons + afwdev `make-*` / `add-*`** | First-class bootstrap for packages, extensions, commands, interface impls; `@todo` and `<afwdev {…}>` are intentional |
| **Doxygen groups** (`afw_doxygen.h`) | Map for **builders** (core / extension / command authors), not pure Adaptive Script app users |

Do **not** hand-edit `generated/` for docs, and do **not** “fix” skeleton placeholders for Doxygen vanity. Full rule: [`.cursor/rules/afw-interfaces-doxygen.mdc`](.cursor/rules/afw-interfaces-doxygen.mdc). Planned thin developer MD pages may live under `src/afw/doc/developer/` (Doxygen + direct read).

Authoritative coding conventions: [`src/afw/doc/guide/developer/contributing.xml`](src/afw/doc/guide/developer/contributing.xml). Packages: [`packages.xml`](src/afw/doc/guide/developer/packages.xml).

## Metadata → generate → implement → test

```text
edit generate/ or hand C/Python  →  ./afwdev build --cdev  →  afwdev test -j
# full package dev install / before PR (maintainer default):
#   ./afwdev build --fulldev  →  afwdev test -j --env-mode valgrind
```

1. **Edit** `src/<srcdir>/generate/` — e.g. `objects/_AdaptiveFunctionGenerate_/*.json`, `interfaces/*.xml` — and/or hand C under `src/afw/…`.
2. **Build (C-dev)** — from package root: `./afwdev build --cdev`. This is the usual C/Python loop: generate, cmake-build (parallel), and install core, extensions, and the `afwdev` Python command. It does **not** build the JS app or docs.
3. **Implement** — e.g. `src/afw/function/afw_function_<category>.c` (not `generated/function_closet/`).
4. **Test** — `afwdev test -j` runs the Adaptive Script tests (judge success from command output). Narrow with `--srcdir-pattern` / `--test-pattern` when useful.

**Before commit/push** (docs, multi-area, finish pass — not every one-line C fix): prefer  
`./afwdev build --fulldev` (or at least a docs-aware build). `--cdev` alone will not catch handbook XML/docs-builder failures.

**Full build and test before a PR** (maintainer default; also when the user asks for full verify):  
`./afwdev build --fulldev` then `afwdev test -j --env-mode valgrind`.  
`--fulldev` is short for **`--all --generate --clean --install --scan`** plus **parallel jobs (`-j`)**: all contexts (C, docs, JS, docker tags), regenerate from package metadata (including version), clean trees, install, and clang analyze-build. Valgrind is much slower — not for every edit. Note: **`--all` alone does not run generate or install**.

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
# From package root — C/Python day-to-day (generate + cmake + install + -j; not JS/docs)
./afwdev build --cdev

# After --cdev, installed afwdev is current
afwdev test -j
afwdev test --srcdir-pattern afw --test-pattern 'rql/.*'
afwdev validate --pattern 'src/afw/generate/objects/...'

# Full package dev install (all contexts + generate + clean + install + scan + -j):
./afwdev build --fulldev

# Full verify before PR (maintainer default; also when user asks for full build/test):
./afwdev build --fulldev
afwdev test -j --env-mode valgrind   # much slower

# Narrow generate only (usually unnecessary if using --cdev / --fulldev)
afwdev generate --srcdir-pattern '*'
```

`--cdev` and `--fulldev` are convenience profiles (both include **`-j`** / parallel cmake unless you pass **`-j N`**). `--cdev` = generate/clean/install/-j for C work (default cmake context; no docs/JS/docker). `--fulldev` = `--all --generate --clean --install --scan` plus `-j` (version headers, Doxyfile `PROJECT_NUMBER`, handbook, JS, docker tags, clang scan). **`--all` alone does not generate or install.** Both define `AFW_DEBUG_EVALUATION`, `AFW_DEBUG_LOCK`, and `AFW_DEBUG_POOL` (runtime flags still off unless set). Extra C preprocessor defines: `afwdev build --define NAME` or `--define NAME=VALUE`. CMake output lives under `build/cmake/`.

## Documentation

- Author: `src/afw/doc/` (XML, markdown, images); builder-oriented Doxygen MD under `src/afw/doc/developer/`.
- Publish: included in `./afwdev build --fulldev` (or `--all --generate --install` / `afwdev build --docs`) → `build/docs/` (served as `/docs/...` when installed).
- **Doxygen:** afwdev runs `doxygen Doxyfile` into `build/docs/doxygen/` only if that directory is missing; use **`./afwdev build --docs --clean -j`** to force a refresh. `--clean` only cleans the **active** build context(s) (e.g. `--docs --clean` wipes `build/docs/`, not cmake). Config is the checked-in `Doxyfile`; see `afw-interfaces-doxygen`.
- **Doxygen HTML skin:** experimental light/dark slate sheet `src/afw/doc/doxygen-extra.css` (`HTML_EXTRA_STYLESHEET`). Maintainer notes in that file’s header and `src/afw/doc/developer/doxygen-skin.md` — read before “simplifying” menus/`div.header` overrides.
- `--cdev` installs libs/headers/`afwdev` via cmake; it does not build the handbook or admin app.

## Cursor layout

| Path | Role |
|------|------|
| `.cursor/rules/afw-project.mdc` | Always-on (generate/build focus) |
| `.cursor/rules/afw-runtime-model.mdc` | Always-on runtime mental model |
| `.cursor/rules/afw-interfaces-doxygen.mdc` | Always-on: interface macros, afwdev scaffolds, Doxygen for builders |
| `.cursor/rules/afw-core-layout.mdc` | `src/afw` module map and usage modes |
| `.cursor/rules/afw-headers.mdc` | Include hierarchy; hand vs generated headers; Doxygen file hygiene |
| `.cursor/rules/afw-core-services.mdc` | Env consumers: adapter, object, request, auth, model; retrieve max-objects limit (#49) |
| `.cursor/rules/afw-environment.mdc` | Environment registries; core/extension/command registration |
| `.cursor/rules/afw-environment-variables.mdc` | Process env / `process::` / request props; env create ambient; #71 single `current` |
| `.cursor/rules/afw-command.mdc` | `afw` CLI host (`src/afw_command`) |
| `.cursor/rules/afw-server.mdc` | `afw_server` / `afw_request` hosts (interface vs FCGI/local/future HTTP) |
| `.cursor/rules/afw-server-fcgi.mdc` | `afwfcgi` FastCGI host (`src/afw_server_fcgi`) |
| `.cursor/rules/afw-stream.mdc` | Streams, `open_file`, rootFilePaths, progressive response write path (≠ retrieve limit/paging; see #49 / core-services) |
| `.cursor/rules/afw-vfs.mdc` | VFS adapter (`afw_vfs`): vfsMap, whole-file data, multi-map, tests (#79) |
| `.cursor/rules/afw-extensions.mdc` | Loadable extensions (curl/ldap/lmdb/ubjson/vfs/yaml) |
| `.cursor/rules/afw-adapter-index.mdc` | Adapter indexes (core + LMDB): definitions in DB, `current::` eval (#54), create/txn residuals (#57) |
| `.cursor/rules/afw-c-runtime.mdc` | C when editing `.c`/`.h` |
| `.cursor/rules/afw-value-memory.mdc` | Value lifetimes / pools / shared mutables / long-running escape |
| `.cursor/rules/afw-script-eval.mdc` | Compile/eval pipeline, scopes, statement_flow |
| `.cursor/rules/afw-compile.mdc` | `afw_compile.h` API, compile types, parser map |
| `.cursor/rules/afw-compiler-ebnf.mdc` | EBNF-in-comments harvest |
| `.cursor/rules/afw-function.mdc` | Built-in execute_*, polymorphic, compiler_*, default-clone |
| `.cursor/rules/afw-adaptive-script.mdc` | Adaptive Script authoring (equality, nullish, defaults, probes) |
| `.cursor/rules/afw-qualified-variables.mdc` | Qualifier stack, get vs `qualifier()`/`qualifiers()` snapshots (#9), `current::`/`custom::`, `variable_get`, includeUntrusted |
| `.cursor/rules/afw-generate-metadata.mdc` | When editing `generate/` |
| `.cursor/rules/afw-afwdev-python.mdc` | When editing `src/afw_dev` |
| `.cursor/rules/afw-afwdev-generate.mdc` | When editing `_afwdev/generate/` generators |
| `.cursor/rules/afw-json-schema.mdc` | JSON Schema projection / `generated/schemas` / schema tests |
| `.cursor/rules/afw-tests.mdc` | When editing tests (`.as`, Python, `config.py`), regression style |
| `.cursor/skills/add-adaptive-function/` | Add/change Adaptive functions or data types |
| `.cursor/skills/afw-generate-build-test/` | Regenerate, build, validate, test |
| `.cursorignore` | Skips `node_modules/`, `build/`, `generated/`, binaries |

## Other repositories

External AFW packages can implement the same interfaces and add interface XML; `additional_generate/` allows bespoke codegen without forking core afwdev generators. Base-repo extension DSOs (`afw_curl`, `afw_ldap`, …) are documented in [`.cursor/rules/afw-extensions.mdc`](.cursor/rules/afw-extensions.mdc). Working in another Git repo **next to** this one (sibling checkout, write wall, prime kit): [`designs/sibling-afw-package.md`](designs/sibling-afw-package.md).
