# AFW beta backlog (brain dump)

**Audience:** maintainers and assistants working toward a beta-quality tree.  
**Not for end users.** User-facing changes while work is on `mgg-develop` go in [`whats-new.md`](whats-new.md).

## Purpose

Dump **details, design thoughts, unfinished plans, and “don’t forget” items** out of maintainers’ heads so they survive chat sessions and months of beta work.

| Document | Role |
|----------|------|
| **`beta-backlog.md`** (this file) | Working notes, plans, archaeology, half-decided design. Source of truth for “what we still need to remember.” |
| **`designs/`** | Per-issue / per-theme design pads (not user docs). See [`designs/README.md`](designs/README.md). |
| **`designs/memory-management.md`** | Living design notes for umbrella **#2** (long-running memory, value/object identity, escape). |
| **`whats-new.md`** | What **users** of AFW need to know about **`mgg-develop`** (behavior, APIs, migration). |
| **GitHub issues** | Optional promotion when something needs discussion, an assignee, PR linkage, or is a real beta blocker. Prefer thematic umbrellas (e.g. language, memory) over one infinite meta-issue. |

## Branch plan (as of mid‑2026)

**Steady state (normal AFW practice):**

```text
feature branches  →  develop  →  (cleaned up, ≥ beta) main
```

- **`develop`** is the usual shared integration branch.
- Day-to-day work is **feature branches off `develop`**.

**Current exception — concentrated AI / Grok Build pass:**

```text
feature branches  →  mgg-develop  →  (user testing) develop  →  main
```

- Volume of change from Grok Build / Cursor would overwhelm the usual `develop` cadence, so **`mgg-develop`** was cut off `develop` as a long-lived staging line.
- Feature branches (e.g. `issue-#1`) are cut **off `mgg-develop`**, in chunks.
- User-facing “what’s new” while on this line: **`whats-new.md`**.
- After this large pass: **major user testing**, then merge **`mgg-develop` → `develop`**. Then **`develop` is again the main develop branch**.
- Over weeks/months, most work should return to feature branches off `develop` (mgg-develop is not the permanent forever trunk).

Update this section if the plan changes.

## How to use this file

1. **Dump freely** — incomplete thoughts are fine; date or initial a chunk if helpful.
2. **Tag status** loosely: `idea` · `planned` · `partial` · `blocker` · `done` · `wontfix`.
3. **Link** GitHub issues, paths, and FIXMEs when known; don’t invent issue numbers.
4. **When something ships** — mark done here (or move to a short Done section) and, if users care, update `whats-new.md`.
5. **Graduate** to a GitHub issue only when the item is big enough, blocking, or needs external tracking.

## Documentation preference (later)

**Status:** planned direction — not a near-term rewrite

- Prefer **developer knowledge in the code** (comments, module headers, EBNF-in-comments, existing guide XML where it already lives) over a growing pile of external markdown that drifts.
- **`beta-backlog.md`** is a temporary/maintainer **brain dump and beta working list**. Fine for months of beta work; not the long-term home for architecture prose.
- When we improve developer documentation later, **mine this file** (and chat archaeology, FIXMEs, `whats-new.md` where relevant) and **fold durable facts into code comments / in-tree developer docs**, then thin the backlog.
- User-facing material stays separate (`whats-new.md` while on `mgg-develop`, published handbook/docs as appropriate).

## How we work with assistants (session hygiene)

- **Default: new conversation per distinct issue** — keeps context size down; tools re-read code and this file as needed.
- **Reopen an old chat** when that thread’s history is intentionally wanted in context.
- **Durable knowledge** goes here, in **rules / `AGENTS.md` / `designs/` / code comments**, or git — not only in chat.
- Ask the assistant to **update rules / this backlog / designs** when something will matter in future sessions.
- Next real work: expect a **new feature branch** off current integration line (not pile everything only on long-lived chat state).
- **Build before commit:** day-to-day C/Python can use `./afwdev build --cdev` (implies `-j`). Before commit/push on docs, multi-area, or finish-pass work, prefer **`./afwdev build --fulldev`** (`--all --generate --clean --install --scan` + `-j`). PR gate still pairs with `afwdev test -j --env-mode valgrind`.

