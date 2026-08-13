# AFW knowledge atlas (first pass — 2026-08)

**Audience:** maintainers and AI assistants in the ongoing support + development partner role.  
**Not** handbook, not always-on rules, not a second copy of every pad.

**Job of this file:** topic → best sources → live probe → open/residual → gaps.  
**Ground truth:** code + tests. When this atlas drifts, fix the atlas (or the pad it points at).

**How to use**

1. Land on a **topic** below.  
2. Open **day rules** for how to work; **deep pad** for why/history; **probe** to verify.  
3. For philosophy/taste: [`mantras-and-working-style.md`](mantras-and-working-style.md), [`afw-philosophy-and-core-model.md`](afw-philosophy-and-core-model.md).  
4. For symptom-shaped support: [`agent-support.md`](agent-support.md).

**Hubs:** [`AGENTS.md`](../AGENTS.md) · [`designs/README.md`](README.md) · [`beta-backlog.md`](../beta-backlog.md) · [`whats-new.md`](../whats-new.md)

---

## Layer cheat sheet (where truth lives)

| Layer | Role | Do not use it for |
|-------|------|-------------------|
| Code + tests | Ground truth | — |
| `.cursor/rules/*.mdc` | How to *work* in an area | Long history diaries |
| `AGENTS.md` | System map, mission, build loop | Deep design archaeology |
| `designs/*` | Why, options, open questions, maps | Always-on law |
| `whats-new.md` | User/operator-facing on `mgg-develop` | Internal implementation plans |
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

---

## 0. Orientation / product taste

| Field | Content |
|-------|---------|
| **Settled map** | Ongoing partner role; beta = quality bar; hard choices by **consensus** (ask what you think → discuss → live verify → either side can be wrong) |
| **Day rules** | `afw-project` (role one-liner, hard rules, terminology, plain language) |
| **Deep / reference** | [`ai-partner-lessons.md`](ai-partner-lessons.md) (optional team one-pager), mantras (*How consensus is grown*), philosophy pad, agent-support |
| **Probe** | Live system when debating models (`afwfcgi`, env registries) — not CI |
| **Open** | More mantras as shared over time |
| **Gap** | None critical; grow mantras pad, don’t invent phrases |
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
| **Settled map** | Values first; permanent / managed / managed_slice / unmanaged; hierarchical pools; short request teardown proven; long-running needs escape discipline |
| **Day rules** | `afw-runtime-model` (always-on), `afw-value-memory`, `afw-script-eval` |
| **Deep pad** | [`memory-management.md`](memory-management.md) (**large** — do not rewrite this pass); philosophy pad core model |
| **Probe** | Targeted `.as` + `afwdev test -j --env-mode valgrind`; orchestrated multi-request leaves; never “fix memory” without a metric/story |
| **Open** | Umbrella **#2** — phased partner workflow in memory pad; parent of closed #149 |
| **Gap** | Thin “support one-pager” for leaks vs the novel-length pad — optional later; playbook points at pad |

---

## 4. Compile, EBNF, eval, functions

