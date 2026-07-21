# AFW beta backlog (brain dump)

**Audience:** maintainers and assistants working toward a beta-quality tree.  
**Not for end users.** User-facing changes while work is on `mgg-develop` go in [`whats_new.md`](whats_new.md).

## Purpose

Dump **details, design thoughts, unfinished plans, and “don’t forget” items** out of maintainers’ heads so they survive chat sessions and months of beta work.

| Document | Role |
|----------|------|
| **`beta-backlog.md`** (this file) | Working notes, plans, archaeology, half-decided design. Source of truth for “what we still need to remember.” |
| **`whats_new.md`** | What **users** of AFW need to know about **`mgg-develop`** (behavior, APIs, migration). |
| **GitHub issues** | Optional promotion when something needs discussion, an assignee, PR linkage, or is a real beta blocker. Prefer thematic umbrellas (e.g. language, memory) over one infinite meta-issue. |
| **`issues.md`** | Temporary triage experiment; **not** the long-term backlog. Prefer this file. |

## Branch plan (as of mid‑2026)

```text
mgg-develop  →  (when ready) develop  →  (cleaned up, ≥ beta) main
```

- Day-to-day work and user-facing “what’s new” live on **`mgg-develop`**.
- Merge **`mgg-develop` → `develop`** when that line is ready to become the shared integration branch again.
- Promote **`develop` → `main`** when the tree is cleaned up and at least **beta-ready**.

Update this section if the plan changes.

## How to use this file

1. **Dump freely** — incomplete thoughts are fine; date or initial a chunk if helpful.
2. **Tag status** loosely: `idea` · `planned` · `partial` · `blocker` · `done` · `wontfix`.
3. **Link** GitHub issues, paths, and FIXMEs when known; don’t invent issue numbers.
4. **When something ships** — mark done here (or move to a short Done section) and, if users care, update `whats_new.md`.
5. **Graduate** to a GitHub issue only when the item is big enough, blocking, or needs external tracking.

## Documentation preference (later)

**Status:** planned direction — not a near-term rewrite

- Prefer **developer knowledge in the code** (comments, module headers, EBNF-in-comments, existing guide XML where it already lives) over a growing pile of external markdown that drifts.
- **`beta-backlog.md`** is a temporary/maintainer **brain dump and beta working list**. Fine for months of beta work; not the long-term home for architecture prose.
- When we improve developer documentation later, **mine this file** (and chat archaeology, FIXMEs, `whats_new.md` where relevant) and **fold durable facts into code comments / in-tree developer docs**, then thin the backlog.
- User-facing material stays separate (`whats_new.md` while on `mgg-develop`, published handbook/docs as appropriate).

## How we work with assistants (session hygiene)

- **Default: new conversation per distinct issue** — keeps context size down; tools re-read code and this file as needed.
- **Reopen an old chat** when that thread’s history is intentionally wanted in context.
- **Durable knowledge** goes here, in **rules / `AGENTS.md` / code comments**, or git — not only in chat.
- Ask the assistant to **update rules / this backlog** when something will matter in future sessions.
- Next real work: expect a **new feature branch** off current integration line (not pile everything only on long-lived chat state).
- **`issues.md`**: temporary Cursor triage dump; do not treat as canonical. Prefer this file + GitHub.
- **Build before commit:** day-to-day C/Python can use `./afwdev build --cdev -j`. Before commit/push on docs, multi-area, or finish-pass work, prefer **`./afwdev build --all --install --scan -j`** (docs-only exists; full `--all` is not much longer and catches more). PR gate still adds `--clean` + valgrind for full verify.

### Session wrap-up — 2026-07-20

- Explored **#54** (indexes / deprecated variables); did **not** implement — notes under Indexes below.
- Created **`beta-backlog.md`** on **`mgg-develop`** (initial commit `9bfefbf7`; follow-up commits for hygiene/wrap-up notes).
- No feature implementation this session; partnership agreement: keep this file together over months toward beta.

---

## Notes dump

_Add new sections or bullets under the themes below. Newest thoughts can go at the top of a section or under “Inbox”._

### Inbox (unsorted)

_(Paste raw notes here first; sort into themes later.)_

### Indexes / adapters (incl. issue #54)

