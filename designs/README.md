# designs/ — maintainer design pads

**Audience:** maintainers and AI assistants. **Not** published handbook or end-user docs.

## Purpose

Per-**issue** or per-**theme** working notes: why, options, footguns, parked ideas, branch status. This is the repo home for the kind of material that used to live in a personal `dev/` folder — durable enough for git, not polished product docs.

| Put here | Put elsewhere |
|----------|----------------|
| Deep design for one issue/theme (`memory-management`, crypto, decompile status, …) | Always-on agent glue → `.cursor/rules/`, `AGENTS.md` |
| Parked features with enough detail to resume later | Multi-theme “don’t forget” list → `beta-backlog.md` (root) |
| In-flight branch status snapshots | User-facing changes → `whats_new.md` (root, while on `mgg-develop`) |
| | Real developer docs → `src/afw/doc/` (and extension `doc/`) |

## Current pads (non-exhaustive)

| File | Theme |
|------|--------|
| [`memory-management.md`](memory-management.md) | Umbrella **#2** — pools, value lifetimes, escape |
| [`secrets-and-afw-crypto.md`](secrets-and-afw-crypto.md) | **#74** / `afw_crypto` design |
| [`issue-18-decompile-status.md`](issue-18-decompile-status.md) | **#18** decompile/recompile status |
| [`compile-optimize-notes.md`](compile-optimize-notes.md) | Future compile-time optimize (not #18) |

## Conventions

- One topic per file (or a small tightly related set).
- Link from `beta-backlog.md` or GitHub when the work is active.
- When a decision stabilizes, **promote** invariants into rules, code comments, or real docs — then thin the pad.
- Do **not** hand-edit `generated/` from notes here; implement via normal generate/build paths.

### C cleanup (non-pool)

For OpenSSL handles, key material, `FILE *`, etc.: **NULL init + `AFW_TRY` + free in `AFW_FINALLY`**. Documented in `.cursor/rules/afw-c-runtime.mdc`; crypto execute path is the reference.

Root stays thin: hubs (`AGENTS.md`, `beta-backlog.md`, `whats_new.md`) + public project files. Stuff design detail **here**, not more long pads at repository root.
