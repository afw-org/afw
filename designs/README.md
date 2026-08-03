# designs/ — maintainer design pads

**Audience:** maintainers and AI assistants. **Not** published handbook or end-user docs.

## Purpose

Per-**issue** or per-**theme** working notes: why, options, footguns, parked ideas, branch status. This is the repo home for the kind of material that used to live in a personal `dev/` folder — durable enough for git, not polished product docs.

| Put here | Put elsewhere |
|----------|----------------|
| Deep design for one issue/theme (`memory-management`, crypto, decompile status, …) | Always-on agent glue → `.cursor/rules/`, `AGENTS.md` |
| Parked features with enough detail to resume later | Multi-theme “don’t forget” list → `beta-backlog.md` (root) |
| In-flight branch status snapshots | User-facing changes → `whats-new.md` (root, while on `mgg-develop`) |
| | Real developer docs → `src/afw/doc/` (and extension `doc/`) |

## Current pads (non-exhaustive)

| File | Theme |
|------|--------|
| [`memory-management.md`](memory-management.md) | Umbrella **#2** — pools, value lifetimes, escape |
| [`secrets-and-afw-crypto.md`](secrets-and-afw-crypto.md) | **#74** / `afw_crypto` design |
| [`issue-18-decompile-status.md`](issue-18-decompile-status.md) | **#18** decompile/recompile status |
| [`issue-38-expression-property-names.md`](issue-38-expression-property-names.md) | **#38** — `[expr]: value` in object values |
| [`issue-138-meta-on-the-wire.md`](issue-138-meta-on-the-wire.md) | **#138** — `"_meta_"` wire / options / rich types |
| [`compile-optimize-notes.md`](compile-optimize-notes.md) | Future compile-time optimize; #140 Pattern/param/catch notes (mostly landed) + binding sites |
| [`issue-28-type-syntax.md`](issue-28-type-syntax.md) | **#28** script type syntax + opt-in checking (implemented on branch) |
| [`adaptive-function-compile-typecheck.md`](adaptive-function-compile-typecheck.md) | Follow-on: compile typeCheck for adaptive/built-in function calls (design) |
| [`pragma-hash-design.md`](pragma-hash-design.md) | **`#` design** — pragma vs compiler-internal; Pattern B `#compile` |
| [`decompile-compiler-internal-inventory.md`](decompile-compiler-internal-inventory.md) | Decompile emit vs compiler-internal accept checklist + tests |

## Conventions

- One topic per file (or a small tightly related set).
- Link from `beta-backlog.md` or GitHub when the work is active.
- When a decision stabilizes, **promote** invariants into rules, code comments, or real docs — then thin the pad.
- Do **not** hand-edit `generated/` from notes here; implement via normal generate/build paths.

### C cleanup (non-pool)

For OpenSSL handles, key material, `FILE *`, etc.: **NULL init + `AFW_TRY` + free in `AFW_FINALLY`**. Documented in `.cursor/rules/afw-c-runtime.mdc`; crypto execute path is the reference.

Root stays thin: hubs (`AGENTS.md`, `beta-backlog.md`, `whats-new.md`) + public project files. Stuff design detail **here**, not more long pads at repository root.