**Status:** core `current::` eval on branch `Issue-#54` / PR **#130** (see also rule **`afw-adapter-index`**). Partial product story — LMDB create path still broken.  
**Code:** `src/afw/adapter/afw_adapter_impl_index.c`, LMDB `afw_lmdb_index*`, session hooks; Adaptive `index_*` are **core**, not LMDB-only functions.

#### How indexes actually work

- **Purpose:** secondary keys so sargable `retrieve_objects` can avoid full dump (`Index#objectType#key` DBs: value → object id).
- **Only LMDB** implements `get_index_interface` today.
- **Definitions** live **in the LMDB file** (`internalConfig.indexDefinitions` @ Primary UUID 0), managed by **`index_create` / list / remove** — not adapter conf `env`/`limits`.
- **Filter/value** are script **source strings** on those definitions; core still **compiles each try** (`compile_type_script` + evaluate).
- **Without indexes:** LMDB CRUD/journal still work (existing tests). Empty definitions → index walk no-op.

#### #54 (this work)

- **`index_try`** pushes **`current::object` / `objectId` / `objectType` / `key`** for filter/value (auth-style `push_cb_variables` + stack restore).
- Old ambient unqualified `object` push was already dead; we did not remove a live shim.
- Migration: bare `object` / `variable_get("object")` for *index* context → `current::object` / `variable_get("current::object")` (lexical `object` still works in normal scripts).
- No conf **`custom`** for LMDB indexes (custom = maintainer bags where compiled units run, e.g. model).
- Smoke `src/afw_lmdb/tests/adapter/index_current.as` **skipped**.
- `whats_new.md`: honest **partial** note only.

#### Still to do (pre-existing; @Jeremy on #54)

- Session indexer **`txn == NULL`** → save_config **EINVAL** on create.
- Retroactive create + write txn **hang**.
- Unskip e2e / full **#57** tests after create works.
- Compile-once filter/value (hygiene).

#### `current::` vs `custom::`

| Qualifier | Role |
|-----------|------|
| **`current::`** | Framework operation context; **names per context** (auth ≠ model ≠ index). |
| **`custom::`** | Conf author extras for scripts in that thing; compile at load when hybrids. |

#### Authorship (import ≠ git blame)

- Indexes/LMDB/curl/admin/docs/parts of afwdev: **Jeremy**. Core runtime: **mgg** et al. Repo import attributes many files to first committer in this remote.

Durable agent rule: [`.cursor/rules/afw-adapter-index.mdc`](.cursor/rules/afw-adapter-index.mdc).

### Adaptive syntax: expressions → scripts → multi-syntax

**Status:** design context (ongoing)

- Originally AFW only had **Adaptive expressions**. **Adaptive Script** came later.
- Many old “expression slots” can now hold **scripts** (and other compile types). Hosting context still matters:
  - **Expression-like slots** (index filter/value, hybrids, model property expressions, log filter/format, etc.): script **must return a value** (same contract as an expression).
  - **Standalone scripts:** return not required.
  - **CLI / command-style** (including executable + shebang): special case — result should be what you’d expect from running a **command**.
  - Shebang examples in tests: `#!/usr/bin/env afw`, `#!/usr/bin/env -S afw --syntax test_script`.
- Compiler supports **multiple syntaxes** (`script`, `template`, JSON / relaxed JSON, `test_script`, …). Same places can host different syntaxes depending on compile type / CLI / shebang.

### Compiler / values: not always `compiled_value`

**Status:** design context

- Compile returns an **`afw_value_t *`**. Often a `compiled_value`, but **not always**.
- When the answer is known at compile time, the compiler may return a **fully evaluated** data-type value (no `optional_evaluate`) to avoid overhead — e.g. JSON compile type, literals, `#{…}` compile-time substitution result embedded in the tree.
- Callers should use **`afw_value_evaluate`**: if no `optional_evaluate`, the value is already the result. Most code should not care which inf produced it.

### Planned: pure function constant-fold at compile time

**Status:** idea / planned (no dedicated GitHub issue found)

- If a **pure** function is called and **all parameters are known at compile time**, evaluate at compile time and use the **result** instead of a runtime call node.
- Metadata already has **`pure`** (and `sideEffects`) on function generate objects.
- Hooks / FIXMEs: `allow_optimize`, `optimized_value`, `/** @fixme add optimization. */` on call / related value kinds; `compile_noOptimize` flag as escape hatch idea.
- Today “optimize” paths are narrower (e.g. polymorphic built-in specialization) — **not** full pure constant-fold.
- Related but different issues: **#28** compile-time types; **#97** `#{…}`; **#101** evaluate / unevaluated handling.

