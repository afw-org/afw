# Open GitHub issues — status snapshot

**Repo:** [afw-org/afw](https://github.com/afw-org/afw)  
**Branch context:** `mgg-develop` (beta readiness)  
**Generated / refreshed:** 2026-08-12  
**Source:** live GitHub open issues (`gh issue list --state open`) + maintainer notes from recent `mgg-develop` work.

This is a **working tracker**, not a substitute for issue bodies or PRs. “How it stands” mixes GitHub metadata with known landings on `mgg-develop`; re-verify before closing anything.

## Legend

| Working on | Meaning |
|------------|---------|
| **Mike** | GitHub `mike000000000` |
| **Jeremy** | GitHub `JeremyGrieshop` |
| **—** | No assignee |

Assignees are from GitHub. Dual assignees mean both are listed, not necessarily active day-to-day.

## Summary counts

- **Open issues:** 41
- **Jeremy:** 29 issues (as assignee, including shared)
- **Mike:** 19 issues (as assignee, including shared)
- **—:** 5 issues (unassigned)

## All open issues

| # | Working on | Title | Brief | How it stands | Updated |
|---|------------|-------|-------|---------------|---------|
| [#2](https://github.com/afw-org/afw/issues/2) | Mike | Memory Management | In order to support long-running scripts, a new memory management system is required. | Active long-running concern. Partial α/β value/memory on mgg-develop; design pad + more open. Closures escape skips (#35) tied here. #149 catalog slice closed under this umbrella. | 2026-08-06 |
| [#6](https://github.com/afw-org/afw/issues/6) | Mike, Jeremy | Use CMake/CPACK for packages | This can replace much of the current Actions+Docker way of creating .rpm/.deb packages by leveraging CMake's CPACK in… | Backlog enhancement (CPack packaging). No recent activity. | 2023-04-14 |
| [#7](https://github.com/afw-org/afw/issues/7) | Jeremy | Internal documentation for afwdev | The Python code in afwdev may be able to leverage Doxyen using comment syntax compatible with Python. | Backlog docs (afwdev internal Doxygen-style). No recent activity. | 2023-04-14 |
| [#8](https://github.com/afw-org/afw/issues/8) | Jeremy | Migrate away from create-react-app | There are some unpleasant workarounds required to get the plumbing right for a create-react-app project like our curr… | Admin app build system (CRA → Vite etc.). No recent activity. | 2023-04-14 |
| [#13](https://github.com/afw-org/afw/issues/13) | Jeremy | Support stress testing in afwdev | Currently, we can use the following command to send tests to afwfcgi in parallel: | Still open for Jeremy’s knobs/stats story. Load soaks via **orchestrated firehose** leaves (`tests-extra/`, PR **#167**); **`afwdev blast` retired**. | 2026-08-11 |
| [#22](https://github.com/afw-org/afw/issues/22) | Jeremy | Finishing documenting differences between Adaptive Script and ECMAScript | Title says it all. | Jeremy’s polished ES differences doc. Maintainer notes in root typescript-differences.md (not a substitute). | 2023-07-27 |
| [#33](https://github.com/afw-org/afw/issues/33) | Mike, Jeremy | Review/Change Error Codes | Now that we can try/catch in Adaptive Script, we need to review current error codes and change any, or create new one… | Review/change error codes for try/catch. Stale; no recent activity. | 2023-07-25 |
| [#35](https://github.com/afw-org/afw/issues/35) | Mike, Jeremy | Support for closures | Here are a few tests that we expect to fail, because we do not currently implement closures like ECMAScript does: | Closures largely work (closures.as 25 pass). 11 skips are before2/after2 escape/lifetime (#2), not “no closures.” Handbook Features still says no closures (stale). Left open 2026-08-04 process-close pass. | 2026-07-22 |
| [#40](https://github.com/afw-org/afw/issues/40) | Jeremy | Documentation for Language Bindings | We need docs on the Javascript and Python bindings (they are currently empty on the GitHub pages site). Perhaps these… | Language binding docs empty on GH Pages. Open documentation. | 2023-07-28 |
| [#41](https://github.com/afw-org/afw/issues/41) | Jeremy | Review the calls to afw_authorization_check() in afw_adaptor_impl.c | Review all calls to afwauthorizationcheck() in afwadaptorimpl.c and make sure the information is passed that is neede… | Review adapter authorization_check call sites + tests. Open review. | 2026-07-19 |
| [#44](https://github.com/afw-org/afw/issues/44) | Jeremy | Fix "Docs" GitHub Action to auto-deploy GH Pages | This GitHub Action currently does not work. | Docs GH Action / Pages deploy broken. Open workflows bug. | 2023-08-03 |
| [#45](https://github.com/afw-org/afw/issues/45) | Jeremy | Make GitHub Action steps only run when necessary | This marketplace action could be used to create "if" conditions on file path changes in order to avoid running Action… | Path-filter GH Actions to skip unnecessary jobs. Open workflows enhancement. | 2023-08-03 |
| [#48](https://github.com/afw-org/afw/issues/48) | Mike, Jeremy | Change data type 'list' to 'array' | Data type 'list' has taken on most of the semantics of an array so it is being renamed to 'array'. | list→array rename largely done; residual string/docs/test “list” references possible. | 2026-07-30 |
| [#49](https://github.com/afw-org/afw/issues/49) | Mike, Jeremy | Deal with max number of objects returned | Currently, we have hard-coded the maximum number of objects returned by retrieveobjects in order to not blow up the s… | Partial: materializing retrieve maxObjects default 100. Progressive write-release is **#127 closed**. | 2026-07-20 |
| [#53](https://github.com/afw-org/afw/issues/53) | Jeremy | Generate javascript "mock" data | Currently, the Javascript test code heavily uses "mock" data for simulating requests for the UI. These reside in src/… | Generate JS mock data for admin. Open. | 2023-08-17 |
| [#54](https://github.com/afw-org/afw/issues/54) | Jeremy | Add custom:: variable to replace deprecated variable sets in afw_adaptor_impl_index.c | AFW core no longer supports afwxctxscopedeprecatedvariableset(). Change these to 'current::' variables. | Partial: index filter/value current::. custom:: multi-layer redesign deferred. LMDB index_create residuals → #57. | 2026-08-09 |
| [#57](https://github.com/afw-org/afw/issues/57) | Jeremy | Create tests for indexes | The LMDB adaptor uses the core index interface, which needs some tests written for it. | Adapter index tests. Open (blocked on create/txn residuals). | 2023-08-19 |
| [#59](https://github.com/afw-org/afw/issues/59) | Jeremy | Fiddle output does not react to resizing browser window | Fiddle output does not react to resizing browser window | Fiddle output resize reactivity. Open UI. | 2023-08-29 |
| [#60](https://github.com/afw-org/afw/issues/60) | Jeremy | Fiddle output would be more useful if it changed when switching input tabs | This would take a re-design of how the tabbed editor and resizable pane are placed inside the fiddle page. | Fiddle output refresh on tab switch. Open UI. | 2023-08-29 |
| [#62](https://github.com/afw-org/afw/issues/62) | Mike | Adaptive Script language changes | This issue and this top comment is a index of various Adaptive Script language changes that are needed. These changes… | Umbrella Adaptive Script language changes. Meta/backlog. | 2026-08-01 |
| [#63](https://github.com/afw-org/afw/issues/63) | Mike, Jeremy | Remove data types 'expression' and 'hybrid' | With the removal of data type 'expressiontuple', the ability to specify a script in a template, and since a script ca… | Remove expression/hybrid data types. Open breaking cleanup. | 2023-09-09 |
| [#64](https://github.com/afw-org/afw/issues/64) | Mike | Improve the way stack overflow, segfault, etc are handled | Especially for stack overflow, improve how they are detected and handled other than the indirect ways like setting ca… | Stack overflow / segfault handling improvements. Open. | 2023-09-06 |
| [#69](https://github.com/afw-org/afw/issues/69) | Mike | Implement data type 'json' and 'relaxed_json' | Data type 'script', 'template', and others have a 'compile' and similar functions. For consistency, there should also… | Implement json / relaxed_json data types more fully. Open (related conversion/source-holding work on tree). | 2023-09-23 |
| [#70](https://github.com/afw-org/afw/issues/70) | — | JSON <-> Adaptive Objects | JSON does not support Infinity, NaN and undefined. Also, an integer is handled differently by Adaptive Script and ECM… | JSON ↔ Adaptive objects semantics (Infinity/NaN/undefined). Unassigned. | 2023-09-25 |
| [#74](https://github.com/afw-org/afw/issues/74) | Mike, Jeremy | Useful enhancements for Adaptive Script | While creating some Clemson scripts, the following enhancements would be nice to look into: | Partial: process:: ambient + afw_crypto. Interactive readpass / secrets polish still open. | 2026-08-09 |
| [#80](https://github.com/afw-org/afw/issues/80) | Jeremy | Service Editor UI for authorizationHandler script is buggy | When editing an authorization handler script, the UI immediately closes when you attempt to change any source. | Service Editor UI for authorizationHandler script buggy. Open admin. | 2024-01-17 |
| [#81](https://github.com/afw-org/afw/issues/81) | Jeremy | afwdev should build (and install) language bindings | Some language bindings could be delivered by their appropriate language installation mediums (npm, pip, etc.), assumi… | afwdev build/install language bindings. Open tooling. | 2024-01-17 |
| [#85](https://github.com/afw-org/afw/issues/85) | Mike, Jeremy | Review authorization modes and delegation | A quick fix was applied to src/afw/authorization/afwauthorization.c in order to address an infinite recursion by temp… | Review authorization modes and delegation. Open security review. | 2024-02-20 |
| [#86](https://github.com/afw-org/afw/issues/86) | Mike, Jeremy | Fix issue with query criteria when searching on embedded properties | Currently, afwquerycriteria.c processes query criteria and one of its features is to allow for "dotted" property name… | Query criteria on embedded properties. Open bug. | 2024-02-22 |
| [#87](https://github.com/afw-org/afw/issues/87) | Mike, Jeremy | Implement more rql operators | Currently, the following RQL operators have been left un-implemented: | RQL operators in/match/contains (+ others). Open enhancement. | 2024-02-22 |
| [#91](https://github.com/afw-org/afw/issues/91) | Mike | Use authorization mode 'intermediate' for accessing more resources adaptors need for their internal use | Currently user requests to adaptors use 'user' mode to access to resources needed internally by the adaptor. For reso… | Use intermediate auth mode for adapter-internal resource access. Open. | 2024-07-24 |
| [#101](https://github.com/afw-org/afw/issues/101) | Mike, Jeremy | Changes to how evaluate() and how unevaluated values are handled | This issue involves several changes related to compiled values and when they are evaluated plus a few function name c… | eval()/unevaluated value evaluation model changes. Open language design. | 2025-04-23 |
| [#102](https://github.com/afw-org/afw/issues/102) | Jeremy | Allow model "on" functions access to adapterTypeSpecific parameter | When implementing an "adapter" entirely using models and "on" functions/adaptive scripts, the "on" functions currentl… | Expose adapterTypeSpecific to model on* scripts. Open (docs label). | 2025-04-27 |
| [#106](https://github.com/afw-org/afw/issues/106) | — | Resolve FIXME's in afw tests | Resolve FIXME's in afw tests (source/afw/tests//.as) | Resolve FIXMEs in afw tests (test262 etc.). Unassigned long-burn. | 2025-04-26 |
| [#108](https://github.com/afw-org/afw/issues/108) | Jeremy | Support for callbacks in curl functions | Currently, the curl http functions read and write data completely in memory. This will be not be very resourceful whe… | curl adaptive functions: streaming callbacks. Open. | 2025-04-27 |
| [#114](https://github.com/afw-org/afw/issues/114) | — | [Fiddle] Closing an inactive tab with unsaved changes targets the active tab instead | To reproduce: | Fiddle: close inactive unsaved tab closes active tab. Unassigned UI bug. | 2026-07-07 |
| [#125](https://github.com/afw-org/afw/issues/125) | Mike | Review max vs maximum (and related) property naming for consistency | For consistency with almost all other Adaptive property names, maximumNumberOfParameters on AdaptiveFunction should p… | max vs maximum property naming consistency. Parked naming pass. | 2026-07-19 |
| [#126](https://github.com/afw-org/afw/issues/126) | Mike | Review and harden journal support (meta) | Meta issue for journal work: review, harden, document, and track follow-ups. Use this as the single place to note jou… | Meta: journal support review/harden/docs. Tracker. | 2026-07-19 |
| [#138](https://github.com/afw-org/afw/issues/138) | Mike, Jeremy | Meta issue: sideband meta on the wire ("_meta_"), object options, and rich type info for apps | Adaptive objects keep sideband meta (identity, paths, parents, view decorations, edit helpers, optional type-related … | Meta: _meta_ on wire, object options, rich type info. Design pad; dual-assignee. | 2026-08-01 |
| [#157](https://github.com/afw-org/afw/issues/157) | — | afwdev advanced-test: hermetic afwfcgi scenario leaves | Add advanced tests to afwdev test: marker-file leaf directories (advanced-test.yaml / .json) that spawn the installed… | **Shipped as orchestrated tests** (PR **#167**): `orchestration.yaml`, hosts afwfcgi/local, `tests-extra/`, expect-stdout. Issue may stay open for residuals; schema in `src/afw/tests-extra/SCHEMA.md`. | 2026-08-08 |
| [#170](https://github.com/afw-org/afw/issues/170) | — | Application-shared Adaptive functions (conf); polymorphic script functions | Provide a way to supply a set of Adaptive functions that all scripts in the application can use, configured with the … | **Open successor of #28.** Conf/application-shared Adaptive functions first; script polymorphic families as part of that story. Unassigned. Living body on GitHub. | 2026-08-12 |

## Clusters (for planning)

| Cluster | Issues | Notes |
|---------|--------|-------|
| Memory / long-running | [#2](https://github.com/afw-org/afw/issues/2), related [#49](https://github.com/afw-org/afw/issues/49) | Value lifetimes; [#127](https://github.com/afw-org/afw/issues/127) progressive write-release **closed**; [#35](https://github.com/afw-org/afw/issues/35) escape skips; [#149](https://github.com/afw-org/afw/issues/149) catalog slice **closed** |
| Adaptive Script language | [#35](https://github.com/afw-org/afw/issues/35), [#62](https://github.com/afw-org/afw/issues/62), [#101](https://github.com/afw-org/afw/issues/101), [#22](https://github.com/afw-org/afw/issues/22), [#170](https://github.com/afw-org/afw/issues/170) | **#28 types closed** (PR **#171**); [#170](https://github.com/afw-org/afw/issues/170) app-shared functions + script poly; [#39](https://github.com/afw-org/afw/issues/39)/[#153](https://github.com/afw-org/afw/issues/153)/[#17](https://github.com/afw-org/afw/issues/17) **closed**; closures left open for [#2](https://github.com/afw-org/afw/issues/2) |
| Built-ins / JSON | [#69](https://github.com/afw-org/afw/issues/69), [#70](https://github.com/afw-org/afw/issues/70) | stringify [#18](https://github.com/afw-org/afw/issues/18) **closed** 2026-08-04 |
| Adapters / auth / RQL | [#41](https://github.com/afw-org/afw/issues/41), [#54](https://github.com/afw-org/afw/issues/54), [#85](https://github.com/afw-org/afw/issues/85)–[#87](https://github.com/afw-org/afw/issues/87), [#91](https://github.com/afw-org/afw/issues/91), [#102](https://github.com/afw-org/afw/issues/102) | Review and feature work |
| Retrieve / limits | [#49](https://github.com/afw-org/afw/issues/49) | maxObjects partial; progressive release [#127](https://github.com/afw-org/afw/issues/127) **closed** |
| Conf / process / crypto | [#74](https://github.com/afw-org/afw/issues/74) | [#15](https://github.com/afw-org/afw/issues/15) **closed**; [#74](https://github.com/afw-org/afw/issues/74) still partial (`process::` + crypto landed, interactive read/readpass open) |
| Admin / Fiddle / UI | [#8](https://github.com/afw-org/afw/issues/8), [#59](https://github.com/afw-org/afw/issues/59), [#60](https://github.com/afw-org/afw/issues/60), [#80](https://github.com/afw-org/afw/issues/80), [#114](https://github.com/afw-org/afw/issues/114) | Jeremy / unassigned UI |
| afwdev / CI / packaging | [#6](https://github.com/afw-org/afw/issues/6), [#7](https://github.com/afw-org/afw/issues/7), [#13](https://github.com/afw-org/afw/issues/13), [#44](https://github.com/afw-org/afw/issues/44), [#45](https://github.com/afw-org/afw/issues/45), [#81](https://github.com/afw-org/afw/issues/81), [#157](https://github.com/afw-org/afw/issues/157) | [#61](https://github.com/afw-org/afw/issues/61) **closed**; orchestrated tests **#167**; blast retired; #13 knobs still open |
| Meta / wire / journal | [#126](https://github.com/afw-org/afw/issues/126), [#138](https://github.com/afw-org/afw/issues/138) | Trackers / design pads |
| Tests / rename debt | [#48](https://github.com/afw-org/afw/issues/48), [#106](https://github.com/afw-org/afw/issues/106) | list→array residuals; test262 FIXMEs |
| Hosts / process stop | — | [#158](https://github.com/afw-org/afw/issues/158) **closed** (PR #165) |

## Recently closed (context only)

Useful for “don’t restart this.” Includes process-close batch **2026-08-04** and later closes through refresh date:

| # | Title (short) | Note |
|---|---------------|------|
| [#9](https://github.com/afw-org/afw/issues/9) | Provide a way for a script to see all of the qualified variables available for a qualifier | Qualifier snapshots — closed earlier |
| [#14](https://github.com/afw-org/afw/issues/14) | Add --allow parameter to afw command and add block (pipe) character output support for YAML strings with newlines | **Closed 2026-08-04** — afw --allow + YAML; 22 yaml tests green |
| [#15](https://github.com/afw-org/afw/issues/15) | Config file path enhancements | **Closed 2026-08-04** — PR #135 conf path templates + ambient |
| [#17](https://github.com/afw-org/afw/issues/17) | Mutable faces / shared instances (object & array) | **Closed 2026-08-06** — PR #150 faces → mgg-develop |
| [#18](https://github.com/afw-org/afw/issues/18) | Second parameter of stringify() needs to be implemented | **Closed 2026-08-04** — PR #137 stringify replacer |
| [#28](https://github.com/afw-org/afw/issues/28) | Compile time type (dataType) checking | **Closed 2026-08-12** — PRs #144/#145 core; wrap-up PR **#171** |
| [#38](https://github.com/afw-org/afw/issues/38) | Support for computed property names | **Closed 2026-08-04** — PR #139 computed property names |
| [#39](https://github.com/afw-org/afw/issues/39) | Support holes in list literals | **Closed 2026-08-06** — PR #152 array semantics / dense elision |
| [#50](https://github.com/afw-org/afw/issues/50) | A return inside a switch doesn't return from a function block | **Closed 2026-08-07** — return inside switch |
| [#55](https://github.com/afw-org/afw/issues/55) | Common object and array methods that will be useful | **Closed 2026-08-04** — PR #134 object/array helpers |
| [#61](https://github.com/afw-org/afw/issues/61) | Create Exception subclasses for some 'afwdev' errors | **Closed 2026-08-09** — afwdev exception subclasses (with test harness work) |
| [#89](https://github.com/afw-org/afw/issues/89) | Passing a function as a parameter from an array seems to generate an error | **Closed 2026-08-07** — function from array as callback |
| [#90](https://github.com/afw-org/afw/issues/90) | checkIndividualObjectReadAccess configuration parameter for adaptors | checkIndividualObjectReadAccess — closed earlier |
| [#109](https://github.com/afw-org/afw/issues/109) | Improvements for creating adapters with adaptive scripts | Pure-script model adapters — closed earlier |
| [#127](https://github.com/afw-org/afw/issues/127) | Progressive retrieve should release objects after write | **Closed 2026-08-11** — release after write (to_response / to_stream / HTTP list) |
| [#131](https://github.com/afw-org/afw/issues/131) | variable_exists() says a variable doesn't exist if it does but its value is undefined | variable_exists bound vs undefined — PR #146 |
| [#140](https://github.com/afw-org/afw/issues/140) | Adaptive Script: Pattern destructuring in function parameters (and optional catch) | Param/catch Patterns — closed via #141/#142 |
| [#149](https://github.com/afw-org/afw/issues/149) | Runtime / afw adapter catalog lifetime (live maps vs materialize cost) | **Closed 2026-08-09** — PRs #160–#162 runtime catalog lifetime (under #2) |
| [#153](https://github.com/afw-org/afw/issues/153) | UTF-8 values as immutable code-point sequences (index, for-of, array consumers) | **Closed 2026-08-07** — UTF-8 code-point sequences |
| [#158](https://github.com/afw-org/afw/issues/158) | afwfcgi: honor signals for graceful shutdown | **Closed 2026-08-09** — PR #165 graceful SIGTERM/SIGINT stop |

## Process-close notes (2026-08-04)

| Issue | Decision |
|-------|----------|
| [#14](https://github.com/afw-org/afw/issues/14), [#55](https://github.com/afw-org/afw/issues/55), [#38](https://github.com/afw-org/afw/issues/38), [#18](https://github.com/afw-org/afw/issues/18) | Verified green → closed |
| [#15](https://github.com/afw-org/afw/issues/15) | Shipped on mgg-develop via PR [#135](https://github.com/afw-org/afw/pull/135); residual conf host-path inventory empty (curl `caInfo`/`caPath` are runtime options, not conf create). **Closed 2026-08-04** — reopen if more path templates wanted |
| [#35](https://github.com/afw-org/afw/issues/35) | Investigated; **leave open** — 25/36 closures tests pass; 11 skips are [#2](https://github.com/afw-org/afw/issues/2) escape/lifetime; handbook Features still wrong |

## Later closes (post process-close; do not reopen casually)

| Issue | Note |
|-------|------|
| [#17](https://github.com/afw-org/afw/issues/17) | Mutable faces — PR [#150](https://github.com/afw-org/afw/pull/150) |
| [#39](https://github.com/afw-org/afw/issues/39) | Array semantics — PR [#152](https://github.com/afw-org/afw/pull/152) |
| [#50](https://github.com/afw-org/afw/issues/50), [#89](https://github.com/afw-org/afw/issues/89) | Language bugs closed 2026-08-07 |
| [#153](https://github.com/afw-org/afw/issues/153) | UTF-8 code-point sequences |
| [#61](https://github.com/afw-org/afw/issues/61) | afwdev exception subclasses |
| [#149](https://github.com/afw-org/afw/issues/149) | Runtime catalog lifetime — PRs [#160](https://github.com/afw-org/afw/pull/160)–[#162](https://github.com/afw-org/afw/pull/162) |
| [#158](https://github.com/afw-org/afw/issues/158) | Graceful process stop — PR [#165](https://github.com/afw-org/afw/pull/165) |
| [#127](https://github.com/afw-org/afw/issues/127) | Progressive retrieve write-then-release — **closed 2026-08-11** |
| [#28](https://github.com/afw-org/afw/issues/28) | Compile-time type checking — PRs [#144](https://github.com/afw-org/afw/pull/144)/[#145](https://github.com/afw-org/afw/pull/145) core; wrap-up PR [#171](https://github.com/afw-org/afw/pull/171) (fence, FunctionSignature typeCheck, call-site formals, handbook Types). **Closed 2026-08-12** |

## How to maintain

1. Refresh from `gh issue list --state open` when planning a beta pass (or after a close batch).
2. Prefer updating **How it stands** after merges on `mgg-develop`, not only when opening issues.
3. Close GitHub issues when acceptance is met even if residual polish moves to a new ticket.
4. File name is **`open-issues-status.md`** (not `open-issues.md`).
