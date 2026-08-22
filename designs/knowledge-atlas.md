# AFW knowledge atlas (first pass — 2026-08)

**Audience:** maintainers and AI assistants in the ongoing support + development partner role.  
**Not** handbook, not always-on rules, not a second copy of every pad.

**Job of this file:** topic → best sources → live probe → open/residual → gaps.  
**Ground truth:** code + tests. When this atlas drifts, fix the atlas (or the pad it points at).

**How to use**

1. Land on a **topic** below.  
2. Open **day rules** for how to work; **deep pad** for why/history; **probe** to verify.  
3. For the **whole story** (why + how parts relate): [`afw-philosophy-and-core-model.md`](afw-philosophy-and-core-model.md). Mantras: [`mantras-and-working-style.md`](mantras-and-working-style.md).  
4. For symptom-shaped support: [`agent-support.md`](agent-support.md).

**Hubs:** [`AGENTS.md`](../AGENTS.md) · [`designs/README.md`](README.md) · [`beta-backlog.md`](../beta-backlog.md) · [`whats-new.md`](../whats-new.md)

---

## How the C surface fits (one picture)

Everything we paired on in the `mgg-develop` wave hangs off this. Use it when a symptom could be “any layer.”

```text
generate/  →  generated/  →  env registries (afw_environment_t)
                                   │
                    runtime objects on adapterId=afw
                                   │
              ┌────────────────────┴────────────────────┐
              │                                         │
     GET /afw/<type>/[id]                      POST /afw
     (adapter CRUD)                    (actions = any function)
              │                                         │
              └──────────── same process env ───────────┘
                                   │
         afw CLI          afwfcgi + nginx :8080         admin Fiddle
         compile/eval     real request / services       AfwClient.perform
                                   │
              compile → value graph → evaluate → execute_*
                                   │
         objects / arrays (faces)   pools / xctx   adapters / streams
```

**Wrong paths that cost us sessions:** treat `EnvironmentRegistry/current` as the only catalog (functions dominate; prefer typed retrieve). Forget `POST /afw` is a different door from REST GET. Mix **create** (compile, `argv[0]` is the callee expression) with **evaluate** (`x->function` is harvest). Restart tests after `--cdev` without restarting a long-lived `afwfcgi` (it still maps deleted `libafw`).

**Campaign index** (what we actually paired on — pads, not ticket diaries):

| Area | Pointers |
|------|----------|
| Env / runtime catalog / accessors | #149 pads; atlas §5; metrics/properties pin until caller pool cleanup |
| Hosts / stop | #158; atlas §6 |
| Memory / faces / `create_array` | [`issue-2-lifetime.md`](issue-2-lifetime.md); names-as-values slice; #17 faces; atlas §3 |
| Types | #28 pad + `typescript-differences.md`; #186 parse nesting; #188 names declared before use |
| Script language | #62 pad; #33 errors; #172 must-change |
| Compile / call / spread | `afw-script-eval`; #140 / #181; `compiler_internal` kind-check |
| Arrays / converts / UTF-8 | #39, converts pad, #153; #190 empty-match `replace` |
| Streams / VFS / retrieve | stream + vfs rules; #127; #49 |
| afwdev / tests | recipe + tests-extra SCHEMA; #157; C probes #207 |
| Crypto | #74 pad |
| Admin / Fiddle | atlas §16 (contract only) |

---

## Layer cheat sheet (where truth lives)

| Layer | Role | Do not use it for |
|-------|------|-------------------|
| Code + tests | Ground truth | — |
| `.cursor/rules/*.mdc` | How to *work* in an area | Long history diaries |
| `AGENTS.md` | System map, mission, build loop | Deep design archaeology |
| `designs/*` | Why, options, open questions, maps | Always-on law |
| `whats-new.md` | User/operator-facing on `develop` | Internal implementation plans |
| Handbook / Doxygen | Authors and builders | Issue numbers / designs paths (handbook) |
| Project MEMORY | Sticky prefs + thin resume | Second handbook; ticket dumps |

---

## Topic index