### Language / script syntax (misc)

**Status:** planned dump area

- Expect many small items (finish/add syntax, etc.). Prefer notes here; graduate to GitHub (or extend umbrella **#62** Adaptive Script language changes) when actively implementing.
- _(Add bullets as they leave your head.)_

### Runtime / memory / long-running

**Status:** pointer / **beta-relevant**

- Umbrella **#2** (memory). Related: retrieve caps **#49**, progressive release **#127**, value lifetime rules in project docs / `.cursor/rules/afw-value-memory.mdc`.
- **Qualifier snapshots (issue #9)** — `qualifier()` / `qualifiers()` allocate **fresh memory objects** and can get **very large** (`environment::`, `request::`, nested `qualifiers()` over every active qualifier, multi-entry contribute). Documented as debug/tools/not hot path + size warning in function metadata, language XML, `whats_new.md`.
  - Another reason **memory management / managed release / long-running escape** needs to be solid **before calling the tree beta**: scripts that snapshot often (or hold results) will stress pools and lifetimes harder than `qualifier::name` get.
  - Do **not** treat #9 as “done for beta” solely because the API exists; couple with #2 progress and real long-running exercise if tools use snapshots heavily.
- Prefer everyday **`qualifier::name`**; snapshots only when listing/debug is intentional.

### Qualified variables / issue #9 (finish notes)

**Status:** partial (API on `Issue-#9` / PR #129; memory/beta still open)

- Shipped direction: multi-entry contribute (most recent wins per property), nullish when no visible entry, `qualifiers()` omits inactive names, `includeUntrusted` = less-secure view while secure.
- Still deferred by design (not blocking the snapshot API itself):
  - **custom::** multi-layer contribute redesign (risk of residual names under property-level `on*`).
  - Secure-mode fixture for `includeUntrusted` (needs secure xctx entry in tests).
  - Isolation / valgrind battery as a dedicated pass.
- Handbook: use supported doc XML tags only (`literal`, `italic`, `strong`, …) — not DocBook `<emphasis>` (afwdev docs build logs `Unknown element`). Cursor: **`.cursor/rules/afw-qualified-variables.mdc`**, pointers in adaptive-script / value-memory / model-adapter / afwdev-python.

### Beta gate (checklist sketch)

_Not a commitment — fill in as “must be true before we call it beta.”_

- [ ] Memory / long-running story credible (**#2**): managed values, pools, no silent leak under realistic server/script load
- [ ] Large materializations constrained or progressive where needed (**#49**, **#127**, client progressive path)
- [ ] Snapshot / debug APIs (e.g. **#9** `qualifier`/`qualifiers`) documented as non-hot-path and size-aware; not used as everyday data access
- [ ] User-facing behavior documented in `whats_new.md` / real docs as appropriate
- [ ] `mgg-develop` merged to `develop` when ready; `develop` → `main` when beta-ready

---

## Done (archive short notes)

| When | Item |
|------|------|
| | |

---

## Changelog of this file

| Date | Note |
|------|------|
| 2026-07-20 | Created; seeded from index/#54 discussion, expression vs script context, compile/value model, pure-fold plan, branch plan, doc roles. |
| 2026-07-20 | Doc preference: long-term developer knowledge in code; this file is dump/source for later fold-in. |
| 2026-07-20 | Session hygiene; wrap-up note for #54 explore-only session. |
| 2026-07-20 | #54: adapter/model `custom::` vs index `current::`; model context as reference for few current vars. |
| 2026-07-20 | `custom::` = conf maintainer’s extra vars for exprs/templates/scripts in that thing. |
| 2026-07-20 | custom vars compiled at conf read; evaluate at use (not recompile each time). |
| 2026-07-20 | #54 current:: surface: object, objectId, objectType, key. |
| 2026-07-20 | #54 implemented on Issue-#54 (uncommitted): index_try current:: push + docs/test. |
| 2026-07-21 | #54 docs: soft whats_new; afw-adapter-index rule; AGENTS/extensions/qualified-vars; how indexes work. |
