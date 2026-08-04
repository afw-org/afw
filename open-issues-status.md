# Open GitHub issues — status snapshot

**Repo:** [afw-org/afw](https://github.com/afw-org/afw)  
**Branch context:** `mgg-develop` (beta readiness)  
**Generated / refreshed:** 2026-08-04  
**Source:** live GitHub open issues + maintainer notes from recent `mgg-develop` work and process-close pass.

This is a **working tracker**, not a substitute for issue bodies or PRs. “How it stands” mixes GitHub metadata with known landings on `mgg-develop`; re-verify before closing anything.

## Legend

| Working on | Meaning |
|------------|---------|
| **Mike** | GitHub `mike000000000` |
| **Jeremy** | GitHub `JeremyGrieshop` |
| **—** | No assignee |

Assignees are from GitHub. Dual assignees mean both are listed, not necessarily active day-to-day.

## Summary counts

- **Open issues:** 47
- **Jeremy:** 32 issues (as assignee, including shared)
- **Mike:** 26 issues (as assignee, including shared)
- **—:** 3 issues (as assignee, including shared)

## All open issues

| # | Working on | Title | Brief | How it stands | Updated |
|---|------------|-------|-------|---------------|---------|
| [#2](https://github.com/afw-org/afw/issues/2) | Mike | Memory Management | In order to support long-running scripts, a new memory management system is required. | Active long-running concern. Partial α/β value/memory work on mgg-develop; design pad + more open. Closures escape skips (#35) also tied here. | 2026-07-22 |
| [#6](https://github.com/afw-org/afw/issues/6) | Mike, Jeremy | Use CMake/CPACK for packages | This can replace much of the current Actions+Docker way of creating .rpm/.deb packages by leveraging CMake's CPACK instead. | Backlog enhancement (CPack packaging). No recent activity. | 2023-04-14 |
| [#7](https://github.com/afw-org/afw/issues/7) | Jeremy | Internal documentation for afwdev | The Python code in afwdev may be able to leverage Doxyen using comment syntax compatible with Python. | Backlog docs (afwdev internal Doxygen-style). No recent activity. | 2023-04-14 |
| [#8](https://github.com/afw-org/afw/issues/8) | Jeremy | Migrate away from create-react-app | There are some unpleasant workarounds required to get the plumbing right for a create-react-app project like our current admin app to use package modules, and still no way to make flexible build options. Furthermore,… | Admin app build system (CRA → Vite etc.). No recent activity. | 2023-04-14 |
| [#13](https://github.com/afw-org/afw/issues/13) | Jeremy | Support stress testing in afwdev | Currently, we can use the following command to send tests to afwfcgi in parallel: | afwdev stress-test options (rounds/continuous). No recent activity. | 2023-05-09 |
| [#15](https://github.com/afw-org/afw/issues/15) | Mike, Jeremy | Config file path enhancements | 1. Resolve relative file path parameter values in conf files to full path at create time when when appropriate. The base parameter of file adaptor is an example. 2. Change these file path parameter to data type… | Partial: conf path templates, process/environment ambient on mgg-develop. Residual hybrid/path cases possible. | 2026-07-30 |
| [#17](https://github.com/afw-org/afw/issues/17) | Mike | Make object literals immutable and fix the resulting errors | Make object literals immutable and fix the resulting errors | Object literals immutable — labeled in development; not verified as fully merged. | 2026-07-30 |
| [#22](https://github.com/afw-org/afw/issues/22) | Jeremy | Finishing documenting differences between Adaptive Script and ECMAScript | Finishing documenting differences between Adaptive Script and ECMAScript | Jeremy’s polished ES differences doc. Maintainer notes in root typescript-differences.md (not a substitute). | 2023-07-27 |
| [#28](https://github.com/afw-org/afw/issues/28) | Mike | Compile time type (dataType) checking | Opt-in type syntax and type checking for Adaptive Script (Adaptive data types as leaves). Branch: issue-28. | Core shipped on mgg-develop (syntax, flags, #compile, checks, handbook Types, type_* tests green). Left open intentionally for residuals/packaging; advanced TS + optimize out of issue bar. | 2026-08-03 |
| [#33](https://github.com/afw-org/afw/issues/33) | Mike, Jeremy | Review/Change Error Codes | Now that we can try/catch in Adaptive Script, we need to review current error codes and change any, or create new ones that may be more appropriate. | Review/change error codes for try/catch. Stale; no recent activity. | 2023-07-25 |
| [#35](https://github.com/afw-org/afw/issues/35) | Mike, Jeremy | Support for closures | Here are a few tests that we expect to fail, because we do not currently implement closures like ECMAScript does: | Closures largely work (closures.as 25 pass). 11 skips are before2/after2 escape/lifetime (#2), not “no closures.” Handbook Features still says no closures (stale). Left open 2026-08-04 process-close pass. | 2026-07-22 |
| [#39](https://github.com/afw-org/afw/issues/39) | Mike | Support holes in list literals | Support holes in list literals and deal with all of the consequences of entries in lists being able to be undefined. There are a lot of functions that can traverse lists that need to deal with this properly like the… | Array literal holes + fallout across list HOFs/for-of. Labeled in development; status unclear. | 2026-07-30 |
| [#40](https://github.com/afw-org/afw/issues/40) | Jeremy | Documentation for Language Bindings | We need docs on the Javascript and Python bindings (they are currently empty on the GitHub pages site). Perhaps these can be easily generated. | Language binding docs empty on GH Pages. Open documentation. | 2023-07-28 |
| [#41](https://github.com/afw-org/afw/issues/41) | Jeremy | Review the calls to afw_authorization_check() in afw_adaptor_impl.c | Review all calls to afwauthorizationcheck() in afwadaptorimpl.c and make sure the information is passed that is needed to make a decision. Include test, especially ones that have query criteria. | Review adapter authorization_check call sites + tests. Open review. | 2026-07-19 |
| [#44](https://github.com/afw-org/afw/issues/44) | Jeremy | Fix "Docs" GitHub Action to auto-deploy GH Pages | This GitHub Action currently does not work. | Docs GH Action / Pages deploy broken. Open workflows bug. | 2023-08-03 |
| [#45](https://github.com/afw-org/afw/issues/45) | Jeremy | Make GitHub Action steps only run when necessary | This marketplace action could be used to create "if" conditions on file path changes in order to avoid running Actions and steps unnecessarily: | Path-filter GH Actions to skip unnecessary jobs. Open workflows enhancement. | 2023-08-03 |
| [#48](https://github.com/afw-org/afw/issues/48) | Mike, Jeremy | Change data type 'list' to 'array' | Data type 'list' has taken on most of the semantics of an array so it is being renamed to 'array'. | list→array rename largely done; residual string/docs/test “list” references possible. | 2026-07-30 |
| [#49](https://github.com/afw-org/afw/issues/49) | Mike, Jeremy | Deal with max number of objects returned | Currently, we have hard-coded the maximum number of objects returned by retrieveobjects in order to not blow up the server memory over calls that return large JSON objects. We need to deal with this long-term. | Partial: materializing retrieve maxObjects default 100. Progressive release is #127. | 2026-07-20 |
| [#50](https://github.com/afw-org/afw/issues/50) | Mike | A return inside a switch doesn't return from a function block | The return true; here should return true from the function, f: | return inside switch vs function — open bug (2023). | 2023-08-14 |
| [#53](https://github.com/afw-org/afw/issues/53) | Jeremy | Generate javascript "mock" data | Currently, the Javascript test code heavily uses "mock" data for simulating requests for the UI. These reside in src/afwtest/javascript/src/mocks. They often get out of date and need to be manually updated. Ideally,… | Generate JS mock data for admin. Open. | 2023-08-17 |
| [#54](https://github.com/afw-org/afw/issues/54) | Jeremy | Add custom:: variable to replace deprecated variable sets in afw_adaptor_impl_index.c | AFW core no longer supports afwxctxscopedeprecatedvariableset(). Change these to 'current::' variables. | Partial: index filter/value current::. custom:: multi-layer redesign deferred. | 2026-07-22 |
| [#57](https://github.com/afw-org/afw/issues/57) | Jeremy | Create tests for indexes | The LMDB adaptor uses the core index interface, which needs some tests written for it. | Adapter index tests. Open. | 2023-08-19 |
| [#59](https://github.com/afw-org/afw/issues/59) | Jeremy | Fiddle output does not react to resizing browser window | Fiddle output does not react to resizing browser window | Fiddle output resize reactivity. Open UI. | 2023-08-29 |
| [#60](https://github.com/afw-org/afw/issues/60) | Jeremy | Fiddle output would be more useful if it changed when switching input tabs | This would take a re-design of how the tabbed editor and resizable pane are placed inside the fiddle page. | Fiddle output refresh on tab switch. Open UI. | 2023-08-29 |
| [#61](https://github.com/afw-org/afw/issues/61) | Jeremy | Create Exception subclasses for some 'afwdev' errors | The test runner will often encounter an error that could be the result of an error that occurred in Python, or it could be an error object produced by afw. In order to differentiate between the two and get a… | afwdev exception subclasses. Open; mentioned after #28 merge notes. | 2026-08-03 |
| [#62](https://github.com/afw-org/afw/issues/62) | Mike | Adaptive Script language changes | This issue and this top comment is a index of various Adaptive Script language changes that are needed. These changes can link to new issues to track particular changes when it helps or pushes can reference this… | Umbrella Adaptive Script language changes. Meta/backlog. | 2026-08-01 |
| [#63](https://github.com/afw-org/afw/issues/63) | Mike, Jeremy | Remove data types 'expression' and 'hybrid' | With the removal of data type 'expressiontuple', the ability to specify a script in a template, and since a script can be a single expression, the need for data types 'expression' and 'hybrid' are greatly diminished. | Remove expression/hybrid data types. Open breaking cleanup. | 2023-09-09 |
| [#64](https://github.com/afw-org/afw/issues/64) | Mike | Improve the way stack overflow, segfault, etc are handled | Especially for stack overflow, improve how they are detected and handled other than the indirect ways like setting call depth limits, Handle out of memory gracefully, if possible. Apr counts on a segfault of a border… | Stack overflow / segfault handling improvements. Open. | 2023-09-06 |
| [#69](https://github.com/afw-org/afw/issues/69) | Mike | Implement data type 'json' and 'relaxed_json' | Data type 'script', 'template', and others have a 'compile' and similar functions. For consistency, there should also be a data type 'json' and 'relaxedjson' to hold their source. | Implement json / relaxed_json data types more fully. Open. | 2023-09-23 |
| [#70](https://github.com/afw-org/afw/issues/70) | — | JSON <-> Adaptive Objects | JSON does not support Infinity, NaN and undefined. Also, an integer is handled differently by Adaptive Script and ECMAScript. | JSON ↔ Adaptive objects semantics. Unassigned. | 2023-09-25 |
| [#74](https://github.com/afw-org/afw/issues/74) | Mike, Jeremy | Useful enhancements for Adaptive Script | While creating some Clemson scripts, the following enhancements would be nice to look into: | Partial: process:: ambient + afw_crypto. Interactive readpass / secrets polish still open. | 2026-08-01 |
| [#80](https://github.com/afw-org/afw/issues/80) | Jeremy | Service Editor UI for authorizationHandler script is buggy | When editing an authorization handler script, the UI immediately closes when you attempt to change any source. | Service Editor UI for authorizationHandler script buggy. Open admin. | 2024-01-17 |
| [#81](https://github.com/afw-org/afw/issues/81) | Jeremy | afwdev should build (and install) language bindings | Some language bindings could be delivered by their appropriate language installation mediums (npm, pip, etc.), assuming they are published. However, it would also be useful for afwdev build --install to build and… | afwdev build/install language bindings. Open tooling. | 2024-01-17 |
| [#85](https://github.com/afw-org/afw/issues/85) | Mike, Jeremy | Review authorization modes and delegation | A quick fix was applied to src/afw/authorization/afwauthorization.c in order to address an infinite recursion by temporarily entering intermediate mode during the execution of authorization handler code. | Review authorization modes and delegation. Open security review. | 2024-02-20 |
| [#86](https://github.com/afw-org/afw/issues/86) | Mike, Jeremy | Fix issue with query criteria when searching on embedded properties | Currently, afwquerycriteria.c processes query criteria and one of its features is to allow for "dotted" property names to be referenced to indicate "embedded" properties within the object. (Note this is different… | Query criteria on embedded properties. Open bug. | 2024-02-22 |
| [#87](https://github.com/afw-org/afw/issues/87) | Mike, Jeremy | Implement more rql operators | Currently, the following RQL operators have been left un-implemented: | RQL operators in/match/contains (+ others). Open enhancement. | 2024-02-22 |
| [#89](https://github.com/afw-org/afw/issues/89) | Mike, Jeremy | Passing a function as a parameter from an array seems to generate an error | As discussed, I'm adding a test that fails to demonstrate the evaluation error (set to skip for now). | Passing function from array as callback errors. Open bug (skipped test). | 2024-05-17 |
| [#91](https://github.com/afw-org/afw/issues/91) | Mike | Use authorization mode 'intermediate' for accessing more resources adaptors need for their internal use | Currently user requests to adaptors use 'user' mode to access to resources needed internally by the adaptor. For resources not directly visible to user, 'intermediate' mode is more appropriate instead in many cases… | Use intermediate auth mode for adapter-internal resource access. Open. | 2024-07-24 |
| [#101](https://github.com/afw-org/afw/issues/101) | Mike, Jeremy | Changes to how evaluate() and how unevaluated values are handled | This issue involves several changes related to compiled values and when they are evaluated plus a few function name changes and fixes to evaluation bugs. These changes should make template literals and data type… | eval()/unevaluated value evaluation model changes. Open language design. | 2025-04-23 |
| [#102](https://github.com/afw-org/afw/issues/102) | Jeremy | Allow model "on" functions access to adapterTypeSpecific parameter | When implementing an "adapter" entirely using models and "on" functions/adaptive scripts, the "on" functions currently have access to most of the parameters that get passed into the adapter functions: addobject,… | Expose adapterTypeSpecific to model on* scripts. Open (docs label). | 2025-04-27 |
| [#106](https://github.com/afw-org/afw/issues/106) | — | Resolve FIXME's in afw tests | Resolve FIXME's in afw tests (source/afw/tests//.as) | Resolve FIXMEs in afw tests (test262 etc.). Unassigned long-burn. | 2025-04-26 |
| [#108](https://github.com/afw-org/afw/issues/108) | Jeremy | Support for callbacks in curl functions | Currently, the curl http functions read and write data completely in memory. This will be not be very resourceful when requests become large, or across lots of concurrent threads. Therefore, the ability to supply a… | curl adaptive functions: streaming callbacks. Open. | 2025-04-27 |
| [#114](https://github.com/afw-org/afw/issues/114) | — | [Fiddle] Closing an inactive tab with unsaved changes targets the active tab instead | To reproduce: - Open a new tab in Fiddle and make some changes, then save the file as test.as - Open a second tab and make some changes, but don't save the file (it will be titled Untitled-N, where N is a number) | Fiddle: close inactive unsaved tab closes active tab. Unassigned UI bug (dreynol report). | 2026-07-07 |
| [#125](https://github.com/afw-org/afw/issues/125) | Mike | Review max vs maximum (and related) property naming for consistency | For consistency with almost all other Adaptive property names, maximumNumberOfParameters on AdaptiveFunction should probably be renamed to a max… prefix (e.g. maxNumberOfParameters or similar). | max vs maximum property naming consistency. Parked naming pass. | 2026-07-19 |
| [#126](https://github.com/afw-org/afw/issues/126) | Mike | Review and harden journal support (meta) | Meta issue for journal work: review, harden, document, and track follow-ups. Use this as the single place to note journal-related tasks instead of scattering one-off issues unless something needs a dedicated ticket. | Meta: journal support review/harden/docs. Tracker. | 2026-07-19 |
| [#127](https://github.com/afw-org/afw/issues/127) | Mike | Progressive retrieve should release objects after write (re-enable release) | Progressive retrieve paths (retrieveobjectstoresponse, related stream/callback paths) are intended so each object can be written and then released, instead of holding the whole result set. Today some release paths… | Re-enable progressive retrieve object release after write. Open (related #49/#2). | 2026-07-19 |
| [#138](https://github.com/afw-org/afw/issues/138) | Mike, Jeremy | Meta issue: sideband meta on the wire ("_meta_"), object options, and rich type info for apps | Adaptive objects keep sideband meta (identity, paths, parents, view decorations, edit helpers, optional type-related material) separate from normal properties. Property iteration does not walk that sideband. | Meta: _meta_ on wire, object options, rich type info. Design pad; dual-assignee. | 2026-08-01 |

## Clusters (for planning)

| Cluster | Issues | Notes |
|---------|--------|-------|
| Memory / long-running | #2, #127, related #49 | Value lifetimes, progressive release; #35 escape skips |
| Adaptive Script language | #28, #35, #39, #62, #101, #22 | Types mostly shipped (left open); closures left open for #2 |
| Built-ins / JSON | #69, #70 | stringify #18 **closed** 2026-08-04 |
| Adapters / auth / RQL | #41, #54, #85–#87, #91, #102 | Review and feature work |
| Retrieve / limits | #49, #127 | maxObjects partial; release incomplete |
| Conf / process / crypto | #15, #74 | Partial landings |
| Admin / Fiddle / UI | #8, #59, #60, #80, #114 | Jeremy / unassigned UI |
| afwdev / CI / packaging | #6, #7, #13, #44, #45, #61, #81 | Tooling and workflows |
| Meta / wire / journal | #126, #138 | Trackers / design pads |
| Tests / rename debt | #48, #106 | list→array residuals; test262 FIXMEs |

## Recently closed (context only)

Useful for “don’t restart this.” Process-close batch **2026-08-04** verified on `mgg-develop` then closed:

| # | Title (short) | Note |
|---|---------------|------|
| #9 | Qualifier snapshots | Closed earlier |
| #14 | `afw --allow` + YAML block strings | **Closed 2026-08-04** — 22 yaml tests green |
| #18 | stringify second param (replacer) | **Closed 2026-08-04** — PR #137; 18 stringify tests |
| #38 | Computed property names in object values | **Closed 2026-08-04** — PR #139; 11 tests |
| #55 | Object/array helpers | **Closed 2026-08-04** — PR #134; 65 tests |
| #90 | checkIndividualObjectReadAccess | Closed earlier |
| #109 | Pure-script model adapters | Closed earlier |
| #131 | variable_exists bound vs undefined | Closed via PR #146 |
| #140 | Param/catch Patterns | Closed via #141/#142 |

## Process-close notes (2026-08-04)

| Issue | Decision |
|-------|----------|
| #14, #55, #38, #18 | Verified green → closed |
| #35 | Investigated; **leave open** — 25/36 closures tests pass; 11 skips are #2 escape/lifetime; handbook Features still wrong |
| #28 | Investigated; **leave open** (maintainer choice) — core shipped; residuals/packaging still tracked on issue |

## How to maintain

1. Refresh from `gh issue list --state open` when planning a beta pass.
2. Prefer updating **How it stands** after merges on `mgg-develop`, not only when opening issues.
3. Close GitHub issues when acceptance is met even if residual polish moves to a new ticket.