| Field | Content |
|-------|---------|
| **Settled map** | Compile → value graph → evaluate; EBNF-in-comments harvest; functions metadata + hand execute; statement_flow control |
| **Day rules** | `afw-compile`, `afw-compiler-ebnf`, `afw-script-eval`, `afw-function` |
| **Deep pads** | `compile-optimize-notes`, `pragma-hash-design`, `decompile-compiler-internal-inventory`, `compile-contextual-audit`, `adaptive-function-compile-typecheck`, `issue-28-type-syntax` |
| **Probe** | `afw -s '…'` / tests under `src/afw/tests/`; regenerate EBNF via `--cdev`; check `generated/ebnf/syntax.ebnf` has `::=` for nonterminals |
| **Open** | Compile optimize future (`compile-optimize-notes`); app-shared / script poly (**#170**); #28 **closed** (PR **#171**) — map: `issue-28-type-syntax.md` + `typescript-differences.md` + handbook Types (git archive; not session memory) |
| **Gap** | Support playbook for “parse error / decompile mismatch” still thin — use rules + decompile pads |

---

## 5. Environment & runtime objects

| Field | Content |
|-------|---------|
| **Settled map** | Process-wide registries; everything registered discoverable on `adapterId=afw`; runtime objects = immutable views; #149 accessor reliability shipped |
| **Day rules** | `afw-environment`, `afw-environment-variables`, `afw-core-services` (runtime section) |
| **Deep pads** | [`runtime-objects-and-environment.md`](runtime-objects-and-environment.md) (architecture), [`runtime-value-accessors.md`](runtime-value-accessors.md) (catalog snapshot), [`runtime-catalog-lifetime.md`](runtime-catalog-lifetime.md) (discovery notes) |
| **Probe** | `retrieve_objects` / GET `/afw/_Adaptive…_/`; `/afw/_AdaptiveRuntimeValueAccessor_/`; **do not** stop permanent `adapter-afw` / `adapter-conf`; prefer typed retrieve over full `current` materialize |
| **Open** | Residuals under **#2**; more adapters may need terminating checks; attach lifecycle for orchestrated leaves not fully built |
| **Gap** | MEMORY held a long durable env section — **promoted into playbook** this pass; keep architecture pad as deep map, not MEMORY novel |

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
| **Deep pad** | None required; user notes in `whats-new.md` |
| **Probe** | `src/afw/tests/advanced/afwfcgi_signal_shutdown/`; `afwfcgi --help` |
| **Open** | Drain timeout, SIGHUP, Windows service, general signal framework, every extension retrieve loop, `--local` read unblock |
| **Gap** | MEMORY #158 section → **promoted into playbook** this pass |

**Host contracts**

| Host | SIGTERM/SIGINT |
|------|----------------|
| **`afwfcgi`** | `env->terminating`; FCGI shutdown pending; **SIGUSR1** each request thread; join; close listen; **unlink Unix `-p` path** (not TCP) |
| **`afw`** | Set `env->terminating` only |

**Core:** error code `terminating` → HTTP **503**; `AFW_XCTX_THROW_IF_TERMINATING(xctx)` at work-unit boundaries.

**Wrong path:** close listen fd alone to wake multi-thread accept while main is in join — use **thread signal** for libfcgi.

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
| **Gap** | “How to write a good .as test” → `afw-tests` + writing-tests developer doc |

---

## 11. afwdev: build / generate / test

| Field | Content |
|-------|---------|
| **Settled map** | **Gate** vs **lab**; **orchestrated** leaves (`orchestration.yaml`); blast **retired** (PR **#167**) |
| **Day rules** | `afw-tests`, `afw-afwdev-python`, `afw-afwdev-generate` |
| **Deep pads** | [`afwdev-test-recipe.md`](afwdev-test-recipe.md), [`afwdev-advanced-test.md`](afwdev-advanced-test.md) (history), [`afwdev-blast.md`](afwdev-blast.md) (retired); `src/afw/tests-extra/{README,SCHEMA}.md`; developer `writing-tests.md` |
| **Probe** | See recipe commands below |
| **Open** | #13 stress knobs/stats on `test` (Jeremy); attach mode not fully built; #157 residuals if any |
| **Gap** | MEMORY testing split → **in recipe + playbook**; drop duplicating full MEMORY novel after promote |

**Intent split**

| Surface | Job | Default gate? |
|---------|-----|----------------|
| `afwdev test -j` | Language/package suite + short orchestrated leaves under `src/*/tests/` | **Yes** |
| `afwdev test -T path` | Opt-in only (`tests-extra/`, etc.) | Opt-in |
| `afwdev test --env-mode afwfcgi` | Conf-free `.as` on live `:8080/afw` (skips hermetic orchestrated leaves) | Optional |
| `afwdev test --env-mode valgrind -j` | Memory check; heavy — fewer jobs often faster wall-clock | Optional full verify |
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
| UTF-8 code points | `utf8-code-point-sequences.md` | #153 oriented |
| Mutable faces | `issue-17-mutable-object-faces.md` | Closed PR #150 |
| Expression property names | `issue-38-…` | Closed |
| Meta on wire | `issue-138-…` | Design/status in pad |
| Decompile | `issue-18-…` | Pad useful; stringify closed |
| Types | `issue-28-…` | **#28 closed** (PR **#171** wrap-up) |
| Pragma `#` | `pragma-hash-design.md` | Pattern B `#compile` (landed) |
| Error codes / HTTP / `e.id` | `issue-33-error-codes.md` | **#33** closed (PR **#173**) — map in `afw_common.h`; script `throw` may set `id`; prefer `e.id` |
| Script language syntax | `issue-62-script-language.md` | **#62** — multi `let`/`const`, `for` init, assignment chain landed; result value, labels next |

---

## Cross-cutting mantras (see mantras pad)

Most used when navigating this atlas:

- **Get it right at most once** — metadata / shared accessor / shared helper  
- **Values first** — then pools  
- **Maps over tickets** — issue ids are pointers  
- **Gate vs lab** — don’t redefine `test -j` as soak  
- **Live verify** when teaching  
- **Code wins** when notes drift  

---

## Gaps backlog (first-pass harvest)

Items worth future promote/fill — **not** blocking this atlas:

| Gap | Suggested home | Priority |
|-----|----------------|----------|
| Leak / long-run “first 15 minutes” support card | agent-support or thin pointer into memory pad | Medium (#2 active) |
| Parse/decompile mismatch playbook | agent-support | Low until pain |
| Adapter write / conf service lifecycle card | agent-support (partially in recipe) | Medium |
| Attach mode orchestrated leaves | afwdev-advanced-test pad / SCHEMA | When building |
| More Mike mantras | mantras pad | When shared |
| MEMORY env/runtime novel fully thinned | Keep pointer; git pads win | Done enough this pass |
| Per-extension support cards | agent-support or extensions | On demand |
| open-issues-status / beta-backlog refresh | root hubs | Campaign hygiene, not atlas |

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
