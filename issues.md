# Open issues triage

Snapshot of open issues on [afw-org/afw](https://github.com/afw-org/afw) to help find “where things stand.”  
Generated for work on **`mgg-develop`** (off **`develop`**). `main` is far behind and not the day-to-day base.  
Last refreshed: 2026-07-18 (59 open on GitHub; triage notes from `whats_new.md` / branch work).

Grouped by rough next-step meaning (not formal GitHub status).

## Close? — likely done; verify and close

Work largely on `mgg-develop` / related PRs — confirm, note leftovers, then close.

| # | Title | Brief idea |
|--:|:------|:-----------|
| 110 | [Segfault: embedded props in `on*`](https://github.com/afw-org/afw/issues/110) | Clone-on-return fix on `develop`; regression tests on `mgg-develop` (#118 / `whats_new`). Verify under valgrind and close or file leftovers. |
| 109 | [Script-only adapters (no dummy map)](https://github.com/afw-org/afw/issues/109) | Optional `mappedAdapterId` for pure-script models landed (#119 / `whats_new`). Confirm docs + close, or keep open for a dedicated script-adapter type. |
| 103 | [Finish/remove stream functions](https://github.com/afw-org/afw/issues/103) | Stream / `open_file` / `rootFilePaths` finished on `mgg-develop` (#120). Verify, update issue, close. |
| 97 | [Compile-time `#{…}` substitutions](https://github.com/afw-org/afw/issues/97) | Feature largely in (#100); docs, tests, and backtick `\#`/`\$` escapes completed on `mgg-develop` (`whats_new`). Verify and close. |
| 79 | [VFS empty file (0 bytes)](https://github.com/afw-org/afw/issues/79) | Fixed + hardened on issue/`mgg-develop` branch; merge/verify tests, then close (follow-ups listed in issue). |
| 71 | [Duplicate `_AdaptiveEnvironmentVariables_`](https://github.com/afw-org/afw/issues/71) | Retrieve now returns one `current`; UTF-8/hexBinary value handling on `mgg-develop` (`whats_new`). Verify and close. |
| 35 | [Closures (test262 gaps)](https://github.com/afw-org/afw/issues/35) | Scope/closure work advanced; remaining test262 skips — verify against current `develop`/`mgg-develop` and close or list gaps. |
| 30 | [Interactive afw input (libedit)](https://github.com/afw-org/afw/issues/30) | libedit line editing/history on `mgg-develop` (#117). Verify packages/docs and close. |
| 3 | [JSON Schema `$ref` mixing](https://github.com/afw-org/afw/issues/3) | Schema projection cleanup on `mgg-develop` (#116). Verify validate/editor and close. |

## Code — implementation still needed

| # | Title | Brief idea |
|--:|:------|:-----------|
| 114 | [Fiddle: close inactive unsaved tab](https://github.com/afw-org/afw/issues/114) | Closing an inactive unsaved tab closes the *active* tab instead — fix tab-id in close dialog (data-loss risk). |
| 106 | [Resolve FIXMEs in afw tests](https://github.com/afw-org/afw/issues/106) | Hunt `FIXME` in `src/afw/tests/**/*.as` and fix or convert to tracked issues. |
| 102 | [Model `on*`: `adapterTypeSpecific`](https://github.com/afw-org/afw/issues/102) | Expose `adapterTypeSpecific` to model `on*` scripts (and document). |
| 91 | [Auth mode `intermediate` for adapters](https://github.com/afw-org/afw/issues/91) | Use `intermediate` (not `user`) when adapters fetch internal/non-user-visible resources. |
| 89 | [Pass function from array → error](https://github.com/afw-org/afw/issues/89) | `bar(a[0])` hits evaluation-limit error; skipped repro in `function.as` — fix eval/convert path. |
| 86 | [Query criteria on embedded props](https://github.com/afw-org/afw/issues/86) | Dotted properties don’t resolve property types / `allowQuery`; add extended property-type get. |
| 80 | [Auth handler script editor UI](https://github.com/afw-org/afw/issues/80) | Admin UI closes when editing authorizationHandler script source. |
| 70 | [JSON ↔ Adaptive specials](https://github.com/afw-org/afw/issues/70) | Map Infinity/NaN/undefined/integer vs double (and document rules). |
| 69 | [Data types `json` / `relaxed_json`](https://github.com/afw-org/afw/issues/69) | Source-holding types with `compile` etc., like `script`/`template` (labeled in development). |
| 63 | [Remove `expression` & `hybrid`](https://github.com/afw-org/afw/issues/63) | Drop diminished data types from core, app, tests, docs. |
| 61 | [afwdev Exception subclasses](https://github.com/afw-org/afw/issues/61) | Distinguish Python vs afw errors in test runner via typed exceptions + stored error object. |
| 60 | [Fiddle: output per input tab](https://github.com/afw-org/afw/issues/60) | Redesign so output follows the active input tab. |
| 59 | [Fiddle: resize doesn’t update output](https://github.com/afw-org/afw/issues/59) | Output pane ignores browser resize. |
| 54 | [Replace deprecated variable sets](https://github.com/afw-org/afw/issues/54) | `afw_adaptor_impl_index.c`: move off deprecated xctx variable set to `current::` / custom. |
| 52 | [Interface impl `SELF_T` macro](https://github.com/afw-org/afw/issues/52) | Generate `AFW_*_SELF_T` so impl methods take `self` without cast assign; migrate impls. |
| 50 | [`return` inside `switch`](https://github.com/afw-org/afw/issues/50) | `return` in switch doesn’t leave the function; unskip `switch-return-fn` when fixed. |
| 48 | [Rename data type `list` → `array`](https://github.com/afw-org/afw/issues/48) | Finish rename leftovers (JS tests, remaining “list” refs); labeled in development. |
| 44 | [Fix Docs Action → GH Pages](https://github.com/afw-org/afw/issues/44) | Docs deploy workflow currently broken. |
| 28 | [Compile-time type checking](https://github.com/afw-org/afw/issues/28) | Runtime checks exist; add compile-time dataType checking (in development). |
| 18 | [`stringify()` 2nd parameter](https://github.com/afw-org/afw/issues/18) | Decide semantics beyond objects and implement replacer/space (or AFW equivalent). |
| 17 | [Immutable object literals](https://github.com/afw-org/afw/issues/17) | Mark object literals immutable at compile (like lists) and fix fallout. |

## Docs — documentation is the main remaining work

| # | Title | Brief idea |
|--:|:------|:-----------|
| 101 | [`eval` / unevaluated handling](https://github.com/afw-org/afw/issues/101) | Code largely done (merged #105); finish docs, mocks/app checks, then PR notes / close. |
| 40 | [Docs for language bindings](https://github.com/afw-org/afw/issues/40) | JS/Python binding docs empty on GH Pages; write or generate. |
| 22 | [Doc Adaptive vs ECMAScript](https://github.com/afw-org/afw/issues/22) | Finish documenting language differences. |
| 7 | [Internal afwdev docs](https://github.com/afw-org/afw/issues/7) | Doxygen-style / internal docs for Python afwdev. |
| 1 | [Doxygen cleanup](https://github.com/afw-org/afw/issues/1) | Finish doxygen cleanup on remaining files. |

## Review — design or decision before more code

| # | Title | Brief idea |
|--:|:------|:-----------|
| 85 | [Auth modes & delegation](https://github.com/afw-org/afw/issues/85) | Blunt `intermediate` workaround exists; need real model for handler install + delegated security domains. |
| 49 | [Max objects from retrieve](https://github.com/afw-org/afw/issues/49) | **Partial:** `maxObjects` on `retrieve_objects` / `with_uri` (default 100). Still open: paging/RQL, progressive product story. |
| 41 | [Review auth checks in adaptor impl](https://github.com/afw-org/afw/issues/41) | Audit remaining: criteria on query, modify entries, actionId questions. |
| 33 | [Review/change error codes](https://github.com/afw-org/afw/issues/33) | With try/catch, revisit error ids for script authors; docs + tests. |
| 127 | [Progressive retrieve release](https://github.com/afw-org/afw/issues/127) | Re-enable safe object release after to_response write (old bug workaround). |
| 2 | [Memory Management](https://github.com/afw-org/afw/issues/2) | Long-running scripts / graceful OOM — later; `maxObjects` stays useful alongside. |

## Test — tests (or unskipping) are the main remaining work

| # | Title | Brief idea |
|--:|:------|:-----------|
| 90 | [`checkIndividualObjectReadAccess`](https://github.com/afw-org/afw/issues/90) | **Wiring + tests:** optional per-object `read`; special/no_dupe always run. |
| 57 | [Tests for indexes](https://github.com/afw-org/afw/issues/57) | Write tests for core index interface (LMDB uses it). |

## Backlog — real work, not urgent / not started

| # | Title | Brief idea |
|--:|:------|:-----------|
| 108 | [Curl callback read/write](https://github.com/afw-org/afw/issues/108) | Stream large HTTP bodies via Adaptive Script callbacks instead of full in-memory buffers. |
| 87 | [More RQL operators](https://github.com/afw-org/afw/issues/87) | Implement `in`, `match`, `contains` (and decide on other nonstandard ops). |
| 81 | [afwdev install language bindings](https://github.com/afw-org/afw/issues/81) | `afwdev build --install` should build/install JS/Python bindings for local consumers. |
| 74 | [Script: `args` / secret read](https://github.com/afw-org/afw/issues/74) | `process::` (or similar) for args/env; interactive `read`/`readpass` for secrets. |
| 64 | [Stack overflow / OOM handling](https://github.com/afw-org/afw/issues/64) | Better detect/handle stack overflow, segfault, OOM than APR border-page tricks. |
| 55 | [Object/array helper methods](https://github.com/afw-org/afw/issues/55) | Add useful JS-like helpers (`keys`/`values`/`entries`, `at`/`every`/`some`, …). |
| 53 | [Generate JS mock data](https://github.com/afw-org/afw/issues/53) | Stop hand-maintaining stale `__mocks__`; generate from runtime/afwfcgi somehow. |
| 45 | [GHA: run steps only when needed](https://github.com/afw-org/afw/issues/45) | Use path filters so Actions skip irrelevant paths. |
| 39 | [Holes in list/array literals](https://github.com/afw-org/afw/issues/39) | Support sparse literals and undefined entries across list HOFs / `for-of`. |
| 38 | [Computed property names](https://github.com/afw-org/afw/issues/38) | Support `{[expr]: value}` object initializer syntax. |
| 15 | [Config file path enhancements](https://github.com/afw-org/afw/issues/15) | Resolve relative paths at create; hybrid eval; `environment::` in `afw` CLI conf. |
| 14 | [`afw --allow` + YAML block strings](https://github.com/afw-org/afw/issues/14) | Content-type for value output; YAML block style for multiline strings. |
| 13 | [afwdev stress testing](https://github.com/afw-org/afw/issues/13) | `--rounds` / `--continuous` (and later stats / special envs). |
| 9 | [List qualified variables](https://github.com/afw-org/afw/issues/9) | API/introspection so scripts can see vars available for a qualifier. |
| 8 | [Leave create-react-app](https://github.com/afw-org/afw/issues/8) | Migrate admin app build (e.g. Vite). |
| 6 | [CMake/CPack packages](https://github.com/afw-org/afw/issues/6) | Prefer CPack over custom Actions+Docker packaging. |

## Meta — tracking / umbrella issues

| # | Title | Brief idea |
|--:|:------|:-----------|
| 62 | [Adaptive Script language changes](https://github.com/afw-org/afw/issues/62) | Umbrella: statement results, multi `let`/`const`, `for` init, `=` as expr, loop labels. |
| 2 | [Memory management](https://github.com/afw-org/afw/issues/2) | Long-running script lifetimes; substantial prior work — track remaining managed escape / pool polish. |

---

*Regenerate by listing open issues with `gh issue list --state open` and refreshing the tables. Not kept in sync automatically.*