### Session wrap-up — 2026-08-06 (#17 merged)

- **#17** merged to **`mgg-develop`** via [PR #150](https://github.com/afw-org/afw/pull/150) (`dd318e4f`). Issue renamed/closed; whats-new + pads updated for landed status.
- **#149** (child of **#2**) remains open for runtime/`afw` catalog lifetime — `designs/runtime-catalog-lifetime.md`. Not started.
- Optional residuals O1–O4 done as docs/audit only before merge.
- **Next session:** not #17 feature work; pick other beta items or #149 later.

### Session notes — 2026-08-06 (object multi-impl cleanup)

- Branch **`cleanup-object-composite-impls`**: remove dead half-finished object impls deferred from #17.
  - **Removed:** `afw_object_create_composite` + `afw_object_create_properties_callback` (sources, public decls, internal selfs, opaque). No in-tree callers; unfinished (NIY get_count, broken/empty iterators, composite `get_setter` always NULL).
  - **Kept:** `afw_object_create_merged` (actions); `afw_object_aggregate_external_create` (**live** — `afw_command_local_server` request properties); object **option** `composite` (views / parentPaths — different thing).
  - **Residual:** memory `clone_on_set` field always false; not productized.
  - Product mutable look-through remains **faces** (`create_wrapper_*`), not these APIs.
- **#127** (progressive retrieve release): **not** app-only. C still has `@fixme Need corresponding releases` on script/materialize `impl_retrieve_cb` in `afw_function_adapter.c`; stream/to_response paths differ. Caps/faces landed earlier; progressive **release** still C (+ any app progressive consumer). Blocked on focused work / Jeremy for end-to-end progressive path, not “only app.”

### Session wrap-up — 2026-07-20

- Explored **#54** (indexes / deprecated variables); did **not** implement — notes under Indexes below.
- Created **`beta-backlog.md`** on **`mgg-develop`** (initial commit `9bfefbf7`; follow-up commits for hygiene/wrap-up notes).
- No feature implementation this session; partnership agreement: keep this file together over months toward beta.

### Session notes — 2026-07-21 (issue #1 + Doxygen / interface intent)

- Local commit on **`issue-#1`**: C file-level Doxygen hygiene + non-skeleton generator briefs (see git history).
- Design intent captured in rule **`afw-interfaces-doxygen`** and below under **Doxygen / interface API docs**.
- Follow-ups: macro Doxygen quality from XML/`interfaces.py`; group tree; thin `src/afw/doc/developer/*.md`; re-scope #1 away from infinite file stamps.

### Session wrap-up — 2026-07-22 (PR #132 merged; #1 closed)

- PR **#132** merged to **`mgg-develop`** (`c8be2744`). Follow-up **`c31df26e`**: `whats-new.md` + backlog status.
- GitHub **#1** closed, labels **documentation** + **implemented**, comment **@JeremyGrieshop** with summary + links.
- Durable (also in rules / AGENTS — do not re-litigate):
  - **`--cdev`** day-to-day; **`--fulldev`** = `--all --generate --clean --install --scan` (both imply **`-j`** / parallel). **`--all` alone does not generate/install** (version bumps need generate).
  - **Core vs base srcdirs:** `src/afw/` = libafw; other `src/*` self-contained over public core; extension Doxygen in that srcdir’s headers.
  - **Doxygen builders:** macros = C API; edit XML/`interfaces.py`/`afw_doxygen.h`/hand headers — not `generated/`; leave skeleton `@todo` alone; don’t stamp every `.c`.
  - **`local_test.py`:** normalizes local-mode version banner (expects don’t track package version).
  - Package **0.12.2** on this line after #132 work.
- **#1 is a wrap** — no further dedicated Doxygen campaign unless a real gap appears while editing.
- Next: **new conversation + feature branch off `mgg-develop`** per theme (indexes, memory, language, …). Keep dumping durable notes here and in rules as we go over the next months.

### Session wrap-up — issue #103 streams (closed)

