# AFW beta backlog (brain dump)

**Audience:** maintainers and assistants working toward a beta-quality tree.  
**Not for end users.** User-facing changes go in [`whats-new.md`](whats-new.md).

## Purpose

Dump **details, design thoughts, unfinished plans, and “don’t forget” items** out of maintainers’ heads so they survive chat sessions and months of beta work.

| Document | Role |
|----------|------|
| **`beta-backlog.md`** (this file) | Working notes, plans, archaeology, half-decided design. Source of truth for “what we still need to remember.” |
| **`designs/`** | Per-issue / per-theme design pads (not user docs). See [`designs/README.md`](designs/README.md). |
| **`designs/issue-2-hold-in-inf.md`** | **#2 rails** — `get_reference` / `get_assignable_value`; MUST NOT. |
| **`designs/experiment-brainstorm.md`** | **#277 closed** — two worlds, create names, last_return. |
| **`designs/experiment-eval-p.md`** | **#287 landed** — eval `p` = `scope->p` when `{ }` has a frame. |
| **`designs/issue-2-lifetime.md`** | **#2 08-21 story** (history). |
| **`designs/memory-management.md`** | Umbrella **#2** archaeology / old phases. |
| **`whats-new.md`** | What **users** of AFW need to know about **`develop`** (behavior, APIs, migration). |
| **GitHub issues** | Optional promotion when something needs discussion, an assignee, PR linkage, or is a real beta blocker. Prefer thematic umbrellas (e.g. language, memory) over one infinite meta-issue. |

## Branch plan (as of 2026-08-14)

**Steady state (current):**

```text
feature branches  →  develop  →  (cleaned up, ≥ beta) main
```