| Topic | Settled? | Start here |
|-------|----------|------------|
| [0. Orientation / product taste](#0-orientation--product-taste) | Yes (thin) | `ai-partner-lessons` + mantras + philosophy |
| [1. Metadata, generate, packages](#1-metadata-generate-packages) | Yes | `afw-project`, generate rules |
| [2. Core layout & C conventions](#2-core-layout--c-conventions) | Yes | `afw-core-layout`, `afw-c-runtime` |
| [3. Values, pools, memory (#2)](#3-values-pools-memory-2) | Partial | value-memory + memory-management pad |
| [4. Compile, EBNF, eval, functions](#4-compile-ebnf-eval-functions) | Yes (core); residuals open | compile + script-eval + function |
| [5. Environment & runtime objects](#5-environment--runtime-objects) | #149 closed | runtime-objects pad + environment rule |
| [6. Hosts & process lifetime](#6-hosts--process-lifetime) | #158 closed | server-fcgi + command rules |
| [7. Core services (adapter/object/request)](#7-core-services-adapterobjectrequest) | Ongoing | core-services |
| [8. Streams, files, VFS](#8-streams-files-vfs) | Mostly | stream + vfs rules |
| [9. Qualified variables & context](#9-qualified-variables--context) | #9 landed; watch | qualified-variables |
| [10. Adaptive Script authoring](#10-adaptive-script-authoring) | Ongoing | adaptive-script + script-errors |
| [11. afwdev: build / generate / test](#11-afwdev-build--generate--test) | Gate solid; lab experimental | test recipe + tests rule |
| [12. Extensions (shipped base)](#12-extensions-shipped-base) | Thin map | extensions rule |
| [13. Docs surfaces](#13-docs-surfaces) | Split known | AGENTS docs + interfaces-doxygen; overview points at maps |
| [14. Security / crypto](#14-security--crypto) | Design-heavy | secrets pad |
| [15. Language surface residuals](#15-language-surface-residuals-pointers-only) | Mixed closed/open | issue pads |
| [16. Admin app, Fiddle, actions](#16-admin-app-fiddle-actions) | Thin; core contract known | this § + env playbook |

---

## 0. Orientation / product taste

| Field | Content |
|-------|---------|
| **Settled map** | Ongoing partner role; beta = quality bar; hard choices by **consensus** (ask what you think → discuss → live verify → either side can be wrong) |
| **Day rules** | `afw-project` (role one-liner, hard rules, terminology, plain language) |
| **Deep / reference** | [`ai-partner-lessons.md`](ai-partner-lessons.md) (optional team one-pager), mantras (*How consensus is grown*), philosophy pad (*Adaptive concepts*), agent-support |
| **Probe** | Live system when debating models (`afwfcgi`, env registries) — not CI |
| **Open** | More mantras as shared over time |
| **Gap** | None critical; grow mantras pad, don’t invent phrases |
| **Also** | Crash-shaped review lists: private Project + how-to issue; public `afw` one card at a time — [`agent-support.md`](agent-support.md) *Disclosure-sensitive C review*. Base vs private packages, Docker ICU/APR floor, ICU home: [`lineage-and-library-floor.md`](lineage-and-library-floor.md) |
| **Origin note** | Env/runtime “smaller chunks → live discovery → adapters click” discussion: genesis of this support-partner path (ah-ha → map) |

---

## 1. Metadata, generate, packages

| Field | Content |
|-------|---------|
| **Settled map** | Humans edit `generate/`; never hand-edit `generated/`; metadata SoT; “get it right at most once” |
| **Day rules** | `afw-project`, `afw-generate-metadata`, `afw-afwdev-generate`, `afw-json-schema`, `afw-interfaces-doxygen` |
| **Deep pad** | Philosophy pad (method); contributing/packages handbook XML |
| **Probe** | `./afwdev build --cdev`; `afwdev validate --pattern '…'`; grepping `generated/` only for review |
| **Open** | Package-move discipline for non-core srcdirs (ongoing practice) |
| **Gap** | No single “how to add a package” playbook beyond scaffold + handbook — OK until asked |

---

## 2. Core layout & C conventions

| Field | Content |
|-------|---------|
| **Settled map** | One subdirectory ≈ one concern; includes hierarchy; pools/xctx/errors patterns |
| **Day rules** | `afw-core-layout`, `afw-c-runtime`, `afw-headers` |
| **Deep pad** | — |
| **Probe** | Open matching `src/afw/<module>/`; public API via `afw.h` / generated interface headers |
| **Open** | Line-length soft preference (~80); no mass reformat |
| **Gap** | Non-pool cleanup mantra (NULL + TRY/FINALLY) lives in rules + workspace MEMORY — already in `afw-c-runtime` |

---

## 3. Values, pools, memory (#2)

| Field | Content |
|-------|---------|
| **Settled map** | Values first. **Working story:** [`issue-2-lifetime.md`](issue-2-lifetime.md) — destroy is lifetime; optional `free` is reuse; managed = hold protocol under `xctx->p`; unmanaged = optional holds, zero does not destroy; compiled unit immutable; assign is the script hold site; scalar `add_reference` boxes in `xctx->p`; objects/arrays hold the instance; script mutates a face. Payloads (`afw_utf8_t` / `afw_memory_t`) have no pool; doors are `create`/`set`/`no_copy`/`forced_safe` — [`c-naming-and-payloads.md`](c-naming-and-payloads.md) |
| **Day rules** | `afw-runtime-model` (always-on), `afw-value-memory` (**current tree**; campaign target is the lifetime pad), `afw-script-eval` |
| **Deep pad** | [`issue-2-lifetime.md`](issue-2-lifetime.md) (**campaign map**); archaeology [`memory-management.md`](memory-management.md); philosophy core model; names-as-values [`issue-2-property-name-values.md`](issue-2-property-name-values.md); faces [`issue-17-mutable-object-faces.md`](issue-17-mutable-object-faces.md) |
| **Probe** | Targeted `.as` + `afwdev test -j --env-mode valgrind`; orchestrated multi-request leaves; never “fix memory” without a metric/story |
| **Open** | Umbrella **#2**. Slot protocol landed. Pool split landed (`issue-2-pool-heap`: general APR vs heap/tracker, `managed_p`). **Names as values** landed (PR **#220**). Closures / throw-path rewind (**#35**) store-time bind (literals + nested assign; no hoisting). Remaining: optional `free`, first-fit/P3. |
| **Gap** | Lifetime pad is the one-pager that was missing; keep `memory-management.md` as archaeology |

---

## 4. Compile, EBNF, eval, functions

| Field | Content |
|-------|---------|
| **Settled map** | Compile → value graph → evaluate; EBNF-in-comments harvest; functions metadata + hand execute; statement_flow control |
| **Day rules** | `afw-compile`, `afw-compiler-ebnf`, `afw-script-eval`, `afw-function` |
| **Deep pads** | `compile-optimize-notes`, `pragma-hash-design`, `decompile-compiler-internal-inventory`, `compile-contextual-audit`, `adaptive-function-compile-typecheck`, `issue-28-type-syntax` |
| **Probe** | `afw -s '…'` / tests under `src/afw/tests/`; regenerate EBNF via `--cdev`; check `generated/ebnf/syntax.ebnf` has `::=` for nonterminals |
| **Open** | Compile optimize future (`compile-optimize-notes`); app-shared / script poly (**#170**); #28 **closed** (PR **#171**) — map: `issue-28-type-syntax.md` + `typescript-differences.md` + handbook Types (git archive; not session memory). Named `type` / `interface` names must be declared before use (self-ref in the same statement is allowed; bare alias cycles error). Type and destructure parse nesting is limited (`AFW_COMPILE_PARSE_NESTING_MAX`). |
| **Gap** | Support playbook for “parse error / decompile mismatch” still thin — use rules + decompile pads. `compiler_internal` `execute_*` must kind-check (`afw-function`). |

---

## 5. Environment & runtime objects

| Field | Content |
|-------|---------|
| **Settled map** | Process-wide registries; everything registered discoverable on `adapterId=afw`; runtime objects = immutable views; #149 accessor reliability shipped. Two doors, same env: **GET** adapter CRUD vs **POST `/afw`** actions (any function). Map foreach ≠ `runtime_custom` (services show 0 on the shell, retrieve still lists them). Adapter **type** (factory) ≠ adapter **instance**. Conf `contextType` points at `_AdaptiveContextType_` (which qualifiers exist for that script/template). |
| **Day rules** | `afw-environment`, `afw-environment-variables`, `afw-core-services` (runtime section) |
| **Deep pads** | [`runtime-objects-and-environment.md`](runtime-objects-and-environment.md) (architecture), [`runtime-value-accessors.md`](runtime-value-accessors.md) (catalog snapshot), [`runtime-catalog-lifetime.md`](runtime-catalog-lifetime.md) (discovery notes) |
| **Probe** | Typed `retrieve_objects` / GET `/afw/_Adaptive…_/`; `/afw/_AdaptiveRuntimeValueAccessor_/`; **do not** stop permanent `adapter-afw` / `adapter-conf`; prefer typed retrieve over full `current` materialize |
| **Open** | Residuals under **#2** (managed object/array, long-running escape). Metrics/properties pin the instance until the caller pool is cleaned up (not a deep snapshot; not a managed wrapper). Runtime objects still map over live data; const graphs are fine. A later #2 option is clone-into-requestor-pool **under the lock** so changing data need only live long enough to copy. More adapters may need terminating checks; attach lifecycle for orchestrated leaves not fully built |
| **Gap** | Live-stack / action probes now in `agent-support`. Architecture pad remains the deep map. |

**Registry discovery (condensed)**

| Want | How |
|------|-----|
| Kind list | `AFW_ENVIRONMENT_REGISTRY_TYPE_MAP` or `_AdaptiveEnvironmentRegistry_/current` / registry types |
| All of one kind | `retrieve_objects(afw, _Adaptive…_, maxObjects=0)` |
| One | `get_object` / `/afw/<type>/<id>` |
| Accessor contract | `/afw/_AdaptiveRuntimeValueAccessor_/<key>` |

**maxObjects:** default ~100 may error (`payload_too_large`); **0 = unlimited**.

---

## 6. Hosts & process lifetime

| Field | Content |
|-------|---------|
| **Settled map** | #158 closed (PR **#165**): host-specific wake + core `terminating` |
| **Day rules** | `afw-server`, `afw-server-fcgi`, `afw-command` |
| **Deep pad** | [`lineage-and-library-floor.md`](lineage-and-library-floor.md) — Docker bases are the ICU/APR floor (this Ubuntu container is not the oldest) |
| **Probe** | `src/afw/tests/advanced/afwfcgi_signal_shutdown/`; `afwfcgi --help`; after `--cdev`/`--install`, restart long-lived `afwfcgi` (stale process maps deleted `libafw` — CLI `afw` does not) |
| **Open** | Drain timeout, SIGHUP, Windows service, general signal framework, every extension retrieve loop, `--local` read unblock |
| **Gap** | Stale-`afwfcgi` and GDB `-n 1` notes now in `agent-support` live-stack playbook |

**Host contracts**

| Host | SIGTERM/SIGINT |
|------|----------------|
| **`afwfcgi`** | `env->terminating`; FCGI shutdown pending; **SIGUSR1** each request thread; join; close listen; **unlink Unix `-p` path** (not TCP) |
| **`afw`** | Set `env->terminating` only |

**Core:** error code `terminating` → HTTP **503**; `AFW_XCTX_THROW_IF_TERMINATING(xctx)` at work-unit boundaries.

**Wrong path:** close listen fd alone to wake multi-thread accept while main is in join — use **thread signal** for libfcgi. A throw in `create_request` is outside the request `TRY` — empty 200, unhandled thread error; parse/validate that needs an HTTP error body *after* the request exists.

---

## 7. Core services (adapter/object/request)

| Field | Content |
|-------|---------|
| **Settled map** | Adapters as normalized object stores; model maps; request handlers; auth; retrieve limits (#49) |
| **Day rules** | `afw-core-services`, `afw-model-adapter`, `afw-adapter-index` |
| **Deep pads** | Issue/theme pads as needed; model optional `mappedAdapterId` (#109) in model rule |
| **Probe** | Adapter CRUD via tests; `service_start/stop/restart/get`; lifecycle leaf under `tests-extra` |
| **Open** | Index create/txn residuals (#54/#57); more adapter types; retrieve paging vs progressive stream (not the same as #49 limit) |
| **Gap** | No dedicated “adapter write path” playbook — use core-services + tests |

---

## 8. Streams, files, VFS

| Field | Content |
|-------|---------|
| **Settled map** | Streams host-agnostic for response body; `rootFilePaths` / open_file; VFS is separate adapter (`vfsMap`) |
| **Day rules** | `afw-stream`, `afw-vfs` |
| **Deep pad** | — |
| **Probe** | Stream tests under `tests/miscellaneous/stream_file`; VFS tests in `src/afw_vfs` |
| **Open** | Progressive retrieve vs count limits (see stream rule notes) |
| **Gap** | Thin; OK |

---

## 9. Qualified variables & context

| Field | Content |
|-------|---------|
| **Settled map** | Qualifier stack get vs `qualifier()`/`qualifiers()` snapshots (#9); `current::` / `custom::`; includeUntrusted |
| **Day rules** | `afw-qualified-variables`, environment-variables (process::) |
| **Deep pad** | — |
| **Probe** | `src/afw/tests/compiler/qualifier*.as` |
| **Open** | Watch contribute_cb / untrusted edges |
| **Gap** | Support one-liner only unless symptoms appear |

---

## 10. Adaptive Script authoring

| Field | Content |
|-------|---------|
| **Settled map** | Object/array terminology; equality/nullish quirks; prefer throw + try; `_AdaptiveError_` |
| **Day rules** | `afw-adaptive-script`, `afw-script-errors` |
| **Deep pads** | array-semantics, conversion-functions, utf8-code-point-sequences, issue-17 faces, issue-38, issue-138 meta wire, [`issue-33-error-codes.md`](issue-33-error-codes.md) |
| **Probe** | Language suite under `src/afw/tests/`; Fiddle / `afw -s` |
| **Open** | Language residuals tracked in issues/pads |

---

## 11. afwdev: build / generate / test

| Field | Content |
|-------|---------|
| **Settled map** | **Gate** vs **lab**; **orchestrated** leaves (`orchestration.yaml`); blast **retired** (PR **#167**); handbook Developer Guide **Writing Tests**; gate `src/afw/tests/README.md`; extras SCHEMA in `tests-extra/`; C-only holes use `run_c_probe()` (`_afwdev.test.c_probe`) on a checked-in `*_probe.c` — not cmake; start with `afwdev prime-test-c-probe <path>`; `--env-mode valgrind` wraps those binaries with suite suppressions ([#207](https://github.com/afw-org/afw/issues/207); [`c-probes.md`](c-probes.md)) |
| **Day rules** | `afw-tests`, `afw-afwdev-python`, `afw-afwdev-generate` |
| **Deep pads** | [`afwdev-test-recipe.md`](afwdev-test-recipe.md), [`c-probes.md`](c-probes.md), [`afwdev-advanced-test.md`](afwdev-advanced-test.md) (history), [`afwdev-blast.md`](afwdev-blast.md) (retired); `src/afw/tests-extra/{README,SCHEMA}.md`; handbook `guide/developer/writing-tests.xml` |
| **Probe** | See recipe commands below |
| **Open** | #13 stress knobs/stats on `test` (Jeremy); attach mode not fully built |
| **Gap** | MEMORY testing split → **in recipe + playbook**; drop duplicating full MEMORY novel after promote |

**Intent split**

| Surface | Job | Default gate? |
|---------|-----|----------------|
| `afwdev test -j` | Language/package suite + short orchestrated leaves under `src/*/tests/` | **Yes** |
| `afwdev test -T path` | Opt-in only (`tests-extra/`, etc.) | Opt-in |
| `afwdev test --env-mode afwfcgi` | Conf-free `.as` on live `:8080/afw` (skips hermetic orchestrated leaves) | Optional |
| `afwdev test --env-mode valgrind -j` | Memory check on `.as` via `afw` **and** on C probes via `run_c_probe()`; heavy — fewer jobs often faster wall-clock | Optional full verify |
| `schedule.firehose` leaves under `tests-extra/` | Load thrash at hermetic afwfcgi | **No** |

```bash
./afwdev build --cdev          # day-to-day C/Python
./afwdev build --fulldev       # PR-shaped / docs-aware
afwdev test -j
afwdev test --test-pattern catalog-value-accessors --show-all
afwdev test -T src/afw/tests-extra/adapter-lifecycle --show-all
afwdev test -T src/afw/tests-extra/07b-firehose-catalog-pool
```

After install, **restart afwfcgi** if attach tools talk to a long-lived process.

---

## 12. Extensions (shipped base)

| Field | Content |
|-------|---------|
| **Settled map** | curl, ldap, lmdb, ubjson, vfs, yaml — same env registries; self-contained vs core |
| **Day rules** | `afw-extensions`, plus vfs / adapter-index (lmdb) as specialized |
| **Deep pad** | Per-extension docs under srcdir; secrets/crypto separate |
| **Probe** | Extension tests under `src/afw_*/tests/` |
| **Open** | Index residuals on LMDB; other adapters index interface NULL |
| **Gap** | No per-extension support cards — add when symptoms cluster |
| **Also** | Not-yet-public extensions live in `inter-afw-private` and promote to this repo if they become base — [`lineage-and-library-floor.md`](lineage-and-library-floor.md). Any sibling AFW package: [`sibling-afw-package.md`](sibling-afw-package.md) |

---

## 13. Docs surfaces

| Field | Content |
|-------|---------|
| **Settled map** | Handbook ≠ Doxygen ≠ EBNF railroad chrome; Jeremy handbook voice; short signposts to repo maps for builders |
| **Day rules** | `afw-interfaces-doxygen`, `afw-project` prose; AGENTS Documentation section |
| **Deep pad** | `src/afw/doc/developer/doxygen-skin.md` etc. |
| **Probe** | `./afwdev build --docs --clean -j` when docs must refresh |
| **Signposts to maintainer maps** | `src/afw/doc/developer/overview.md` (table); handbook `guide/developer/introduction.xml` (short paragraph) → `AGENTS.md`, `designs/` |
| **Open** | Thin developer MD under `src/afw/doc/developer/` |
| **Gap** | Workspace MEMORY has chrome details — keep sticky in MEMORY prefs; not required in atlas body |

---

## 14. Security / crypto

| Field | Content |
|-------|---------|
| **Settled map** | Design in progress / #74 oriented |
| **Day rules** | C TRY/FINALLY for OpenSSL in `afw-c-runtime` |
| **Deep pad** | [`secrets-and-afw-crypto.md`](secrets-and-afw-crypto.md) (**large**) |
| **Probe** | Crypto tests / functions when touching that tree |
| **Open** | Product decisions in pad |
| **Gap** | Atlas points only — do not summarize 1k-line pad here |

---

## 15. Language surface residuals (pointers only)

| Theme | Pad / note | Status |
|-------|------------|--------|
| Array semantics | `array-semantics.md` | Shipped (#39) |
| Converts | `conversion-functions.md` | Shipped with #39 wave |
| UTF-8 / memory C doors | [`c-naming-and-payloads.md`](c-naming-and-payloads.md) | `create` copies; `no_copy` points; `forced_safe` `^hex^`; `src/afw/code_point/` |
| UTF-8 code points (script) | `utf8-code-point-sequences.md` | #153 oriented; #190 empty-match `replace`; `to_lower` / ignore-case compare use bounded `U8_NEXT` / `U8_APPEND` and independent offsets ([#206](https://github.com/afw-org/afw/issues/206)) |
| Mutable faces | `issue-17-mutable-object-faces.md` | Closed PR #150 |
| Expression property names | `issue-38-…` | Closed |
| Meta on wire | `issue-138-…` | Design/status in pad |
| Decompile | `issue-18-…` | Pad useful; stringify closed |
| Types | `issue-28-…` | **#28 closed** (PR **#171** wrap-up) |
| Pragma `#` | `pragma-hash-design.md` | Pattern B `#compile` (landed) |
| Error codes / HTTP / `e.id` | `issue-33-error-codes.md` | **#33** closed (PR **#173**) — map in `afw_common.h`; script `throw` may set `id`; prefer `e.id` |
| Script language syntax | `issue-62-script-language.md` | **#62** — multi `let`/`const`, `for` init, assignment chain, running result, loop labels landed |
| Pattern params / catch / call-site `...` | `compile-optimize-notes.md` (#140) | Landed; `execute_try` still trusts parser unless kind-checked (`afw-function`) |
| Evaluate spread once | `afw-script-eval` + whats-new | **#181** closed — do not mix with `x->function` harvest |
| Deprecated throw / declare_helpers | `whats-new.md` Must change | **#172** — `throw` needs `data`; no package `*_declare_helpers.h` |
| #28 decided-not | `issue-28-type-syntax.md` + `typescript-differences.md` | No `Array<T>`, no index signatures / literal types / `readonly` / `enum` / `implements` in script types. Type/pattern parse nesting is limited. |
| App-shared Adaptive functions | **#170** (hold) | Not a leftover of #28 wrap-up; do not mix onto #2 |

---

## 16. Admin app, Fiddle, actions

Jeremy’s stack. Core work does **not** implement JS unless asked; the **contract** is C / request / env.

| Field | Content |
|-------|---------|
| **Settled map** | Admin React app `src/afw_app` + JS client `src/afw_client` talk to the **same** env as `afw` / tests. Fiddle is `AfwClient.perform(payload)` → **`POST /afw`** JSON (`function` + params, or `actions[]`). Scripts: `function: "eval<script>"` (or `eval_script`), `source: <script>`. REST **GET** `/afw/<type>/[id]` is adapter CRUD, not the Fiddle door. Optional stream `Accept: application/x-afw`. |
| **Day rules** | `afw-server`, `afw-server-fcgi`, `afw-environment`; JS work only when explicitly requested |
| **Deep pad** | None required; env architecture pad for registries; `issue-18` for Fiddle listing |
| **Probe** | Dev container: nginx `:8080` → `/var/run/afw.sock` → `afwfcgi`. Curl POST `/afw` with `{"function":"get_object",…}`. Python `Session` + `Request().add_action(…).perform()`. After rebuild, restart `afwfcgi` or the app still talks to a deleted binary. |
| **Open** | Admin JS leftovers (#53 mocks, #101 handbook eval names) — not a C feature branch |
| **Gap** | Playbook in `agent-support` (actions / Fiddle / stale process). Do not grow an app design pad unless Jeremy asks. |

**AFWDev conf** (dev container) already loads extensions such as `afw_crypto` at boot. Measuring `extension_load` deltas needs a **minimal** conf or you will see “already loaded.”

---

## Cross-cutting mantras (see mantras pad)

Most used when navigating this atlas:

- **Get it right at most once** — metadata / shared accessor / shared helper  
- **Values first** — then pools  
- **Maps over tickets** — issue ids are pointers  
- **Gate vs lab** — don’t redefine `test -j` as soak  
- **Live verify** when teaching  
- **Code wins** when notes drift  
- **Don’t let the right way get in the way of getting work done** — patterns, then judgement  

---

## Gaps backlog (first-pass harvest)

Items worth future promote/fill — **not** blocking this atlas:

| Gap | Suggested home | Priority |
|-----|----------------|----------|
| Leak / long-run “first 15 minutes” support card | agent-support or thin pointer into memory pad | Medium (#2 active) |
| Parse/decompile mismatch playbook | agent-support | Low until pain |
| Adapter write / conf service lifecycle card | agent-support (type vs instance + recipe) | Lower — type/instance card filled |
| Attach mode orchestrated leaves | afwdev-advanced-test pad / SCHEMA | When building |
| Admin HTTP tour orchestrated leaf (nav surfaces, not React) | `src/afw/tests/advanced/admin-http-tour/` | Gate leaf: bootstrap + Server/Extensions/Services/Adapters/Logs + Objects + Functions + Fiddle. Not nginx `/docs` HTML. |
| More Mike mantras | mantras pad | When shared |
| MEMORY env/runtime novel fully thinned | Keep pointer; git pads win | Done enough; 2026-08-16 harvest added picture + Fiddle/stack playbooks |
| Per-extension support cards | agent-support or extensions | On demand |
| beta-backlog refresh | root hubs | Campaign hygiene, not atlas |

---

## MEMORY hygiene (this pass)

| MEMORY content | Action |
|----------------|--------|
| Sticky mission, mantras pointer, hubs | **Keep** (thin) |
| Quick resume branch tip | **Refresh** when branch/PR changes — not durable map |
| #158 graceful stop section | **Promoted** → hosts playbook + this atlas §6; MEMORY may shrink to pointer |
| afwdev testing split | **Already** in designs; MEMORY may shrink to pointer |
| Adaptive environment ↔ runtime objects | **Promoted** condensed → atlas §5 + env playbook; architecture pad remains deep |
| #149 closed narrative | **Pointer** only — pads keep detail |
| Workspace prefs (plain language, handbook, EBNF, docs chrome) | **Keep** in workspace MEMORY — operational |

---

## Maintenance

After a deep issue session:

1. Update the **topic row** (open/residual, probe) if the map changed.  
2. Prefer **one settled sentence** over pasting the PR.  
3. Fill or add an **agent-support** playbook only when symptoms repeat.  
4. Do **not** grow this file into a dump of every PR.

**First pass date:** 2026-08-09 · branch work on `feature-afw-support-agent`.  
**Harvest pass:** 2026-08-16 — session interval notes (`/root/.grok/memory/…/sessions/`) walked into this atlas + `agent-support` (picture, live stack, actions/Fiddle, language leftover rows). Session files stay local; git is what other partners see.