- **#103** closed after **PR #120** (file streams) and **PR #121** (tests-as-assets) on **`mgg-develop`**. Comment **@JeremyGrieshop**: summary + reopen if more needed.
- User-facing: **`whats-new.md`** (File streams section + migration bullets).
- Durable rules (already on tree — do not re-litigate):
  - **`afw-stream`**: file streams, `rootFilePaths` (longest prefix / boundary / realpath containment), throw-based `stream()`, admin progressive write path left intact (`retrieve_objects_to_callback` / response hosts).
  - **`afw-script-errors`**: `_AdaptiveError_` / try-catch; no soft stream errors.
  - **`afw-tests`**: **never delete** `src/*/tests/` — afwdev runs from temp copies; fixtures under `src` are permanent regression assets.
  - **`afw-server`**: hosts beyond FCGI (`afw_command` local, etc.).
- Shipped surface: `open_file` + read/write/flush/close; removed unfinished `open_uri` / `open_response` / `get_stream_error` (network later via curl if ever).
- Tests: `src/afw/tests/miscellaneous/stream_file/` (~104 cases).
- Deferred by design (not reopening #103 unless needed): `open_uri` in curl; `stream_is_open` helper; soft stream errors.
- **#103 is a wrap** — new conversation for next theme.

### Operating notes for multi-month beta (assistants)

- Read **`AGENTS.md`**, always-on rules (especially `afw-project`, `afw-interfaces-doxygen`), and this file’s theme section for the issue at hand.
- Prefer **small feature branches** off `mgg-develop` → PR → merge; don’t accumulate unrelated work only in chat.
- After user-facing behavior changes on this line: update **`whats-new.md`**. After design decisions: **rules / this backlog / code comments**.
- Verify: day-to-day `--cdev` + `afwdev test -j`; broader/finish **`--fulldev`**; PR gate often adds valgrind tests.

---

## Notes dump

_Add new sections or bullets under the themes below. Newest thoughts can go at the top of a section or under “Inbox”._

### Inbox (unsorted)

_(Paste raw notes here first; sort into themes later.)_

### Doxygen / interface API docs (builders)

**Status:** **done / closed** on `mgg-develop` via PR **#132** (issue **#1** closed, labeled implemented; Jeremy notified). Rule **`afw-interfaces-doxygen`**. User-facing tooling notes: **`whats-new.md`**. Package **0.12.2**; **`afwdev build --fulldev`**.

**Audience:** AFW developers, extension/command authors, hosts — **not** pure Adaptive Script app users (handbook / `whats-new` for those).

**Architecture to remember:**

- C chosen over C++ for multi-request efficiency; interfaces still required.
- XML interface IDL + Python generate **call macros**, impl declares, closet skeletons (GDB-friendly wiring underneath).
- **Macros = real API** to document; improve via **XML + generators**, never hand-edit `generated/`.
- **Skeletons + afwdev `make-*` / `add-*`** are first-class developer UX; `@todo` / `<afwdev {…}>` intentional — do not “clean” for Doxygen vanity.
- Group essays live in `afw_doxygen.h`; hand headers join canonical groups; preserve long post-`@brief` bodies.
- C-focused HTML: `DoxygenLayout.xml`, `TYPEDEF_HIDES_STRUCT`, `doxygen-extra.css`, `PROJECT_NUMBER` from package version via generate.
- **Core vs base:** `src/afw/` = libafw; other `src/*` srcdirs stay self-contained (public core only). Extension Doxygen groups live in extension headers; core only lightly navigates.
- **Build profiles:** `--cdev` day-to-day; `--fulldev` = `--all --generate --clean --install --scan`; both include `-j` / parallel unless `-j N` is set (not `--all` alone).

**Landed with #132 (do not re-open as infinite file stamps):**

1. Macro Doxygen generator polish (`interfaces.py` `@param`/`@return`/`@relates`/`@see`).  
2. Group tree / nested `@defgroup` + thin group briefs.  
3. `src/afw/doc/developer/*.md` + mainpage / Related Pages.  
4. Interface XML descriptions; opaques; key hand `@file` bodies.  
5. `local_test.py` normalizes local-mode version banner.  
6. Closet noise excluded; C-focused layout/skin/typedef hide-struct; multi-layout `afw_value_t`.  
7. `--fulldev` shortcut; docs/rules prefer it over long `--all …` lines.

**Housekeeping done:** #1 closed + Jeremy comment.  

**Opportunistic only (not a campaign):** method-level XML while implementing an interface; Doxygen skin only if stock look regresses.

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
- `whats-new.md`: honest **partial** note only.

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

- Umbrella **#2** (memory). Working design pad: [`designs/memory-management.md`](designs/memory-management.md). Related: retrieve caps **#49**, progressive release **#127**, value lifetime rules in project docs / `.cursor/rules/afw-value-memory.mdc`.
- **#149** (child of **#2**) — runtime / `afw` adapter **catalog lifetime** (live maps vs `EnvironmentRegistry` materialize / admin catalogs). Pad: [`designs/runtime-catalog-lifetime.md`](designs/runtime-catalog-lifetime.md). Open; not started.
- **#17 mutable object faces** — **done** on `mgg-develop` (PR **#150**, 2026-08-06). Literals + emit, no object/array clone-on-bind, nested hard edge, adapter get/retrieve/callback, #110 defaults, journal get/consumer/advance, YAML hygiene. Pad: [`designs/issue-17-mutable-object-faces.md`](designs/issue-17-mutable-object-faces.md). User: **`whats-new.md`**.
- **Qualifier snapshots (issue #9)** — `qualifier()` / `qualifiers()` allocate **fresh memory objects** and can get **very large** (`environment::`, `request::`, nested `qualifiers()` over every active qualifier, multi-entry contribute). Documented as debug/tools/not hot path + size warning in function metadata, language XML, `whats-new.md`.
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

### Issue #55 — object/array helpers + array as vector

**Status:** **Closed** 2026-08-04 (PR **#134** on `mgg-develop`; re-verified). Feature work landed (C setter, script helpers, handbook, metas, residual memory-array polish).  
**GitHub:** [#55 Common object and array methods](https://github.com/afw-org/afw/issues/55) (closed)

#### Heritage / product framing (remember)

- **Original Adaptive functions** largely map **XACML v3** (shorter Adaptive names): `all_of` / `any_of` / `*_all` / `*_any`, `bag` / `bag_size` / `one_and_only`, typed polymorphic ops, etc. Maintainer once had a full C XACML v3 compliance engine; abandoned as product but designed **AFW so XACML can map onto Adaptive**.
- **XACML extension** (other repo, started): thin layer — register XACML functions/types/combining algs in the **AFW environment** pointing at existing Adaptive execute paths; XACML compile looks up registry. Core stays Adaptive; XACML is not a second type system in base.
- **Bag vs array:** There is **no** separate `bag` data type (old bag type caused pain; then **list**, then **array** for script/JSON familiarity — #48 leftovers). **Bag is real as semantics + bag-oriented functions** (multiset / XACML algebra “under the covers”); **runtime representation is `array`**. Touchy “don’t call bags arrays” people still get bag *functions*; script people get one sequence type.
- **ECMAScript / TypeScript:** Adaptive Script is **not** ES/TS and must differ where the problem set requires it; **avoid unmotivated differences**. Maintainer/beta decision notes: root [`typescript-differences.md`](typescript-differences.md) (Should fix / Will not do; **not** Jeremy’s polished differences doc — see **#22** for that). test262-derived tests help ES programmers — not to make AFW into ES.
- **Docs rule:** Core user/reference docs should **not** need to say “ECMAScript” or “XACML” except (1) Jeremy’s differences doc / language-compare material, (2) future **XACML extension** docs. Describe Adaptive on its own terms. Maintainer dumps (this file, `designs/`) may still mention heritage.

#### #55 ask vs already present

| Jeremy (JS-ish) | Adaptive today / direction |
|-----------------|----------------------------|
| `every` / `some` | Have `all_of` / `any_of` (XACML-shaped, multi-array). **Open to adding `every`/`some`** as ES-facing names (thin over same HOF machinery) for docs/tests — keep `all_of*` as first-class. |
| `push` / bulk append | C `push_value`; script still `add_entries` / spread until wrappers |
| `keys` / `values` / `entries` / `freeze` | **Missing** at script layer (freeze = expose immutability) |
| `at` / stack ops / `splice` | C ready; script `at`/`pop`/`shift`/… not started; splice deferred |

#### Array as vector / deque (C) — done on branch

- **`afw_array_setter` reshaped:** seal; `push_value`/`push_internal`; `pop_value`/`shift_value` (+ optional `found`); `insert_*` / `set_value` / `remove_value_by_index` with **`afw_integer_t`** indexes; content `remove_*` / `remove_all_values`.
- **Indexes:** negatives from end; insert may land at count (append); set/remove require element. `insert_value(index, value)` order.
- **Empty pop/shift:** NULL + optional `found` (not throw). Script wrappers can treat NULL as undefined.
- **Memory array `get_count`:** maintained **`self->count`** O(1); mutators keep it in sync. `get_entry_value` supports from-end negatives like the setter.
- **Helpers:** `afw_array_push_value`, `pop_value`, `shift_value`, `insert_value`, `set_value`, `remove_value_by_index`, …; all repo call sites + data_type_bindings generator updated.

#### Adaptive function homes

| Area | File / place |
|------|----------------|
| Structural array ops (`slice`, `join`, `add_entries`, future `at`/`push`/`pop`/…) | `afw_function_array.c` + category `array` metadata |
| HOFs (`all_of`, `filter`, `map`, …; future `every`/`some`) | `afw_function_higher_order_array.c` |
| `length` / `bag_size` / `bag` / `clone` / poly `includes` | `afw_function_polymorphic.c` |
| Object `keys`/`values`/`entries`, object freeze | `afw_function_object.c` (or poly freeze) |
| C mutability | `afw_array_*` + interface XML `afw_array_setter` |
| generate names | snake_case `functionId`/`functionLabel`; camel auto for JS bindings |

#### Residual concerns

| Concern | Status | Notes |
|---------|--------|--------|
| **`afw_value_meta_values_list` / `_object`** | **Done** | Lazy immutable views; `metas()` for array/object. Tests: `miscellaneous/meta_values.as`. |
| **`set_value` / discard slot release** | **Deferred to #2** | Commented-out helper + `@fixme #2` in `afw_array_memory.c` (match object store-as-is for now). When hold-on-store lands: `optional_release` on set/remove/remove_all; not on pop/shift. |
| **Mid-array insert/remove O(n)** | **Improved** | Still O(n) ring, but index locate walks from **nearer end** (`impl_entry_at`). Ends (`push`/`pop`/`shift`/`insert 0`) stay O(1). Contiguous vector later only if hot. |
| **`get_next_internal` iterator** | **Done** | Matches `get_next_value`: do not store sentinel in iterator; clear to NULL at end. |
| **Stored C NULL vs empty on pop** | **Documented** | Optional `found`; interface + `afw_array.h` describe empty vs removed NULL. |
| **No C vtable `unshift` name** | **Documented** | Intentional: `insert_value(…, 0, …)` / `afw_array_insert_value(a, 0, v, xctx)`. Script has `unshift`. |
| **test262 `\fixme` / skips** | Parallel | Burn down over weeks/months; not #55 MVP. Differences doc #22 separate. |

#### Forward plan (from here)

1. **Done:** C setter + O(1) memory `get_count` + signed get/set indexes + residual memory-array polish above.
2. **Done:** script APIs + tests (`keys`/`values`/`entries`, `at`/stack/`splice`, `freeze`, `every`/`some`, metas, limits/combined).
3. **Done:** Language Reference **Objects and Arrays** + Features + `whats-new`.
4. **Process:** done — PR #134 merged; #55 closed 2026-08-04.
5. **Parallel / later:** full hold-on-store (#2); test262 burn-down; differences doc (#22).

#### Adaptive Script vs ECMAScript — structural (not optional polish)

- **Not prototypal.** There is no `Array.prototype` / `Object.prototype` chain, no mutable global constructor objects, no “methods live on a shared prototype.” That is a **primary** language difference (differences doc / #22), not a temporary gap.
- **No ES-style global objects** as a mutable global namespace. Some host/internal state is visible via **qualified variables** (`qualifier::name` / stacks); those are **not** free-for-all globals and are **not** generally assignable like ES globals.
- Script “methods” are **Adaptive functions** (often `dataTypeMethod` → `value->fn(...)` sugar), registered in the environment — not properties found by prototype walk.
- Converted test262 names like `Array.prototype.entries` are **historical labels** only; Adaptive form is `entries(array)` / `array->entries()` once those functions exist — never real `Array.prototype.*`.

#### test262 suite note (language only)

- Path: `src/afw/tests/test262/` — derived from **test262 language** chapter (not built-in lib like `Array.prototype.every`). README + `_convert.py` (historical helper).
- **~137** cases with `//? skip: true`. Common reasons: `Math.*` / `Number.MIN|MAX` / `isNaN` (auto-skip in convert), missing `String.fromCharCode`, undecided numeric forms (`.0e1`, `0.e1`), string escape policy, ES completion-value/`eval(script(...))` try tests, for-of statement-position decls (`const`/`let`/`function` — “should we allow?”), generators/Symbol iterators.
- Assertion styles mixed: modern `assert(x === y)`; older `throw '#n: ...'`; success often `//? expect: undefined` (not `return 0`). Broken leftovers: multi-arg `assert(a, b, msg)`, `assert.throws`, half-converted `array.entries()` / `array.keys()` for-of tests that currently **`expect: error` (parse)** — **rework** in Adaptive idioms when features exist (not pure unskip; not prototype APIs).
- Policy: when a skip’s intent is clear and Adaptive can express it, convert to Adaptive idioms and remove `skip` rather than leave forever.

#### test262 / language `\fixme` backlog (come back — weeks/months)

**Intent:** Work through **all** `\fixme` (and related `//? skip: true`) in `src/afw/tests/test262/` systematically with maintainer — decide Adaptive behavior, convert tests, or document deliberate non-support in differences material. Not a single PR; ongoing.

**Clusters already spotted (incomplete inventory):**

| Area | Examples / themes |
|------|-------------------|
| Numeric literals | `.0e1`, `0.e1`, trailing `.` — “implement these or not?” |
| String escapes | NonEscapeSequence letters, whether `\A` ≡ `A`; syntax errors on raw CR/LF in strings |
| `String.fromCharCode` | Missing Adaptive equivalent; several string tests blocked |
| for-of statement position | `const`/`let`/`function` in head — “should we allow?” |
| try + completion / `eval(script(...))` | ES completion-value model vs Adaptive |
| Math / Number / isNaN | Convert-auto-skip; only if Adaptive owns those ops |
| Half-converted for-of | `array.entries()` / `keys()` / Symbol.iterator — rewrite when Adaptive APIs exist |
| switch + isNaN cases | Needs `isNaN` + case semantics decision |

When closing a fixme: remove or rewrite skip, fix asserts to Adaptive style, keep test **id/description** for lineage where useful.

---

### Beta gate (checklist sketch)

_Not a commitment — fill in as “must be true before we call it beta.”_

- [ ] Memory / long-running story credible (**#2**): managed values, pools, no silent leak under realistic server/script load
- [ ] Large materializations constrained or progressive where needed (**#49**, **#127**, client progressive path)
- [ ] Snapshot / debug APIs (e.g. **#9** `qualifier`/`qualifiers`) documented as non-hot-path and size-aware; not used as everyday data access
- [ ] User-facing behavior documented in `whats-new.md` / real docs as appropriate
- [ ] `mgg-develop` merged to `develop` when ready; `develop` → `main` when beta-ready

---

## Done (archive short notes)

| When | Item |
|------|------|
| 2026-07 | **#103** file streams — PR #120 (+ #121 tests assets); closed, Jeremy notified. Details in session wrap-up above; `whats-new.md` File streams. |

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
| 2026-07-21 | #54 docs: soft whats-new; afw-adapter-index rule; AGENTS/extensions/qualified-vars; how indexes work. |
| 2026-07-29 | #55 brainstorm dump: bag=functions/array=type, XACML extension mapping, doc boundary (no ES/XACML in core), vector/deque setter, every/some optional, function file map. |
| 2026-07-29 | #55 notes: not prototypal / no ES globals (qualified vars); test262 ~137 skips + plan to burn down all `\fixme` over weeks/months. |
| 2026-07-29 | #55: C array_setter reshape + O(1) get_count; residual concerns + forward plan in this file. |
| 2026-07-30 | #55: residual polish — nearer-end index walk, get_next_internal, unshift docs via insert_value(0); set/remove optional_release deferred to #2 (breadcrumb FIXME, object-safe store-as-is). |
