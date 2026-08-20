# designs/ — maintainer design pads

**Audience:** maintainers and AI assistants. **Not** published handbook or end-user docs.

## Purpose

Per-**issue** or per-**theme** working notes: why, options, footguns, parked ideas, branch status. This is the repo home for the kind of material that used to live in a personal `dev/` folder — durable enough for git, not polished product docs.

| Put here | Put elsewhere |
|----------|----------------|
| Deep design for one issue/theme (`memory-management`, crypto, decompile status, …) | Always-on agent glue → `.cursor/rules/`, `AGENTS.md` |
| Parked features with enough detail to resume later | Multi-theme “don’t forget” list → `beta-backlog.md` (root) |
| In-flight branch status snapshots | User-facing changes → `whats-new.md` (root, on `develop`) |
| | Adaptive Script vs TS/JS — beta decision notes → [`typescript-differences.md`](../typescript-differences.md) (root; not Jeremy’s #22 author doc) |
| | Real developer docs → `src/afw/doc/` (and extension `doc/`) |

## Current pads (non-exhaustive)

| File | Theme |
|------|--------|
| [`ai-partner-lessons.md`](ai-partner-lessons.md) | **Optional** team lessons — pairing with an AI partner on issues/support (follow or not) |
| [`knowledge-atlas.md`](knowledge-atlas.md) | **Topic atlas** — area → rules/pads/probes/gaps; **how the C surface fits**; admin/Fiddle §16. Start here for “where is X?” |
| [`afw-philosophy-and-core-model.md`](afw-philosophy-and-core-model.md) | **Whole story** — why AFW is shaped this way, how parts relate (values, compile, pools, env, hosts). Keep and correct; do not trim maps. Not always-on rules |
| [`mantras-and-working-style.md`](mantras-and-working-style.md) | Mike mantras + partnership habits (subset; grow when shared); philosophy/taste questions — **not** always-on |
| [`memory-management.md`](memory-management.md) | Umbrella **#2** — pools, value lifetimes, escape |
| [`issue-2-property-name-values.md`](issue-2-property-name-values.md) | **#2** — object property names as `const afw_value_t *` (this branch; script/JSON string-only) |
| [`c-naming-and-payloads.md`](c-naming-and-payloads.md) | Value vs utf8/memory payloads; `create` / `set` / `no_copy` / `forced_safe`; code_point dir |
| [`runtime-objects-and-environment.md`](runtime-objects-and-environment.md) | **#149 closed** (PRs #160–#162) — architecture map: generate maps, OT `runtime`, accessors, env registration, checklist; keep for **#2** follow-on |
| [`runtime-value-accessors.md`](runtime-value-accessors.md) | Live catalog snapshot of `_AdaptiveRuntimeValueAccessor_` (refresh via `afw -x` retrieve) |
| [`afwdev-test-recipe.md`](afwdev-test-recipe.md) | Gate vs lab: `test -j`, `-T` / `tests-extra`, firehose, `--output`, valgrind notes |
| [`c-probes.md`](c-probes.md) | **#207** — `run_c_probe()`; `prime-test-c-probe`; valgrind / libunwind |
| [`runtime-catalog-lifetime.md`](runtime-catalog-lifetime.md) | **#149** discovery notes — live maps vs materialize cost (architecture pad is preferred map) |
| [`secrets-and-afw-crypto.md`](secrets-and-afw-crypto.md) | **#74** / `afw_crypto` design |
| [`issue-18-decompile-status.md`](issue-18-decompile-status.md) | **#18** decompile/recompile (stringify issue closed; pad still useful) |
| [`issue-38-expression-property-names.md`](issue-38-expression-property-names.md) | **#38** — `[expr]: value` (issue closed) |
| [`issue-138-meta-on-the-wire.md`](issue-138-meta-on-the-wire.md) | **#138** — `"_meta_"` wire / options / rich types |
| [`compile-optimize-notes.md`](compile-optimize-notes.md) | Future compile-time optimize; #140 Pattern/param/catch (landed) + binding-site inventory |
| [`issue-28-type-syntax.md`](issue-28-type-syntax.md) | **#28** script types + checking — **closed** (PRs **#144**/**#145** core; **#171** wrap-up) |
| [`adaptive-function-compile-typecheck.md`](adaptive-function-compile-typecheck.md) | Adaptive/built-in call formals under typeCheck — **shipped** with #28 |
| [`pragma-hash-design.md`](pragma-hash-design.md) | **`#` design** — pragma vs compiler-internal; Pattern B `#compile` (landed with #28 line) |
| [`decompile-compiler-internal-inventory.md`](decompile-compiler-internal-inventory.md) | Decompile emit vs `#…` accept checklist + tests |
| [`compile-contextual-audit.md`](compile-contextual-audit.md) | call_create / type-check contextual inventory (NULL policy) |
| [`issue-17-mutable-object-faces.md`](issue-17-mutable-object-faces.md) | **#17** — mutable faces (landed PR **#150** → `mgg-develop`; issue closed) |
| [`array-semantics.md`](array-semantics.md) | **#39** — dense arrays, elision, create_array, out-of-range bracket get |
| [`conversion-functions.md`](conversion-functions.md) | Type-named converts vs constructors; no null()/function() convert |
| [`utf8-code-point-sequences.md`](utf8-code-point-sequences.md) | **#153** — utf8-backed values as immutable code-point sequences (index, for-of, array consumers) |
| [`afwdev-advanced-test.md`](afwdev-advanced-test.md) | **#157 closed** — orchestrated tests history pad; live schema in `src/afw/tests-extra/` (PR **#167**) |
| [`afwdev-blast.md`](afwdev-blast.md) | **Retired** `afwdev blast` → `schedule.firehose` + `-T tests-extra/`; **#13** still open for Jeremy knobs |
| [`agent-support.md`](agent-support.md) | Ongoing support/dev partner — playbook stubs, capture checklist; hub is `AGENTS.md` |
| (no pad) | **#158** graceful process stop — landed (PR #165); user note in `whats-new.md` |
| [`issue-127-progressive-retrieve-release.md`](issue-127-progressive-retrieve-release.md) | **#127** — progressive retrieve write-then-release (to_stream / to_response / HTTP list); script callback residual |
| [`issue-33-error-codes.md`](issue-33-error-codes.md) | **#33** — review error codes / HTTP map / script `e.id` (closed; PR **#173** → `mgg-develop`) |
| [`issue-62-script-language.md`](issue-62-script-language.md) | **#62 closed** (PR **#174**) — Adaptive Script language index (multi `let`/`const`, `for` init, assignment chain, result value, labels) |
| [`libafw-headers-and-api-surface.md`](libafw-headers-and-api-surface.md) | libafw header layers (`afw.h` / internal / minimal / common), install public-only goal, Doxygen public vs internal |
| [`lineage-and-library-floor.md`](lineage-and-library-floor.md) | Base vs private packages; Docker ICU/APR floor; ICU in `utf8/` + `code_point/` |
| [`sibling-afw-package.md`](sibling-afw-package.md) | Another Git repo next to this base: layout, write wall, sandbox, prime kit |

## Conventions

- One topic per file (or a small tightly related set).
- Link from `beta-backlog.md` or GitHub when the work is active.
- When a decision stabilizes, **promote** invariants into rules, code comments, or real docs — then thin the pad.
- Do **not** hand-edit `generated/` from notes here; implement via normal generate/build paths.

### C cleanup (non-pool)

For OpenSSL handles, key material, `FILE *`, etc.: **NULL init + `AFW_TRY` + free in `AFW_FINALLY`**. Documented in `.cursor/rules/afw-c-runtime.mdc`; crypto execute path is the reference.

Root stays thin: hubs (`AGENTS.md`, `beta-backlog.md`, `whats-new.md`) + public project files. Stuff design detail **here**, not more long pads at repository root.