- **`develop`** is the shared integration branch again.
- Day-to-day work is **feature branches off `develop`**.
- User-facing “what’s new”: **`whats-new.md`**.
- Harness: **orchestrated tests** (PR **#167**); **`afwdev blast` retired** → `schedule.firehose` + `src/afw/tests-extra/` (`-T`).

**Historical — concentrated AI / Grok Build pass (mid‑2026):**

```text
feature branches  →  mgg-develop  →  (user testing) develop  →  main
```

Volume of change would have overwhelmed the usual `develop` cadence, so **`mgg-develop`** was cut off `develop` as a long-lived staging line. That campaign merged into **`develop`** via [PR #179](https://github.com/afw-org/afw/pull/179) (**Create a merge commit**, history preserved). Tags: `before-mgg-develop-merge`, `mgg-develop-final`, `after-mgg-develop-merge`. The `mgg-develop` branch name is kept for now; do not use it as a base for new work.

### Retarget after `mgg-develop` → `develop` (done)

A merge only moves tree content. Branch names in prose and URLs do not
update automatically. Cutover follow-up (this file / `whats-new` /
test262 How URLs / agent “new work” pointers):

- [x] **`whats-new.md`** — title / window is since `before-mgg-develop-merge`
- [x] **`src/afw/tests/test262/changes.md`** — Index **How** commit URLs
  (`…/commits/mgg-develop/…` → `…/commits/develop/…`) and intro / window text
- [x] **This file** and other maintainer notes — day-to-day trunk is **`develop`**
  again; stop pointing new work at `mgg-develop`
- [x] Issue/PR links (`/issues/N`, `/pull/N`) — **no change** (not branch-scoped)

Historical “landed on `mgg-develop` via PR #N” lines stay. Optional later:
delete the remote `mgg-develop` name when nobody needs the tip.

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
- User-facing material stays separate (`whats-new.md`, published handbook/docs as appropriate).

## How we work with assistants (session hygiene)

- **Default: new conversation per distinct issue** — keeps context size down; tools re-read code and this file as needed.
- **Reopen an old chat** when that thread’s history is intentionally wanted in context.
- **Durable knowledge** goes here, in **rules / `AGENTS.md` / `designs/` / code comments**, or git — not only in chat.
- Ask the assistant to **update rules / this backlog / designs** when something will matter in future sessions.
- Next real work: expect a **new feature branch** off current integration line (not pile everything only on long-lived chat state).
- **Build before commit:** day-to-day C/Python can use `./afwdev build --cdev` (implies `-j`). Before commit/push on docs, multi-area, or finish-pass work, prefer **`./afwdev build --fulldev`** (`--all --generate --clean --install --scan` + `-j`). PR gate still pairs with `afwdev test -j --env-mode valgrind`.

### Session wrap-up — 2026-08-14 (#157 closed)

- **#157 closed:** `advanced-test.yaml` plan replaced by orchestrated tests (PR **#167**). Leftover knobs stay on **#13**; memory stress on **#2**.
- History pad: `designs/afwdev-advanced-test.md`.

### Session wrap-up — 2026-08-10 (docs polish / partner try-it)

- **`whats-new.md`:** Highlights anchors/back-links polished; #149 slug; migration bullets; experimental heading clean-up.
- **`open-issues-status.md`:** refreshed from live GitHub (**42** open; closed #17/#39/#50/#61/#89/#149/#153/#158 removed from open table; **#157** noted).
- **Partner docs:** `designs/ai-partner-lessons.md` *If you try it* expanded (issue → discuss → feature branch → checkpoints → PR); `AGENTS.md` short mirror; atlas §15 TOC link fixed.
- **Handbook:** Features **Closure** no longer claims Adaptive has no closures (aligned with `closures.as` / #35 residual under **#2**).

### Session wrap-up — 2026-08-09 (#149 phase 1 + afwdev harness)

- **#149 closed** (2026-08-09): phases 1–3 via PRs [#160](https://github.com/afw-org/afw/pull/160), [#161](https://github.com/afw-org/afw/pull/161), [#162](https://github.com/afw-org/afw/pull/162) (`ad78aa62`).
  - Architecture pads; `_AdaptiveRuntimeValueAccessor_` registry; lock+copy **referenceCount**; objectOptions pool fix on permanent shells; **metrics/properties** are caller-pool snapshots (pin released after the copy; `metrics.additional` pins only across `get_additional_metrics`).
  - Tests: `catalog-value-accessors`, `tests-extra` lifecycle + blast; object_options envreg cases.
  - Residuals (full-registry materialize size, long-running pool pressure) under **#2**.
- **afwdev follow-ons** (with #160 / after **#159**): `--tests-path`/`-T`, `--output`/`--output-format`, **#61** exceptions (closed).
- **#13** stress: comment to Jeremy; left open.

### Session wrap-up — 2026-08-06 (#17 merged)

- **#17** merged to **`mgg-develop`** via [PR #150](https://github.com/afw-org/afw/pull/150) (`dd318e4f`). Issue renamed/closed; whats-new + pads updated for landed status.
- **#149** (child of **#2**) opened for runtime/`afw` catalog lifetime — later phase 1 landed 2026-08-09 (see wrap-up above).
- Optional residuals O1–O4 done as docs/audit only before merge.
- **Next session (historical):** not #17 feature work; pick other beta items or #149 later.

### Session notes — 2026-08-06 (object multi-impl cleanup)

- Branch **`cleanup-object-composite-impls`**: remove dead half-finished object impls deferred from #17.
  - **Removed:** `afw_object_create_composite` + `afw_object_create_properties_callback` (sources, public decls, internal selfs, opaque). No in-tree callers; unfinished (NIY get_count, broken/empty iterators, composite `get_setter` always NULL).
  - **Kept:** `afw_object_create_merged` (actions); `afw_object_aggregate_external_create` (**live** — `afw_command_local_server` request properties); object **option** `composite` (views / parentPaths — different thing).
  - **Residual:** memory `clone_on_set` field always false; not productized.
  - Product mutable look-through remains **faces** (`create_wrapper_*`), not these APIs.
- **#127** (progressive retrieve release): **closed** for write-only `to_response` / `to_stream` / HTTP list. Residual: `@fixme Need corresponding releases` on script/materialize `impl_retrieve_cb` in `afw_function_adapter.c` (script may retain the object). Not an admin conversion task.

### Session wrap-up — 2026-08-06 (cleanup + managed face pin)

- Landed on branch then **PR → `mgg-develop`**: dead composite/properties_callback removal + **`afw_pool_release` → pool or NULL** + managed object face pins `wrapped`.
- **Invariant (objects):** managed memory object lifetime = **its pool’s RC**. Managed face: `get_reference(wrapped)` once at create; on face release, if `afw_pool_release(face->p) == NULL`, `release(wrapped)`. Unmanaged face still borrows. Save `wrapped` before pool release (self may be freed).
- **Arrays:** memory arrays still pool-owned, **noop** `release` / unmanaged value face — **not** the same bug today. When #2 gives arrays real managed RC, mirror object face pin (plus other array MM work).
- **Not removed:** `create_merged`, `aggregate_external`, views, meta, `create_view_of_c_array` — different jobs than faces.
- Durable notes: `designs/memory-management.md`, `designs/issue-17-mutable-object-faces.md`, comment on **#2**.

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
- Next: **new conversation + feature branch off `develop`** per theme (indexes, memory, language, …). Keep dumping durable notes here and in rules as we go over the next months.

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
- Prefer **small feature branches** off `develop` → PR → merge; don’t accumulate unrelated work only in chat.
- After user-facing behavior changes on this line: update **`whats-new.md`**. After design decisions: **rules / this backlog / code comments**.
- Verify: day-to-day `--cdev` + `afwdev test -j`; broader/finish **`--fulldev`**; PR gate often adds valgrind tests.

---

## Notes dump

_Add new sections or bullets under the themes below. Newest thoughts can go at the top of a section or under “Inbox”._

### Inbox (unsorted)

_(Paste raw notes here first; sort into themes later.)_

- **Soon (2026-09-11):** Generated file banners — **by what** and **from what**. Hand headers stay silent (that *is* the signal they are the authority). Generated `.h`/`.c` already stamp `This file is generated by "afwdev generate <srcdir>"` via `_afwdev/generate/c.py` `get_generated_by`; they do **not** name the `generate/` input. Cleanup: one shared prologue — by `afwdev generate` (or the rare other emitter, e.g. `additional_generate.py`), from the `generate/` path(s), keep “do not edit this file.” Honest **from** for composites (strings: `generate/strings/` plus other generate inputs in the same pass). Fold EBNF’s shorter `Generate by:` line into the same prologue. Not a header-by-header edit; pass source path(s) from each generator and regenerate. Pad pointer: [`designs/libafw-headers-and-api-surface.md`](designs/libafw-headers-and-api-surface.md).

### Admin app bugs (found 2026-10, during the TanStack Router migration)

All pre-existing (not caused by the migration). The `done` ones are fixed on `fix/admin-backlog-bugs` (after #459), each with a regression test. Context: [`designs/tanstack-router-migration.md`](designs/tanstack-router-migration.md).

- `done` **Model > Data Mappings: "Something went wrong!" / `e.getObjectId is not a function`** (often, not always). `Admin/Models/Mappings/ModelMapping.js` retrieves the mapped adapter's object types with `modelOptions: { loadObjectTypes: false }` - not an option anything reads - and passing any object replaces `AfwModel.retrieveObjects`' default `{ adaptiveObject: true, initialize: true }` (`afw_client/javascript/src/model/AfwModel.ts`), so the results are plain JSON; `mappedObjectTypeObject?.getObjectId()` then throws (`?.` only guards null/undefined). Fails whenever the mapped adapter has object types. Fixed: `AfwModel` fills in the defaults a partial `modelOptions` leaves out (`retrieveObjects`, `getObject`, `getObjectWithUri`), and `ModelMapping` drops the bogus option - it needs initialized objects anyway, since `getObjectId()` reads the object type. A second path hit the same message: an object type's Data Mappings (`.../objectTypes/<type>#mappings`) - `ModelObjectTypes.js` passed `objectTypeObject.getPropertyValues()` (plain values) as `propertyTypes`, and `useEventId`'s debug label calls `getObjectId()` on it; it now passes `getPropertyValue("propertyTypes")`.
- `done` **Service editor: the first keystroke jumps from the Configuration tab back to General.** The material `Tabs` (`afw_components/react/material/src/components/Tabs/Tabs.js`) resets its selected tab whenever the `tabs` prop changes identity (`useEffect(..., [props.selectedTab, props.tabs])`, since the 2023 initial commit). Every service editor builds `tabs={[...]}` inline; the first keystroke makes the service savable, re-rendering the editor with a new array. Affects any of the ~33 `<Tabs>` users that re-render while a later tab is shown. Fixed in `Tabs`: reset only when `props.selectedTab` or the tab set (keys/texts) changes - no longer on a new array with the same tabs - and show the first tab when the selection is past the end.
- `done` **Model overview tables put the view hash into the link's path.** `Admin/Models/Overview/ModelObjectTypesTable.js` and `ModelPropertyTypesTable.js` pass `hash` inside `uriComponents` (`[..., objectType, hash]`), so with a view hash set the link becomes `.../objectTypes/<type>/%23tree` (and without one, ends in `/`). These are the editable overview's tables; the read-only overview builds its own links. Fixed: the hash is appended after the encoded path.
- `done` **Re-examine how `template` values render: read-only mode shows an editable text field** (e.g. a file adapter's `properties.root` on its service/adapter page, before clicking Edit). `afw_components/react/core/src/layouts/dataTypes/Template/Template.js` has no read-only branch: collapsed, it always renders a multiline `TextField` wired to `onChanged` (the material `TextField` only knows `disabled`, not read-only), so it looks editable and may accept typing; only the expanded `CodeEditor` honors `readOnly={!editable}`. `String.js` has a `StringReadOnly` path; `Script.js` uses a read-only `CodeEditor`. Fix: in read-only mode render the template as text (pre-wrap, monospace) or a read-only `CodeEditor`, like those two; keep the `TextField` only when editable. Reported by the user while testing the router migration. Fixed: read-only templates use `StringReadOnly` (as `text/plain` strings do); editing is unchanged. Worth a look whether templates deserve monospace or highlighting.
- `done` **Schema: state set during render.** `Admin/Schema/ObjectTypes.js` calls `setAllowAdd`/`setAllowChange`/`setAllowDelete` inside a `useMemo`. Fixed: the memo returns them with the object types.
- `idea` **Read-only model overviews don't encode ids in their links.** `Admin/Models/Overview/ModelOverview.js` (`ModelOverviewReadonly`) and `ModelObjectTypeOverview.js` (`ModelPropertyTypesTableReadonly` and friends) build `url={"/Admin/Models/" + adapterId + "/" + modelId + "/objectTypes/" + objectType + "#overview"}` by concatenation, so an id with `/`, `#`, `?` or `%` breaks the link. Fix: `encodeURIComponent` each part, as the editable tables do. Found while fixing the overview hash links.

### Docker image bugs (found 2026-10, local multi-platform build of `afw:alpine`)

- `PR` ([#469](https://github.com/afw-org/afw/pull/469), `fix/afw-base-alpine-3.24`: `afw-base/Dockerfile.alpine` → 3.24; `afw-base:alpine` still to publish) **`afw:alpine` (and `afwfcgi:alpine`) build but `afw` cannot start: the runtime base is still Alpine 3.16.** Pre-existing. `docker run ghcr.io/afw-org/afw:alpine afw --version` on arm64 and amd64: `Error loading shared library libedit.so.0` and `libicuuc.so.78`, `u_strToUTF8_78: symbol not found`. The C stage builds on `afw-dev-base:alpine` (Alpine 3.24 since #421, ICU 78), and the final stage is `FROM ghcr.io/afw-org/afw-base:alpine`. `docker/images/afw-base/Dockerfile.alpine` still pins `OS_VERSION=3.16.9` (ICU 71), because #421 bumped only `afw-dev-base`. The published `afw-base:alpine` was created 2025-10-08 and has no `libedit` either, even though the Dockerfile lists it, so it is older than its Dockerfile. Fix: bump `afw-base/Dockerfile.alpine` to the same Alpine as `afw-dev-base` (3.24), rebuild and publish `afw-base:alpine` (by hand, as for the dev bases: [`designs/docker-cross-platform-builds.md`](designs/docker-cross-platform-builds.md)), then rebuild `afw`/`afwfcgi` and check `afw --version` in each platform. Better long term: derive both bases from one Alpine version so they cannot drift.

### ASAN findings (found 2026-10, first ASAN run of `test -j`)

All pre-existing. Found by an ASAN + UBSan `libafw` with the pool annotations from `feature/asan-opt-in` (pad: [`designs/asan-opt-in.md`](designs/asan-opt-in.md)); plain ASAN without the annotations reports none of the pool ones. Repro: build per the pad, then run the `.as` from its directory with the ASAN `afw` and `ASAN_OPTIONS=detect_leaks=0:detect_odr_violation=0`.

- `PR` ([#462](https://github.com/afw-org/afw/pull/462), `fix/sort-values-overflow`) **`sort()` writes one pointer past its `values` block.** `afw_function_execute_sort` (`function/afw_function_higher_order_array.c`, the `for (iterator = NULL, value = ctx.values;; value++)` copy loop) mallocs `count` pointers and stores every `afw_array_get_next_value` result including the terminating NULL, so it writes `count + 1`. In a normal build the 8 bytes land in alignment slack or on the next block's `chunk` word (free-list corruption when that block is later freed). Tests: `generated/functions/sort.as`, `language/script/higher_order_array.as`, `compiler/function_contextual.as`. Fix: stop before storing the NULL (or loop `i < count`).
- `done` (fixed on `develop` by the #2 lifetime work in #470, `34f4eed9`: compile-unit literals are unmanaged in the unit; the ASan suite passes these tests 2026-10-05) **A managed container can outlive the nested compile unit whose literals it holds.** Root-caused 2026-10-03. Compile literals (`compile_literal_*`, #280) are stored as-is: `get_reference` / `get_assignable_value` return self and take no hold on their unit. `evaluate(compile(...))` keeps the nested unit for the `compile` call's dest `p`, which is the enclosing script's block scope. A managed array built during the nested evaluate (`reverse` / `slice` / `bag` / `union` / `intersection` push the input elements) keeps the literal pointers, and leaves through `script_result` (a reference, not a copy). The compiled-value evaluate (`value/afw_value_compiled_value.c`, end of `impl_afw_value_optional_evaluate`) copies only a **scalar** unit literal. When the block scope ends, the unit is released (`afw_value_block.c:136` cleanup → `impl_managed_optional_release`), and the result's elements dangle. `test_script` hits it because each test's source is a nested unit compiled into the outer block scope (`afw_value_call_test_script.c`). Normal-build repro, segfaults: an `afw.conf` with `{type: "application", applicationId: "afw", memoryRegionFreeListMaxBytes: 0}` and the script `return evaluate(compile<script>(script("reverse(bag<integer>(integer(2), integer(9)))")));`. With the default region cache it prints `[9, 2]` from a stale chunk. Controls: the same `reverse` in the outer unit is fine; using the nested result inside the script (`string(x)`) is fine. Fix is a #2 design choice (copy at the unit's evaluate boundary, mark containers that hold literals, pin the unit, or copy-on-push); discuss before code. Tests: `generated/functions/{reverse,slice,integer_bag,double_bag,integer_union,double_union,integer_intersection,double_intersection,sort}.as`.
- `done` ([#466](https://github.com/afw-org/afw/issues/466) closed; fix `095e8122` on `develop` via #470; regression `miscellaneous/service_restart_compiled_conf/` segfaults without the fix on a normal build; touches Mike's `e583401a` order - review with him) **Pool last-release destroys unreferenced children before running its cleanups, so a cleanup can release a value inside a destroyed child.** `afw_compile_*` gives a unit its own pool as a child of dest `p->managed_p` and registers the unit's last-release as a cleanup on dest `p`. For a job heap (adapter, service, conf: dest `p == p->managed_p`), last-release of that pool (`afw_pool_internal_release_common`, `pool/afw_pool.c`) runs `destroy_children` (the unit's pool goes) and then `run_cleanups` (releases the dead unit: reads freed memory, then tears the same pool down again, putting its chunk on the region free list twice). Seen on a file adapter restart: `afw_file_adapter_create_cede_p` compiles `root` into the adapter pool; `service_restart` destroys the old adapter. Normal build with `memoryRegionFreeListMaxBytes: 0` in the application conf segfaults in `authorization/application/requiresExecuteAccess.as`; with the default cache the double teardown is silent. Order came in with `e583401a` (2026-09-22, "pool: pin a parent only while a child is held"); before it, last-release threw if children remained. `remaining-apr.md` describes callbacks before teardown. Fix on the branch: run cleanups, then destroy children, then teardown. Normal `test -j` 4589/0; the ASAN suite loses exactly the two authorization failures and gains none. Tests: `authorization/application/{requiresExecuteAccess,functions_permit/permit_protected_functions}.as`. With the region cache off it also fails `advanced/catalog-value-accessors` (`afwfcgi`, "hold metrics and properties across service_stop": `None` instead of `file`, 3/3 without the fix, 0/3 with) and the corrected `compiled_value_managed` `job_heap` probe (segfault).
- `done` ([#467](https://github.com/afw-org/afw/issues/467) closed; `d3bba758`, `60fc9c37` on `develop` via #470) **`compiled_value_managed` probe `job_heap` releases a compile result it never held.** It still follows the contract before `3fdaabdd` ("afw_compile_* is caller does not release"): `afw_value_release(value)` after `afw_compile_to_value(..., job, ...)`, then `afw_pool_release(job)` runs the registered last-release of the same unit - a double release that tears its pool down twice (silent with the region cache, segfault without). `front_door` has the same stale second release (its first release correctly balances `afw_value_get_assignable`). No library caller releases a compile result (scanned). Fix: drop the stale releases (two in `job_heap`, one in `front_door`). With the cache off, the fixed probe still segfaults until the pool-order fix above is in, so it doubles as a regression for that.
- `done` (fixed by #470 with the entry above; `substitution.as` passes under ASan 2026-10-05) **A scalar compile literal returned from a function in an `eval_from_file` unit outlives that unit.** `miscellaneous/substitution/substitution.as` case `hello_world_call_loaded_function`: `const x = eval_from_file('includes/a.as'); return x();` where `a.as` returns `function a() {return "Hello World!";}`. The result is that unit's `compile_literal_string`. Block-exit isolation into `script_result` keeps it as-is (`get_assignable_value` is self), the block scope's cleanup releases the `a.as` unit (`afw_value_block.c:136`), and only then does the compiled-value evaluate (`afw_value_compiled_value.c:355`) read it to copy unit-backed scalars - too late. Normal build with `memoryRegionFreeListMaxBytes: 0` in that test's application conf segfaults on that case. Scalar-specific detail for the #2 fix: the existing scalar copy runs after the block scope is released.
- `PR` ([#465](https://github.com/afw-org/afw/pull/465), merged; guard also requires `typedef` after review) **Model `current::` runtime objects scan the model context as if it were a property list.** The 11 `_AdaptiveModelCurrent*` object types had `"runtime": {}`, so the generator (`_afwdev/generate/runtime_object_maps.py`) defaulted them to `afw_runtime_const_object_instance_t` with `properties_offset = offsetof(..., properties)` and `indirect = false`. The model creates them as `afw_runtime_object_indirect_t` skeletons (`afw_model_internal_create_skeleton_context`), and that offset is `indirect->internal`, the `afw_model_internal_context_t`. After the mapped properties, get and iterate (`afw_runtime.c`) walk the context's fields as `afw_runtime_property_t *` until a NULL field: the first "name" is `ctx->p`'s `inf` (`&impl_afw_pool_scope_inf`). Hit by `qualifier("current")` (contribute walk, `afw_xctx.c` `impl_contribute_object_variables_cb`) and by a `current::` name not in the map. ASAN: global-buffer-overflow in `afw_value_equal` (`mappedAdapterId_context.as`); `onGetSetProperty.as` fails or not depending on what the context holds. Normal build shows nothing (junk compares false). Fix: `"runtime": {"indirect": true, "typedef": "afw_model_internal_context_t"}` on all 11 (accessor-backed properties already got `indirect->internal`), and the generator now exits with an error when a type has `onGetValueCFunctionName` properties without `runtime.indirect`. Normal `test -j` 4589/0; under ASAN both tests and all 33 `model_adapter/` tests pass.
- `done` (fixed by #470 with the entries above; `get_retrieve.as` passes under ASan 2026-10-05) **Objects a model hook returns through `current::returnObject` keep literals of the hook's `eval_from_file` unit.** `model_adapter_script_only/get_retrieve.as` case `retrieve_objects_synthetic`: `onRetrieveObjects` is `eval_from_file('includes/onRetrieveObjects_synthetic.as')`, which calls `current::returnObject({ "x": true, "name": "one" })`. The retrieved object's `name` is that unit's `compile_literal_string`; the unit is released when the hook ends, and `const objects = retrieve_objects(...)` then clones the array into managed memory (`afw_value_slot_store` → `afw_object_create_managed_clone` → `impl_copy_property_into_managed`, `object/afw_object_memory.c:287`) and reads the dead literal. Normal build with `memoryRegionFreeListMaxBytes: 0` in that environment's application conf segfaults. Production shape: a model adapter whose `on*` hook is an `eval_from_file` returning object literals.
- `PR` ([#463](https://github.com/afw-org/afw/pull/463), `fix/flag-register-memcpy-null`) **UBSan: `memcpy` from NULL in flag registration.** `afw_flag_environment_register_flag` (`flag/afw_flag.c:784`, `:791`) copies `env->default_flags` / `env->flag_by_index` with `memcpy` on the first growth, when both are still NULL (size 0). Undefined even at size 0; prints on every ASAN process start. Fix: skip the copies when `env->flags_count_allocated` is 0.

- `done` ([#472](https://github.com/afw-org/afw/pull/472) merged) **UBSan: `memcmp` with a NULL pointer in the memory data-type compare.** `afw_data_type.c:90` (the `afw_memory_t` compare for `base64Binary` / `hexBinary`) calls `memcmp(v1->ptr, v2->ptr, size)` when a value is empty, so a pointer can be NULL with size 0; undefined even for 0 bytes (same class as the flag `memcpy`, #463). Tests: `generated/datatypes/{base64Binary,hexBinary}.as`, `miscellaneous/stream_file/stream_file.as`. Fix: skip the `memcmp` when the compared size is 0. Found by `--env-mode asan` (2026-10-04).
- `done` ([#473](https://github.com/afw-org/afw/pull/473) merged) **UBSan: misaligned 64-bit load of an LMDB journal key.** `afw_lmdb_journal.c:102` reads the last key as `*((afw_uint64_t *)(key.mv_data))`; LMDB does not align key data, so the load is misaligned (UB; a fault on strict-alignment CPUs). Tests: `afw_lmdb/tests/journal/journal_tests_{3,4}.as`. Fix: `memcpy` into `t`, then the endian conversion. Found by `--env-mode asan`.
- `done` ([#474](https://github.com/afw-org/afw/pull/474) merged) **UBSan: signed overflow before the overflow check in integer `multiply`.** `afw_function_execute_multiply_integer` (`function/afw_function_integer.c`, `next *= arg->internal;` then `next / result != arg->internal`) does the overflowing multiply first and detects it afterwards; signed overflow is UB, so the check is not guaranteed to run as written. Tests: `generated/functions/{integer_multiply,multiply}.as` (they expect the "Integer multiply overflow" error). Fix: check before multiplying (`__builtin_mul_overflow`, or a range check against `AFW_INTEGER_MAX` / `MIN`). Found by `--env-mode asan`.

- `idea` **tests-extra under `--env-mode asan`** (2026-10-04, `afwdev test --env-mode asan -T src/afw/tests-extra/<leaf>`): `01-smoke-sequential` passes. `service-restart-conf`, `adapter-lifecycle` and `stress-file-restart-only` crash `afwfcgi` on stop/restart and pass once the #466 fix is applied, so they are #466. `model-lifecycle` step `onGetObject-evaluate-compile-object` (expect `True`, got `None`) is an ASan use-after-poison evaluating a `reference_by_key` value (`afw_value_reference_by_key.c:120` → `afw_value.c:49`) under `eqx`; ASan-only (passes in normal mode). Likely the deferred compile-literal class (`evaluate(compile(...))` in a model hook); not confirmed.
- `idea` **`tests-extra/10-catalog-value-accessors` fails in normal mode too** (step `second-request-re-read`: "Assertion failed"), independent of ASan. Pre-existing; not diagnosed.

- `PR` ([#475](https://github.com/afw-org/afw/pull/475), `fix/integer-subtract-overflow-check`) **Integer `subtract` overflow check overflows itself for `AFW_INTEGER_MIN`.** `afw_function_execute_subtract_integer` (`function/afw_function_integer.c`) checks with `-arg2->internal`, which is signed overflow (UB) when `arg2` is `AFW_INTEGER_MIN`. Found by reading while fixing `multiply` (#474); no test exercises it, so UBSan has not reported it. Fix: check without negating (e.g. `arg2 < 0 && arg1 > AFW_INTEGER_MAX + arg2` / `arg2 > 0 && arg1 < AFW_INTEGER_MIN + arg2`), plus a test `subtract(0, -9223372036854775807 - 1)` expecting the overflow error.

### afwdev test bugs (found 2026-10, during the ASAN opt-in work)

- `PR` ([#464](https://github.com/afw-org/afw/pull/464), `fix/python-test-raise-hang`; regression `src/afw_dev/tests/harness/python_run_raises.py`) **A Python test whose `run()` raises hangs `afwdev test` forever** instead of reporting a failure. Pre-existing. Seen when a stale PATH `afwdev` hit an `ImportError` in a probe's `run()`: no child processes, stack parked in `_afwdev/test/modes/python.py` `run_test` `finally` (`stdout_r.close()`), both `_drain` threads blocked in `fd.read()`. Cause: the writer ends (`stdout_w` / `stderr_w`) are only closed on the success path, so after an exception the drain threads never see EOF; closing a buffered reader while another thread is inside `read()` on it waits on that reader's lock. Fix: in `finally`, close the writers first (if not already closed), join the drain threads with the existing timeout, then close the readers - the exception then surfaces as an ordinary test failure.

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
- Related but different issues: **#28** compile-time types (**closed** PR **#171**); **#97** `#{…}`; **#101** evaluate / unevaluated handling; app-shared functions **#170**.

### Language / script syntax (misc)

**Status:** planned dump area

- Expect many small items (finish/add syntax, etc.). Prefer notes here; graduate to GitHub (or extend umbrella **#62** Adaptive Script language changes) when actively implementing.
- _(Add bullets as they leave your head.)_

### Runtime / memory / long-running

**Status:** pointer / **beta-relevant**

- Umbrella **#2** (memory). **Live maps:** rails [`designs/issue-2-hold-in-inf.md`](designs/issue-2-hold-in-inf.md); two worlds [`designs/experiment-brainstorm.md`](designs/experiment-brainstorm.md) (**#277** closed); eval `p` [`designs/experiment-eval-p.md`](designs/experiment-eval-p.md) (**#287**). 08-21 story (history): [`designs/issue-2-lifetime.md`](designs/issue-2-lifetime.md). Archaeology: [`designs/memory-management.md`](designs/memory-management.md). Related: retrieve caps **#49**, progressive release **#127**, current-tree rules `.cursor/rules/afw-value-memory.mdc`.
- **#2** continues as **many feature branches** (request-lifetime MM retrofitted for script **scope** lifetimes). **Names as values** landed (PR **#220** + wrap-cleanup): object property names are `const afw_value_t *`; script/JSON string-only. Pad: [`designs/issue-2-property-name-values.md`](designs/issue-2-property-name-values.md). `eq` docs-vs-C is noted there as **not** that work.
- **#149** (child of **#2**) — runtime / `afw` adapter **catalog lifetime** — **closed** 2026-08-09 (PRs #160–#162). Pads: [`runtime-objects-and-environment.md`](designs/runtime-objects-and-environment.md), [`runtime-catalog-lifetime.md`](designs/runtime-catalog-lifetime.md), [`runtime-value-accessors.md`](designs/runtime-value-accessors.md). Residual cost/memory under **#2**.
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

#### #55 ask vs landed (historical planning table)

**Shipped** on `mgg-develop` (PR **#134**). Do not treat the pre-land rows below as open work:

| Jeremy (JS-ish) | Landed direction |
|-----------------|------------------|
| `every` / `some` | Thin names over HOF machinery; keep `all_of` / `any_of` as first-class |
| `push` / bulk append | Script `push` + C `push_value` |
| `keys` / `values` / `entries` / `freeze` | Script helpers (object/array) |
| `at` / stack ops / `splice` | Script `at` / `pop` / `shift` / `unshift` / `splice` |

#### Array as vector / deque (C) — done on branch

- **`afw_array_setter` reshaped:** seal; `push_value`; `pop_value`/`shift_value` (+ optional `found`); `insert_value` / `set_value` / `remove_value_by_index` with **`afw_integer_t`** indexes; content `remove_value` / `remove_all_values`. C payloads go through typed `array_of_<type>_add_internal`, not a setter `void *` door.
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
| **Mid-array insert/remove O(n)** | **Vector store** | Memory arrays sit on `afw_vector` of value pointers. Index locate is O(1); mid-array insert/remove still `memmove`. Ends (`push`/`pop`/`shift`/`insert 0`) stay O(1). |
| **`get_next_internal` iterator** | **Gone** | Vtable method dropped. `get_next_value` still clears the iterator to NULL at end (do not store sentinel). |
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
- [x] Large materializing retrieve no longer a default-100 landmine (**#49** `maxObjects` default 0); request memory is **`limitRequestPoolBytes`** (**#329**). Progressive retrieve remains optional (**#127**).
- [ ] Snapshot / debug APIs (e.g. **#9** `qualifier`/`qualifiers`) documented as non-hot-path and size-aware; not used as everyday data access
- [ ] User-facing behavior documented in `whats-new.md` / real docs as appropriate
- [x] `mgg-develop` merged to `develop` ([PR #179](https://github.com/afw-org/afw/pull/179); retarget checklist done); `develop` → `main` when beta-ready

---

## Done (archive short notes)

| When | Item |
|------|------|
| 2026-08-10 | Docs polish: `whats-new` links; `open-issues-status` refresh (42 open); partner try-it guidance; Features **Closure** handbook truth. |
| 2026-08-09 | **#149** runtime catalog lifetime — PRs #160–#162. **#158** graceful stop — PR #165. **#61** afwdev exceptions. |
| 2026-08-07 | **#153** UTF-8 code-point sequences. **#50** / **#89** language bugs. |
| 2026-08-06 | **#17** mutable faces — PR #150. **#39** array semantics — PR #152. |
| 2026-08-04 | Process-close: **#14**, **#15**, **#18**, **#38**, **#55**; earlier **#9**, **#90**, **#109**, **#131**, **#140**. |
| 2026-07 | **#103** file streams — PR #120 (+ #121 tests assets); closed, Jeremy notified. Details in session wrap-up above; `whats-new.md` File streams. |

---

## Changelog of this file

| Date | Note |
|------|------|
| 2026-08-14 | Dropped `open-issues-status.md`; open issues live on GitHub only. |
| 2026-08-14 | Cutover: `mgg-develop` → `develop` ([PR #179](https://github.com/afw-org/afw/pull/179)); retarget checklist done; new work off `develop`. |
| 2026-08-10 | Session wrap-up: whats-new / open-issues-status / partner try-it / Features Closure; Done archive rows for Aug closes; #55 ask table marked historical. |
| 2026-08-07 | Branch plan: cutover **retarget** checklist (`mgg-develop` → `develop` How URLs / whats-new / prose). |
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
