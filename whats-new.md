# What’s new since `before-mgg-develop-merge`

This note is for **AFW users** (script authors, model authors, operators, and people editing Adaptive object JSON). It covers what is now on **`develop`** since tag **`before-mgg-develop-merge`** — the `mgg-develop` campaign, merged via [PR #179](https://github.com/afw-org/afw/pull/179). Matching after tag: **`after-mgg-develop-merge`**.

Internal agent rules, Cursor docs, and pure test-infrastructure work are omitted unless they affect runtime or tooling you use day to day.

---

## Must change (read this first)

This `develop` line is cleaning Adaptive Script and the supported C API toward a beta/release surface, so testers make one transition. If you only have a few minutes, change the things below so existing scripts and C keep working. Then use [Highlights](#highlights) and the detail sections for everything else.

The deprecated forms that used to still run ([#172](https://github.com/afw-org/afw/issues/172)) are **removed**: `throw "…" { … }` without **`data`**, and package **`*_declare_helpers.h`**.

### Script authors

| Change this | To |
|-------------|----|
| `throw "…" { … }` | `throw "…" data { … }` (optional `id "not_found"`). A variable also named `data` is `throw "…" data data`. [Error codes](#error-codes-trycatch-and-http-issue-33) |
| `open_file` / `stream` return `-1`; `get_stream_error` | They **throw**. `get_stream_error`, `open_uri`, and `open_response` are **gone**. [File streams](#file-streams-open_file-and-friends) |
| `retrieve_objects` / `…_with_uri` with no cap | Default **`maxObjects` is 100**. Full dumps need **`maxObjects: 0`** or a progressive `retrieve_objects_to_*`. Over the limit → **`payload_too_large`**. [Retrieve](#materializing-retrieve-maxobjects-issue-49) |
| `stringify` for Adaptive-looking text (`date("…")`, …) | **`stringify` is pure JSON**. Use **`decompile`** for Adaptive compiled form. [stringify / decompile](#stringify-decompile-compiler-listing-and-binary-text) |
| `e.id` names `cast_error`, `arg_error`, `undefined`, `code`, … | `conversion_error`, `argument_error`, `undefined_value`, `coding_error`, … Some HTTP statuses changed (syntax **400**, missing adapter **404**). Prefer **`e.id`**. [Error codes](#error-codes-trycatch-and-http-issue-33) |
| `null()` / `function()` converts; `empty_array`; `(array of …)` / `(object "OT")` | Those converts are gone (use the literal / function value). **`create_array(n)`**. Type spellings are a hard cut. [Converts](#conversion-functions-type-named), [Types](#adaptive-script-types-issue-28), [Arrays](#array-semantics-issue-39) |
| `variable_exists` as “has a defined value” | It is **bound** (true when the value is **undefined**). `variable_get` default only if **not bound**. [variable_exists](#variable_exists-bound-vs-undefined-issue-131) |
| Index filter/value scripts using bare `object` | **`current::object`** (and `objectId` / `objectType` / `key`). [Index `current::`](#adapter-index-filtervalue-current-issue-54--partial) |
| Function metadata `maximumNumberOfParameters` | **`maxNumberOfParameters`**. [Rename](#maxnumberofparameters-issue-125) |
| `checkIndividualObjectReadAccess` policies that only handle `query` | Also handle action **`read`**. [Adapter auth](#adapter-getretrieve-authorization-issue-90) |

### C programmers

If you maintain **anything that links AFW outside a full in-tree rebuild** — extension **DSOs**, custom **commands**, or other binaries that load `libafw` — **rebuild and reinstall them against this AFW install**. Mixing old DSOs/commands with a new `libafw` (or the reverse) is **unsupported**.

In-tree extensions and the `afw` / `afwfcgi` commands built with the same `./afwdev build --cdev` / `--fulldev` install are fine.

| Change this | To |
|-------------|----|
| Old installed headers / mixed binaries | Rebuild once. Prune stale names under the include prefix (`afw_*_internal.h`, old `afw_function_bindings.h`, `afw_declare_helpers.h`, …). |
| `#include` of core internals or package `*_declare_helpers.h` | `#include "afw.h"`. Core **`AFW_DECLARE` / `AFW_DEFINE` / `AFW_BEGIN_DECLARES`** live in **`afw_common.h`**. Package `*_declare_helpers.h` is **not generated**. |
| Type name `afw_iterator` as the old opaque cursor | That name is the new **keyless** iterator. Legacy cursor is **`afw_iterator_old`**. [#153](#utf-8-code-point-sequences-issue-153) |
| `afw_utf8_create` / `create_copy` / `from_utf8_z` / `from_raw` | **`create` always copies** (old `create_copy`). Point without copy is **`create_no_copy`**. `from_utf8_z` → **`utf8_z_to_utf8`** (copy) or **`utf8_z_as_utf8`** (point). `from_raw` / `as_raw` → **`from_memory` / `as_memory`**. Env/request names that are not UTF-8 are **`^` + hex + `^`**, not `_NONUTF8_` + whole-name hex. [UTF-8 doors](#utf-8-create-set-and-forced_safe) |

**Details:** [libafw C API cleanup](#libafw-c-api-cleanup-release-ready-surface).

---

## Highlights

Each **Area** links to the detail section below (when there is one). Detail
sections end with [↑ Highlights](#highlights) to return here.

| Area | What changed |
|------|----------------|
| [**libafw C API cleanup**](#libafw-c-api-cleanup-release-ready-surface) | Toward a **release-ready** supported C surface: public install + implementer headers; internals off install; declare helpers **removed**; **rebuild** out-of-tree C once against this line |
| [**Object / array helpers**](#object-and-array-helpers-issue-55) ([#55](https://github.com/afw-org/afw/issues/55)) | `keys` / `values` / `entries`, `at`, `push`/`pop`/`shift`/`unshift`, `splice`, `freeze`, `every`/`some` (C array-setter reshape covered by C API rebuild rule) |
| [**Expression property names**](#expression-property-names-in-object-values-issue-38) ([#38](https://github.com/afw-org/afw/issues/38)) | Object values may use `{ [expression]: value }` (same idea as `obj[expr]` get/set) |
| [**Qualifier snapshots**](#list-active-qualified-variables-issue-9) ([#9](https://github.com/afw-org/afw/issues/9)) | **`qualifier(name)`** / **`qualifiers()`** return **fresh listable objects** (not live proxies); optional **`includeUntrusted`**; missing name → **nullish**; can be **large** |
| [**Multi-frame `::` get**](#multi-frame-get-aligned-with-snapshots) | Stacked same-name qualifiers: first **defining** frame wins (was “first matching frame only”); aligned with snapshot semantics (landed with [#15](https://github.com/afw-org/afw/issues/15) work) |
| [**Retrieve arrays**](#materializing-retrieve-maxobjects-issue-49) ([#49](https://github.com/afw-org/afw/issues/49)) | Optional **`maxObjects`** on materializing `retrieve_objects` / `…_with_uri` (default **100**, **0** = unlimited; over max → **`payload_too_large`**) |
| [**Progressive retrieve release**](#progressive-retrieve-release-issue-127) ([#127](https://github.com/afw-org/afw/issues/127)) | Write-only progressive paths **release each object after encode/flush** (`to_response` / `to_stream` / HTTP collection list) so large sets do not hold every adapter object until the request ends |
| [**Admin / JS client**](#admin--afwclient-after-the-default-of-100) | `AfwModel` sends **`maxObjects: 0`** for full metadata catalogs so admin loads after the [#49](https://github.com/afw-org/afw/issues/49) default of 100 |
| [**Adapter auth**](#adapter-getretrieve-authorization-issue-90) ([#90](https://github.com/afw-org/afw/issues/90)) | `checkIndividualObjectReadAccess` wiring fixed + tests (action **`read`** as well as **`query`**) |
| [**File streams**](#file-streams-open_file-and-friends) ([#103](https://github.com/afw-org/afw/issues/103)) | Working `open_file` with hardened `rootFilePaths`; stream errors **throw** (not `-1` / `get_stream_error`) |
| [**Conf path templates**](#conf-path-templates-issue-15) ([#15](https://github.com/afw-org/afw/issues/15)) | Path-like conf properties are **templates** at create/start; host dirs often resolved to full path; VFS `vfsMap` / LDAP `url` too |
| [**VFS adapter**](#vfs-adapter-afw_vfs) ([#79](https://github.com/afw-org/afw/issues/79)) | Empty files, safe full-file write, multi-map path rules, `maxReadBytes` |
| [**Model adapters**](#pure-script-model-adapters) ([#109](https://github.com/afw-org/afw/issues/109)) | `mappedAdapterId` is **optional** for pure-script models; `onGetObject` can `throw` … `id "not_found"` for HTTP **404** |
| [**`afw` CLI**](#interactive-afw-line-editing-and-history) | Optional interactive line editing and history ([#30](https://github.com/afw-org/afw/issues/30)); **`--allow` / `-a`** for result content type (YAML block strings, issue **[#14](https://github.com/afw-org/afw/issues/14)**) — see also [YAML / `--allow`](#afw---allow-and-yaml-value-output-issue-14) |
| [**JSON Schema**](#json-schema-for-adaptive-object-types) ([#3](https://github.com/afw-org/afw/issues/3)) | Cleaner editor schemas for Adaptive object types |
| [**Process env**](#process-environment-variables-issue-71) ([#71](https://github.com/afw-org/afw/issues/71)) | One `current` on `_AdaptiveEnvironmentVariables_` retrieve; values string if valid UTF-8 else hexBinary |
| [**`process::`**](#process-ambient-environment-and-process-issues-71--74) ([#74](https://github.com/afw-org/afw/issues/74) partial) | Ambient `args`, `programName`, `pid`, `cwd`, `afwVersion`, `startTime` at env create (with `environment::`) |
| [**`afw_crypto`**](#crypto-extension-afw_crypto-issue-74-partial) ([#74](https://github.com/afw-org/afw/issues/74) partial) | Optional extension: AES-GCM encrypt/decrypt/**seal**/**unseal**, digest/HMAC, keystore, key refs, PBKDF2; LDAP `bindParameters` recipe |
| [**Templates**](#compile-time-template-substitutions-issue-97) ([#97](https://github.com/afw-org/afw/issues/97)) | Compile-time substitution `#{…}` docs and tests; backtick `` `\#` `` / `` `\$` `` match raw templates |
| [**Adapter index `current::`**](#adapter-index-filtervalue-current-issue-54--partial) ([#54](https://github.com/afw-org/afw/issues/54) partial) | Index filter/value scripts see **`current::object`**, `objectId`, `objectType`, `key` (not bare ambient `object`) |
| [**C builders / afwdev**](#c-api-docs-and-full-package-builds-issue-1) ([#1](https://github.com/afw-org/afw/issues/1)) | Richer C API Doxygen, package **0.12.2**, `afwdev build --fulldev` |
| [**Value / memory (α/β)**](#value-lifetime--memory-management-issue-2--alphabeta) ([#2](https://github.com/afw-org/afw/issues/2)) | Permanent scalar reuse, dual-face object/array values, safer managed object value release; **`afw_pool_release` returns pool or NULL**; managed object faces pin base |
| [**`stringify` / `decompile` / listing**](#stringify-decompile-compiler-listing-and-binary-text) ([#18](https://github.com/afw-org/afw/issues/18)) | **`stringify`** pure JSON (+ replacer); **`decompile`** Adaptive compiled form; **compile listing** human tree+symbols; **`decode_to_string`** UTF-8 from octets |
| [**UTF-8 create / set / forced_safe**](#utf-8-create-set-and-forced_safe) | C doors: short **`create`/`set` copy**; **`no_copy`** points; **`forced_safe`** encodes invalid runs as `^hex^`. Env/request names use that encode (not `_NONUTF8_` + whole-name hex) |
| [**UTF-8 in JSON / Fiddle**](#utf-8-in-json-results-and-python-local-mode) | Multi-byte UTF-8 survives **`stringify`**, Fiddle results, and other JSON emitters (signed-char octet bug) |
| [**Python `Session("local")`**](#utf-8-in-json-results-and-python-local-mode) | Local FIFO client uses **binary octet** framing so large/UTF-8 responses no longer hang |
| [**Param / catch Patterns**](#function-parameter-and-catch-patterns-issue-140) ([#140](https://github.com/afw-org/afw/issues/140)) | Function/lambda params + `catch` Patterns; Expression defaults; call-site `f(...arr)`; computed/string keys; type syntax for later checking |
| [**`variable_exists` bound vs value**](#variable_exists-bound-vs-undefined-issue-131) ([#131](https://github.com/afw-org/afw/issues/131)) | `variable_exists` is **bound** (true for uninit / undefined); `variable_get` default only if **not bound**; light function briefs |
| [**Script types**](#adaptive-script-types-issue-28) ([#28](https://github.com/afw-org/afw/issues/28)) | Type annotations on Adaptive dataType leaves + shapes; opt-in `compile:typeCheck*` flags (and optional `#compile` pragma); hard cut of `(array of …)` / `(object "OT")` |
| [**Function reference prototypes**](#function-reference-prototypes-28-spelling) | Generated Adaptive function prototypes (admin Function Reference, Monaco, C Declaration comments) use **[#28](https://github.com/afw-org/afw/issues/28) Type** spelling (`T[]`, `(…) => R`); OT ids stay as `//` notes on multi-line forms |
| [**Mutable object faces**](#mutable-object-faces-issue-17) ([#17](https://github.com/afw-org/afw/issues/17)) | Literals, no clone-on-bind, adapter get/retrieve/callback, defaults, journal (incl. consumer), nested faces — drop many manual `clone()` calls (**PR [#150](https://github.com/afw-org/afw/pull/150)** → `mgg-develop`) |
| [**Array semantics**](#array-semantics-issue-39) ([#39](https://github.com/afw-org/afw/issues/39)) | Literal elision → undefined; assign-append at `length`; **`create_array(n)`**; dense arrays only (no sparse / no `in`/`delete`) |
| [**Conversion functions**](#conversion-functions-type-named) | Type-named converts; no `null()` / `function()` converts; `array` is constructor; source types hold text for `compile` |
| [**UTF-8 code-point sequences**](#utf-8-code-point-sequences-issue-153) ([#153](https://github.com/afw-org/afw/issues/153)) | Utf8-backed values as **immutable code-point sequences**: `s[i]`, for-of, array formals / HOFs; C **`afw_iterator`** redesign (legacy cursor → **`afw_iterator_old`**) |
| [**Empty-match `replace`**](#empty-match-replace-issue-190) ([#190](https://github.com/afw-org/afw/issues/190)) | Empty `match` is a **code-point boundary**. Default inserts once at the start; `limit = -1` inserts at every boundary, including the end |
| [**Orchestrated tests**](#orchestrated-tests-issue-157) ([#157](https://github.com/afw-org/afw/issues/157)) | Hermetic multi-step leaves via `orchestration.yaml` (hosts `afwfcgi` / `local`); `//? expect-stdout` / `expect-stderr`; opt-in `tests-extra/` |
| [**afwdev test recipe flags**](#afwdev-test-recipe-flags) | `-T` / `--tests-path`, `--output` / `--output-format` for machine summaries |
| [**Graceful process stop**](#graceful-process-stop-sigtermsigint-issue-158) ([#158](https://github.com/afw-org/afw/issues/158)) | **`afwfcgi`** honors **SIGTERM/SIGINT** (stop accept, drain workers, unlink Unix listen path); **`afw`** sets **`terminating`**; mid-request I/O can throw **503 Server Terminating** |
| [**Runtime catalog / accessors**](#runtime-catalog--accessors-issue-149) ([#149](https://github.com/afw-org/afw/issues/149)) | Lock+copy **`referenceCount`**; accessor registry; rich objectOptions on permanent shells fixed; **metrics/properties** live-while-active with lock-safe pointer load |
| [**Error codes**](#error-codes-trycatch-and-http-issue-33) ([#33](https://github.com/afw-org/afw/issues/33)) | Review of `e.id` / HTTP map; script `throw` … `id "not_found"` (and similar) sets the catch object and HTTP status |
| [**Multi `let` / `const`**](#multi-let-and-const-issue-62) ([#62](https://github.com/afw-org/afw/issues/62)) | Several names on one `let` / `const`; C-style `for` init; `x = y = 1;` chain; script result is set by assignment, `return`, and a call that is not void; loop labels |
| [**Compiler literals**](#compiler-literals-issue-106) ([#106](https://github.com/afw-org/afw/issues/106)) | `#doubleMax`, `#integerMax`, `#pi`, `#infinity`, and related `#` names fold to those values at compile |
| [**`maxNumberOfParameters`**](#maxnumberofparameters-issue-125) ([#125](https://github.com/afw-org/afw/issues/125)) | Function metadata property renamed from `maximumNumberOfParameters` |

---

## Orchestrated tests (issue [#157](https://github.com/afw-org/afw/issues/157))

Hermetic multi-step / multi-request tests discovered by **`orchestration.yaml`**
(or `.json`) under `src/*/tests/` (gate) and **`src/*/tests-extra/`** (opt-in).

| | |
|--|--|
| **What** | One **leaf** directory: harness starts **`afwfcgi`** or runs **`afw --local`**, drives feeds (`action` / `rest` / raw local), compares expects (including **x-afw** demux: `expect-response`, `expect-raw-*`). |
| **When it runs** | Gate leaves: normal **`afwdev test -j`**. Opt-in: **`afwdev test -T src/afw/tests-extra/…`**. Needs **PyYAML** and installed binaries on PATH. |
| **Gate examples** | `src/afw/tests/advanced/`, `src/afw_command/tests/local-mode/` |
| **Extras** | Progressive, firehose, REST soaks — `src/afw/tests-extra/` ([README](src/afw/tests-extra/README.md), [SCHEMA](src/afw/tests-extra/SCHEMA.md)) |
| **Test scripts** | `//? expect-stdout` / `expect-stderr` (and `//? key: <<< path` file values) on Adaptive test scripts |
| **How to write** | Handbook Developer Guide **Writing Tests** (Guides → Developer → Writing Tests; installed `/docs/afw/html/guide/developer/writing-tests.html`). Also [`src/afw/tests/README.md`](src/afw/tests/README.md). Pads [`designs/afwdev-advanced-test.md`](designs/afwdev-advanced-test.md) (history), [`designs/afwdev-test-recipe.md`](designs/afwdev-test-recipe.md) |
| **PR** | **[#167](https://github.com/afw-org/afw/pull/167)** → `mgg-develop` |

**Retired:** marker name **`advanced-test.yaml`** (migrated to `orchestration.yaml`);
subcommand **`afwdev blast`** (use `schedule.firehose` leaves under `tests-extra/`).
Live `--env-mode afwfcgi` still means “shared stack”; orchestrated leaves stay
**hermetic** under default mode (skipped under live `afwfcgi` env-mode).

[↑ Highlights](#highlights)

---

## Graceful process stop (SIGTERM/SIGINT, issue [#158](https://github.com/afw-org/afw/issues/158))

Operators and tooling can stop long-lived hosts without relying on SIGKILL under normal load.

| Host | Behavior |
|------|----------|
| **`afwfcgi`** | **SIGTERM** / **SIGINT**: set environment **`terminating`**, stop accepting FastCGI requests, wake request threads, join, close listen fd, **unlink Unix** `-p` path (not TCP `:<port>`). Brief note in `afwfcgi --help`. |
| **`afw`** (one-shot, interactive, **`--local`**) | Same signals set **`terminating` only** (no accept-loop wake). |
| **In-flight Adaptive work** | Long I/O / retrieve paths may throw error **`terminating`** → HTTP **503** (“Server Terminating”) via **`AFW_XCTX_THROW_IF_TERMINATING`**. |

Hermetic suite check: `src/afw/tests/advanced/afwfcgi_signal_shutdown/`. Parent still may SIGKILL after a grace period (systemd/Docker/`stop_afwfcgi`). No configurable drain timer; no SIGHUP reload.

[↑ Highlights](#highlights)

---

## Error codes, try/catch, and HTTP (issue [#33](https://github.com/afw-org/afw/issues/33))

Error codes are both what a script sees on **`catch (e)`** (`e.id`, `e.errorCode`) and the **HTTP status** of an uncaught error on `afwfcgi`.

**`general`** remains the default. Prefer **`e.id`** over numeric **`e.errorCode`** — this beta line reordered the enum (`none`, `general`, `throw` first) so numbers may not match an older build.

This beta line also renamed a few ids that collided with language names or read as jargon. HTTP status for each is unchanged:

| Was | Now | Why |
|-----|-----|-----|
| `cast_error` | `conversion_error` | These are conversion functions, not casts. A failed `integer("x")` or a failed convert in `eq` / `ne` uses this name. |
| `arg_error` | `argument_error` | Spell the word. |
| `evaluate` / `evaluation_error` | folded into `argument_error` / `undefined_value` | The leftover `evaluation_error` token was too vague. |
| `undefined` | `undefined_value` | Do not collide with the `undefined` value / data type. A required function parameter that is `undefined` now uses this id (it was `general`). |
| `code` | `coding_error` | `code` was too vague (and a throw-macro parameter name). |
| `client_time_out` | `client_timeout` | Same style as `client_closed`. |
| `objects_needed` | `coding_error` | Cache-resolve miss is an internal invariant, not a client 400. |

A failed convert-from-text of **date**, **dateTime**, **time**, **dayTimeDuration**, **yearMonthDuration**, **base64Binary**, or **hexBinary** is **`conversion_error`**. Too few or too many arguments, and a parameter of the wrong data type, are **`argument_error`**.

`open_file` of a missing file (**ENOENT**) is **`not_found`** (still with errno on `e.rv`). Other open/I/O failures stay **`general`** plus errno.

The map also includes assigned IANA HTTP statuses a server or extension may issue (201–511 that we might use, plus Adaptive extras that share a number). Prefer **`e.id`**. Script `throw` may use the request-facing names (including `gone`, `too_many_requests`, redirects). Host names such as `memory` and `coding_error` are not allowed on script `throw`.

| Situation | `e.id` | HTTP if uncaught |
|-----------|--------|------------------|
| Script `throw "…"` | `throw` | 400 |
| `assert(false)` | `assertion_failed` | 400 |
| Adaptive parse / compile error | `syntax` | **400** (was 500) |
| Missing adapter, object, or URI | `not_found` | **404** (missing adapter was 500) |
| HTTP method this resource does not allow | `method_not_allowed` | **405** (was 500) |
| Unsupported request `Content-Type` | `unsupported_content` | **415** (was 500) |
| Capability the implementation does not have (e.g. retrieve of journal entries) | `method_not_supported` | **501** (was 500) |
| Authorization deny | `denied` | 403 |
| Anything else | `general` | 500 |

Script `throw` may set **`id`** so the catch object and the HTTP status match a built-in name:

```adaptive
throw "Person not found" id "not_found";
throw "Person not found" id "not_found" data { personId: id };
```

Allowed names are the request-facing ones in the map (`not_found`, `denied`, `gone`, `too_many_requests`, redirects, and the rest of that family). Host names such as `memory` are not allowed. Omitted **`id`** is still **`throw`** (HTTP 400).

`throw "…" { … }` (a second expression with no **`data`** name) is **removed**. Write **`throw "…" data { … }`**. After the message, **`data`** is only the clause name, so a variable also named `data` is **`throw "…" data data`**.

Handbook: Language Reference **Features** (exception handling). Tests: `src/afw/tests/errors/error_codes.as`.

[↑ Highlights](#highlights)

## Multi `let` and `const` (issue [#62](https://github.com/afw-org/afw/issues/62))

One `let` or `const` statement may declare several names, separated by commas. Each binding is in the **current** block (same as writing two statements). An initializer is optional on `let` and required on every `const` binding. Types and destructuring work per name.

```adaptive
let a = 1, b = 2;
let i, j = 0;
const x = 1, y = 2;
let n: integer = 1, s: string = "x";
```

There is no trailing comma. `let a = 1, const b = 2` is two statements, not one.

A C-style **`for` initializer** can be empty, one `let`, one `const`, or assignment. It is not a mix of different kinds of declarations. `for (let i = 0, j = 1; …)` is one `let` with two names. `for (let i = 0, let j = 1; …)` is a syntax error. `for (i = 0, j = 1; …)` still assigns existing names. `for-of` is unchanged.

```adaptive
for (let i = 0, j = 1; i < 3; i = i + 1) {
    print(i + j);
}
```

You can chain assignments in one statement, right to left. That is still a statement, not an expression: `x = y = 1;` is fine; `(x = 1) > x` and `let x = y = 1` are not. Assignment inside a larger expression is almost always a typo.

```adaptive
let x;
let y;
x = y = 1;
x = y += 2;
```

The **result of a script** starts as `undefined`. A `return` or assignment sets it. Calling a function that is not void also sets it (`x = 1; abs(-3);` is `3`). `let`, `const`, `if` / `for` / `while` / `try`, and `break;` do not change it. `print()` and a script function declared `: void` do not change it either. A script that is only a call or an expression (`1 + 2`, `abs(-3);`) still has that value as its result. An empty script is `undefined`.

A **label** may precede `for`, `while`, or `do` (`outer: for (…)`). `break` / `continue` may name that label to leave or continue that loop from a nested loop or `switch`. Labels are not allowed on blocks or `if`. One label per loop.

```adaptive
outer: for (let i = 0; i < 3; i = i + 1) {
    for (let j = 0; j < 3; j = j + 1) {
        break outer;
    }
}
```

Handbook: Language Reference **Statements**. Tests: `src/afw/tests/language/script/let_const.as`, `for.as`, `assignment.as`, `script_result.as`, `void_result.as`, `labels.as`.

[↑ Highlights](#highlights)

---

## Compiler literals (issue [#106](https://github.com/afw-org/afw/issues/106))

A **compiler literal** is a `#` name for a value the compiler already knows. It is a value in an expression, not a statement and not a function call.

| Name | Value |
|------|--------|
| `#doubleMax` `#doubleMin` `#doubleMinSubnormal` `#doubleEpsilon` | IEEE double limits |
| `#integerMax` `#integerMin` | 64-bit integer limits |
| `#pi` `#e` | math constants |
| `#infinity` `#inf` | same as `Infinity` |
| `#minusInfinity` | same as `-Infinity` |
| `#nan` | same as `NaN` |

```adaptive
assert(#doubleMax / 0.9 === Infinity);
assert(#integerMax + #integerMin === -1);
assert(#infinity === Infinity);
```

Reserved words `Infinity`, `NaN`, and `INF` are unchanged. `#compile` is still a pragma. `#block` and similar names are still for decompile, not everyday script.

Handbook: Language Reference **Language Features** (Compiler literals). Tests: `src/afw/tests/compiler/compiler_literals.as`.

[↑ Highlights](#highlights)

---

## `maxNumberOfParameters` (issue [#125](https://github.com/afw-org/afw/issues/125))

On `_AdaptiveFunction_` (and polymorphic functions that inherit it), the property id is now **`maxNumberOfParameters`**. It still means the largest number of parameters, or **-1** if there is no maximum.

If a script or client read **`maximumNumberOfParameters`** from a function object on `adapterId=afw`, use the new name. Briefs and labels still say “Maximum …”.

New limit/cap property ids should use a **`max…`** prefix (`maxReadBytes`, `maxValue`, `maxObjects`).

[↑ Highlights](#highlights)

---

## afwdev test recipe flags

| Flag | Who | Role |
|------|-----|------|
| **`-T` / `--tests-path`** | `afwdev test` | Exclusive opt-in trees (e.g. `src/afw/tests-extra/…`); default `test -j` never scans those roots |
| **`--output` / `--output-format`** | `afwdev test` | Write a machine summary (`json`, `json-compact`, or `text`) to a path or `-` |

Recipes: [`designs/afwdev-test-recipe.md`](designs/afwdev-test-recipe.md).

[↑ Highlights](#highlights)

---

## Runtime catalog / accessors (issue [#149](https://github.com/afw-org/afw/issues/149))

Child of **[#2](https://github.com/afw-org/afw/issues/2)** memory work. Focused accessor / catalog lifetime slice (see GitHub **[#149](https://github.com/afw-org/afw/issues/149)**).

| | |
|--|--|
| **`referenceCount` on catalog adapter / auth handler objects** | Snapshot under the existing anchor lock |
| **`_AdaptiveRuntimeValueAccessor_`** | First-class registry objects (lock-copy / live-reference contracts) |
| **Rich objectOptions on permanent / const shells** | `metaFull+normalize` (etc.) on **`EnvironmentRegistry/current`** no longer throws **Object must have a pool** (mutable propertyTypes on the view pool) |
| **`metrics` / `properties` on `_AdaptiveAdapter_`** | Live-while-active: pointer loaded under **adapter_id_anchor_lock** (`adapter_metrics` / `adapter_properties`); not a deep snapshot of counters or conf |

If you hold Adaptive values from `/afw/…` adapter objects across **service stop**, treat **metrics** / **properties** as valid only while the instance is active (or you hold a session ref); **`referenceCount`** is a safe integer snapshot. Full-registry materialize size and long-running pool pressure remain under **[#2](https://github.com/afw-org/afw/issues/2)**.

[↑ Highlights](#highlights)

---

## UTF-8 code-point sequences (issue [#153](https://github.com/afw-org/afw/issues/153))

Adaptive values whose internal form is **`afw_utf8_t`** (`string`, `anyURI`, and other utf8-backed types) are treated as **immutable sequences of Unicode code points** (not UTF-16 code units, not raw bytes).

| Topic | Behavior |
|-------|----------|
| **`length` / `substring` / `index_of` / …** | Indexes and length are **code points** (as before; kept consistent). |
| **`s[i]`** | Read one code point as a **one-code-point string**; out of range → **undefined** (soft, like arrays). Negative indexes count from the end. **No** `s[i] = …` (immutable). |
| **`for-of`** | Walks **code points** (same element shape as `s[i]`). Also still walks **arrays**. Not plain objects. |
| **Array formals / HOFs** | When a built-in expects an **array of values** (e.g. `map` / `filter` / `reduce`) or a script formal is **`array` / `T[]` / tuple**, a utf8 value is accepted as that sequence (temporary materialize). Does **not** permanently retype the string to `array`. **XACML bag** formals still treat a scalar string as **bag-of-one**, not code-point expansion. |
| **Search** | `includes` (with optional start), `replace`, and non-empty `split` search at **code-point boundaries**. |
| **Empty-match `replace`** | An empty `match` is a match at a **code-point boundary** (including start and end). Default `limit` 1 inserts at the start; `limit = -1` inserts at every boundary. See [#190](#empty-match-replace-issue-190). |
| **Substring / replace / repeat returns** | Polymorphic string ops return **`string`**, not the input’s specialized utf8 type. |

Storage remains **valid NFC UTF-8**. The value’s **data type stays** `string` / `anyURI` / … — it is not retyped to `array`.

**C / extensions:** keyless **`afw_iterator`** + data-type `optional_initialize_iterator`; value helpers `afw_value_has_iterator` / `initialize_iterator` / `as_array_sequence`. Legacy opaque cursor type renamed **`afw_iterator_old`** (part of the [C API cleanup](#libafw-c-api-cleanup-release-ready-surface) rebuild line). Maintainer pad: [`designs/utf8-code-point-sequences.md`](designs/utf8-code-point-sequences.md). Tests: `src/afw/tests/language/script/string_code_points.as`.

Residuals (not required for this language story): lazy array **face** over utf8; shared `afw_utf8_*` index helpers; produce-type percolation on call IR (see `designs/compile-optimize-notes.md` / [#28](https://github.com/afw-org/afw/issues/28)).

[↑ Highlights](#highlights)

---

## Empty-match `replace` (issue [#190](https://github.com/afw-org/afw/issues/190))

An empty `match` in `replace(value, match, replacement, limit)` is a match at a **code-point boundary**, not “eat the next character.” The replacement is inserted at that gap; the next character is copied as usual.

| Call | Result |
|------|--------|
| `replace("abc", "", "x")` | `"xabc"` — default `limit` is **1** (insert at the start) |
| `replace("abc", "", "x", -1)` | `"xaxbxcx"` — every boundary, including the start and the end |
| `replace("", "", "x")` | `"x"` |
| `replace("😀", "", "x", -1)` | `"x😀x"` — one Unicode code point, not UTF-16 units |

This is the same family as empty-separator `split()` (one entry per code point). It is the Adaptive form of ES `replace` / `replaceAll` with an empty string, except Adaptive walks **code points**.

Tests: `src/afw/tests/language/script/string_code_points.as`, `src/afw/tests/generated/functions/replace.as`.

[↑ Highlights](#highlights)

---

## Array semantics (issue [#39](https://github.com/afw-org/afw/issues/39))

Adaptive **`array`** is a **dense** ordered sequence of values (not an object, not a sparse ES array).

| Topic | Behavior |
|-------|----------|
| **Literal elision** | `['a', ,'b']` has length **3**; the middle slot is **undefined** (same as writing `undefined`). Allowed in **script** and **`relaxed_json`**. |
| **Strict JSON** | Elision and trailing commas remain **invalid** (RFC JSON). |
| **`a[i] =`** | Replace when `0 ≤ i < length`. **Append** when `i === length`. **Error** if `i > length` (no gap fill). |
| **`create_array(n)`** | New array of length **n** filled with **undefined** (`n` from **0** to **1_000_000**). Renamed from `empty_array`. |
| **Get out of range** | Bracket `a[i]` and **`at(a, i)`** both return **undefined**. |
| **`for-of` / list HOFs** | Visit every index, including undefined slots (no ES “skip holes”). |

Not supported (by design): `for-in`, `in`, `delete`, sparse present/missing indexes. See maintainer pad [`designs/array-semantics.md`](designs/array-semantics.md). Tests: `src/afw/tests/language/script/array_semantics.as`.

[↑ Highlights](#highlights)

---

## Conversion functions (type-named)

Many Adaptive **data types** have a same-named **conversion function** `T(value)` that produces a value of type **T** (or fails with **`conversion_error`**). That error id was previously named `cast_error`. Meta types (`any`, `undefined`, `void`, …) never had one.

| Kind | Examples | Notes |
|------|----------|--------|
| **Classic convert** | `integer`, `boolean`, `double`, dates, binaries, XACML-ish stringy types, … | Shared `afw_function_execute_convert` |
| **Source-holding** | `json`, `relaxed_json`, `script`, `template`, `regexp`, `xpathExpression` | Value holds **source text** of that kind; does **not** compile. Polymorphic **`compile`** compiles it. Good property types for source fields. |
| **`string(...)`** | convert + optional multi-arg **concat** | One arg ≈ convert-to-string |
| **`object(x)`** | JSON text → object, or object unchanged | Not an object-literal constructor (`{…}`) |
| **`array(...)`** | **Constructor** from elements / spread | **Not** a conversion function |
| **`create_array(n)`** | Length-based constructor, all **undefined** | **Not** a conversion function |

**Removed** type-named converts (data types remain): **`null()`**, **`function()`** — the names clash with the null literal and function syntax; use the literal / function values instead of a convert.

Maintainer pad: [`designs/conversion-functions.md`](designs/conversion-functions.md).

[↑ Highlights](#highlights)

---

## Mutable object faces (issue [#17](https://github.com/afw-org/afw/issues/17))

> **Status:** **Landed on `mgg-develop`** via [PR #150](https://github.com/afw-org/afw/pull/150) (2026-08-06). Maintainer design pad: [`designs/issue-17-mutable-object-faces.md`](designs/issue-17-mutable-object-faces.md).

### What problem this is about

Adaptive Script often hands you an **object** or **array** that is really a **shared instance** or a **view / non-memory implementation** under the hood:

- The same **object or array literal** in a function or loop can be **one shared instance** reused across evaluations, so mutating it “sticks” the next time.
- **Binding** used to **clone** as a safety net; that clone-on-bind is **gone** for objects **and** arrays — isolation comes from **faces** on literals, defaults, and script-facing returns.
- **Defaults** on helpers such as `property_get` / `variable_get` (issue **[#110](https://github.com/afw-org/afw/issues/110)**) get a **mutable face** (not a deep clone of the whole graph).
- **Adapter get/retrieve** may return a **view** or other low-cost implementation. ECMAScript authors often expect “I got an object → I can set properties” and used to wrap in **`clone()`** by hand.

Product goal:

> When you work with an object (or array) in script, you should usually get a **mutable face that is safe for you to change**, without poisoning the next evaluation, the compile-time shared instance, or the shared base the platform still owns — and without needing `clone()` just to set a property.

Under the hood a face is a **memory wrapper** (local sets; get falls through or materializes entries; nested objects/arrays get **fresh faces** so typed `map` / index paths do not share nested instances). Authors do not need a wrap API for the happy path.

### Where you can drop manual `clone()`

| Path | Notes |
|------|--------|
| **Object / array literals** (const/let, returns, multi-call) | Platform isolates shared compile-time bags |
| **`get_object` / `get_object_with_uri`** | Mutable face over the adapter object (including views). **Exception:** `{ reconcilable: true }` keeps the entity/view for `reconcile_object` — use `clone()` if you also want a free-form mutable object. |
| **`retrieve_objects` / `retrieve_objects_with_uri`** | Each object in the result array is a face |
| **`retrieve_objects_to_callback` / `_with_uri_to_callback`** | Object passed to the callback is a face |
| **`property_get` / `variable_get` object or array defaults** | Missing/unbound default is a **face** |
| **`journal_get_*`**, **consumer** gets, **after_cursor**, **advance** | Response objects are faces |

Example: `let o = get_object(...); o.foo = 1;` — no `clone(get_object(...))` required for that mutate-on-face pattern.

`property_delete` on a face (including a script object literal) now hides the property. The first ship left unshadowed delete as a silent no-op — look-through still saw the shared base.

### `clone()` vs `freeze` vs `const` vs faces

| Tool | Meaning |
|------|---------|
| **Face (platform)** | Mutable local layer; base not poisoned; **not** a deep copy and **not** store write-through |
| **`clone()`** | Explicit **deep** independent copy of a graph (or when you still need a free-form object over reconcilable/entity paths) |
| **`freeze`** | Explicit **immutability** of a value graph (or as documented for that API) |
| **`const`** | **Binding-level** only — the name cannot be reassigned; nested properties may still be mutable unless frozen |

**Still use `clone()` when:** you want a true deep independent copy; you need a free-form mutable object while keeping reconcilable identity separate; or you need a full snapshot for other reasons.

### What this is *not*

- **`retrieve_*_to_response` / `_to_stream`** — write/encode only; no script-owned face.
- **Journal entry returns** from add/modify/replace/… — fresh memory “receipts,” not store rows.
- **Faces are not write-through** to the adapter store — persist with add / modify / replace / update.
- **YAML conf / content-type parse** stays plain objects (no parse-time faces); script isolation for YAML-backed **file** data still goes through adapter get faces when applicable.
- **Runtime / `afw` catalog** objects (environment registry, live maps) are a separate lifetime topic — see issue **[#149](https://github.com/afw-org/afw/issues/149)** (under **[#2](https://github.com/afw-org/afw/issues/2)**), not this faces feature.

### Migration / habits

- Drop redundant `clone()` around **get_object / retrieve / callback / journal get** and literal isolation paths once you confirm behavior on this tree.
- Out-of-tree C that links face APIs: same [C rebuild rule](#c-programmers). Pure script authors follow this section.

[↑ Highlights](#highlights)

---

## Function reference prototypes ([#28](https://github.com/afw-org/afw/issues/28) spelling)

Adaptive **function** docs no longer show old paren Types such as `(array of integer)` or `(object _AdaptiveJournalEntry_)`.

What you see in the **admin Function Reference** (and related generated Declaration text) is script-shaped:

```adaptive
function retrieve_objects (
    adapterId: string,
    objectType: string,
    queryCriteria?: object, // _AdaptiveQueryCriteria_
    options?: object, // _AdaptiveObjectOptions_
    adapterTypeSpecific?: object,
    maxObjects?: integer
): array;
```

- **Arrays:** `string[]`, `integer[]`, rest formals as `...values_rest: integer[]`
- **Functors:** `(...values: any) => boolean`, `(accumulator: any, value: any) => any`, …
- **ObjectType extras:** still documented as a trailing `// _Adaptive…_` note (not a script Type import)
- **Compact one-liner** (Monaco signature help): same Types with `/* OT */` mid-line where needed

This is generate/docs presentation only; Adaptive function call semantics are unchanged. Rebuild/reinstall AFW so runtime strings and admin pick up the new prototypes.

[↑ Highlights](#highlights)

---

## UTF-8 create, set, and forced_safe

C `afw_utf8_t` doors now say **who owns the little struct** and **whether `.s` is a copy**.

| Old | New |
|-----|-----|
| `afw_utf8_create_copy` | **`afw_utf8_create`** (always copy into `p`, NFC or throw) |
| `afw_utf8_create` (point if already NFC) | **`afw_utf8_create_no_copy`** (struct in `p`, `.s` stays yours; not-NFC throws) |
| `afw_utf8_from_utf8_z` | **`afw_utf8_z_to_utf8`** (copy) or **`afw_utf8_z_as_utf8`** (point) |
| `afw_utf8_from_raw` / `as_raw` | **`afw_utf8_from_memory` / `as_memory`** |
| Hand-set `.s` / `.len` | **`afw_utf8_set`** (copy into `p`) or **`set_no_copy`** (point; no `p`). From a C string: **`afw_utf8_z_set`** / **`afw_utf8_z_set_no_copy`**. |
| `afw_utf8_starts_with_z` / `ends_with_z` | **`afw_utf8_starts_with_utf8_z`** / **`ends_with_utf8_z`** |
| `afw_utf8_z_starts_with_z` | **`afw_utf8_z_starts_with`** |

`afw_memory_t` uses the same verbs (no NFC). There is no `afw_raw_t`.

**`forced_safe`** (create/set, always copy): valid UTF-8 text passes through; `^` becomes `^^`; Unicode control (Cc) and invalid UTF-8 **runs** become `^` + uppercase hex + `^`. Tab/LF/CR and other whitespace/EOL stay as text. Result is valid UTF-8, **not** promised NFC, **not** an Adaptive value. `printf` / `z_printf` use this so a bad `%s` does not throw.

**`create_property_name`**: same encode, then NFC. Used for process env and FCGI/CGI request parameter names.

`AFW_UTF8_LITERAL` is still a trusted C `"…"` initializer (no check). ASCII including `\n` is always UTF-8 NFC.

[↑ Highlights](#highlights)

---

## UTF-8 in JSON results and Python local mode

### JSON / `stringify` / Fiddle

JSON serialization treated each UTF-8 **octet** as a signed `char`. High bytes (multi-byte characters, emoji) were misclassified as control characters and emitted as broken escapes such as `\ufffffff0…`. **Fiddle**, **`stringify()`**, and **`afw -a json`** all use that path.

**Fixed:** emitters compare/pass octets as unsigned. Printable ASCII and non-ASCII UTF-8 octets pass through (RFC 8259); only U+0000–U+001F (plus `"` and `\`) are escaped. Related signed-char control checks were hardened in log output, YAML helpers, and `afw_ascii_is_control_character`.

Example (result text keeps real code points):

```adaptive
assert(stringify("🎉 egg") === "\"🎉 egg\"");
```

### Python client `Session("local")`

The Adaptive Framework local protocol length-prefixes chunks by **UTF-8 octet count**. The Python client opened the FIFO in **text** mode and used `read(n)` as character counts. After JSON began emitting real multi-byte UTF-8, large responses (for example `retrieve_objects_with_uri` with `maxObjects: 0` for full object-type catalogs) could **deadlock**: the client waited for more characters that would never arrive.

**Fixed:** binary FIFO/stdin, exact octet reads, and correct framing. Rebuild/reinstall is not required for libafw ABI; update the **`afw` Python package** (`src/afw_client/python`) if you use `Session("local")` out of tree.

[↑ Highlights](#highlights)

---

## Function parameter and catch Patterns (issue [#140](https://github.com/afw-org/afw/issues/140))

**Issue [#140](https://github.com/afw-org/afw/issues/140)** — PRs **[#141](https://github.com/afw-org/afw/pull/141)** (params + catch Patterns) and **[#142](https://github.com/afw-org/afw/pull/142)** (call-site spread, Pattern keys, catch decompile, type syntax) on `mgg-develop`.

Adaptive Script already allowed list/object **Patterns** on `let` / `const`, assignment, and `for` / `for-of` heads. The same Patterns may now appear:

- In **function and lambda parameter lists** (nested rename, holes, rest, property defaults, **computed/string keys**).
- In **`catch (…)`** bindings (e.g. `catch ({ message, data })`).
- **Call-site spread:** `f(...arr)` / `f(a, ...rest, b)` expands an array into separate arguments. The spread expression is evaluated **once** (not once to size `argv` and again to fill). Object Pattern rest (`{ [key()]: v, ...r }`) does not re-evaluate computed keys. [#181](https://github.com/afw-org/afw/issues/181)
- **Built-in as a value:** `let add1 = add; add1(2, 3)` (also a property, array slot, or argument) calls that built-in. Previously only a compile-time name like `add(2, 3)` worked.

Parameter **defaults are Expressions**. Pattern leaves and whole formals may carry **type annotations** (syntax for upcoming compile-time checking; not enforced yet). Options-style example:

```adaptive
function connect({ host, port = 443 } = { host: "localhost" }) {
    return host + ":" + string(port);
}
function sum3(a, b, c) { return a + b + c; }
assert(sum3(...[1, 2, 3]) === 6);
```

Not included: arrow functions, ES `arguments` object (use formal `...rest`). Language Reference: Function statement; Features — Exception Handling, Functions and parameters.

[↑ Highlights](#highlights)

---

## `variable_exists`: bound vs undefined (issue [#131](https://github.com/afw-org/afw/issues/131))

**Issue [#131](https://github.com/afw-org/afw/issues/131)** on `mgg-develop` (branch `issue-#131-variable-exists`).

A declared name is **bound** even when its value is `undefined` (including `let x` with no initializer and an omitted optional parameter). That matches object `property_exists` (key present with undefined still exists) and TypeScript-shaped expectations for the same syntax.

| API | Meaning |
|-----|---------|
| **`variable_exists(name)`** | True if the name is bound (lexical symbol or defined on a qualifier frame), including value undefined or null |
| **`variable_get(name, default?)`** | Value if bound; optional **default only when not bound** (not when the value is undefined) |
| **`variable_is_not_null(name)`** | Bound and value is not Adaptive **null** (undefined counts as not null) |
| **`is_defined` / `is_nullish`** | Value predicates (unchanged role) |

Lexical scope slots start as the permanent **undefined** singleton (not C “empty”); assigns of missing/nullish values store that singleton. Function reference briefs for these helpers (and related `property_*`) were clarified.

```adaptive
let a: any;
assert(variable_exists("a") === true);
assert(is_nullish(a) === true);
assert(is_nullish(variable_get("a", "D"))); /* bound: default not used */
assert(variable_get("missing", "D") === "D");
```

Maintainer notes: root `typescript-differences.md` (bound vs value).

[↑ Highlights](#highlights)

---

## Object and array helpers (issue [#55](https://github.com/afw-org/afw/issues/55))

**Issue [#55](https://github.com/afw-org/afw/issues/55)** — **closed** 2026-08-04 (landed via PR **[#134](https://github.com/afw-org/afw/pull/134)** on `mgg-develop`).

Adaptive Script gains common object/array helpers and stack/queue-style mutators. These are **Adaptive functions** registered in the environment (with optional `value->method(...)` sugar when the function is a data-type method).

**C note:** the **`afw_array_setter`** interface was reshaped (e.g. `add_value` → `push_value`, new `pop_value` / `shift_value`, signed indexes). Covered by the [C rebuild rule](#c-programmers).

### Object

| Function | Role |
|----------|------|
| **`keys(object)`** | Array of property names |
| **`values(object)`** | Array of property values |
| **`entries(object)`** | Array of `[name, value]` pairs (each pair is a two-element array) |
| **`freeze(object)`** | Make the object immutable (also works on arrays; see below) |

Also available as methods when useful, e.g. `obj->keys()`, `obj->freeze()`.

### Array

| Function | Role |
|----------|------|
| **`at(array, index)`** | Value at index; negative indexes count from the end; out of range → **undefined** |
| **`push(array, …values)`** | Append; returns the modified array |
| **`pop(array)`** | Remove last; returns value or **undefined** if empty |
| **`shift(array)`** | Remove first; returns value or **undefined** if empty |
| **`unshift(array, …values)`** | Insert at front (order preserved); returns the modified array |
| **`splice(array, startIndex, deleteCount?, …values)`** | Remove and/or insert; returns array of removed values |
| **`freeze(array)`** | Make the array immutable |

`push` / `pop` support LIFO stacks; `push` / `shift` support FIFO queues. Mutable ops require a non-immutable array (frozen targets throw).

### Higher-order (names aligned with everyday use)

| Function | Role |
|----------|------|
| **`every(predicate, array, …)`** | Same behavior as **`all_of`** (all pass) |
| **`some(predicate, array, …)`** | Same behavior as **`any_of`** (any passes) |

Prefer **`all_of` / `any_of`** when you need the multi-array / bag-style quantifier family; **`every` / `some`** are convenient names for the common single-array case.

### C array face (for extension authors)

Memory arrays keep an **O(1) `get_count`**. Setter renames and end ops (`push_value`, `pop_value` with optional `found`, etc.) are the C foundation under the script helpers. See interface docs for index and empty-pop contracts.

### Handbook

Language Reference: **Objects and Arrays**
(`src/afw/doc/reference/language/objects-and-arrays.xml`), linked from the
Language index. Function Reference pages for each function are generated from
metadata when docs are built.

[↑ Highlights](#highlights)

---

## Expression property names in object values (issue [#38](https://github.com/afw-org/afw/issues/38))

**Issue [#38](https://github.com/afw-org/afw/issues/38)** — **closed** 2026-08-04 (landed via PR **[#139](https://github.com/afw-org/afw/pull/139)** on `mgg-develop`).

In an **object value**, a property name may be an **expression in square brackets**, not only an identifier or string literal. The expression is evaluated when the object value is evaluated; the result is used as the property name (a string). This matches **`obj[expression]`** get and assignment on an existing object.

```adaptive
const col = "Customer Name";
const n = 2;
const row = {
    id: 1,
    [col]: "Ada",
    ["col " + string(n)]: 42,
    ...defaults
};
assert(row["Customer Name"] === "Ada");
assert(row["col 2"] === 42);
```

### Notes

- **Variables** remain lexical identifiers; only **property names** can be any string (via a string literal or a bracket expression).
- Pure **JSON** object text still allows only the usual JSON property-name forms (no expression brackets).
- A **literal** `_meta_: { … }` property in source still installs **sideband meta** (not a normal property). A **bracket** name that evaluates to `"_meta_"` sets a **normal** property under that name — map content types that reserve `"_meta_"` on the wire are a separate topic (issue **[#138](https://github.com/afw-org/afw/issues/138)**).
- Handbook: Language Reference **Objects and Arrays** and **Features** (working with objects).

Tests: `src/afw/tests/language/script/object_expression_names.as`.

[↑ Highlights](#highlights)

---

## Value lifetime / memory management (issue [#2](https://github.com/afw-org/afw/issues/2)) — alpha/beta

**Issue [#2](https://github.com/afw-org/afw/issues/2)** — work in progress on `develop` via branch `issue-#2` (partial land; design continues).

This is **not** a finished memory-management productization. Treat it as **alpha/beta** on the maintainer develop line: useful foundation and mostly behavior-compatible for in-tree tests, but the long-running escape / assign / scope-release story is **not** complete.

### What landed so far (high level)

- Prefer **shared permanent Adaptive values** (generated constants / `afw_v_*`) for known scalars instead of allocating fresh ones where safe (null, boolean true/false, many const_objects properties).
- **Object and array instances** more consistently expose a dual Adaptive value face (`->value`) with a lifetime-matched permanent/managed/unmanaged inf.
- **Managed object values**: container-aware `optional_release` / `clone_or_reference` paths that do **not** free an embedded dual-face header; `create_managed_object` requires a non-null object and takes a container hold.
- **`afw_pool_release`**: returns the pool if still referenced, or **NULL** if that call destroyed the pool (C API; ignore return if you do not care). Used so managed **object faces** can hold one reference on the wrapped base and drop it only when the face pool is destroyed. Unmanaged faces still borrow. Array faces remain pool-owned for now.
- Living design notes for maintainers: `designs/memory-management.md` (not user docs).

**C note:** value/pool lifetime work is part of the [C API cleanup](#libafw-c-api-cleanup-release-ready-surface) line — same rebuild rule for out-of-tree linkers.

### Not done yet (do not rely on)

- Value **create** always returning existing dual-face identity (no double-wrap) — planned next (**1d**).
- Script **assign** via `clone_or_reference` (objects/arrays as shared references; scalars typically cloned when escaping).
- Scope teardown walking each variable with value release (today: scope subpool bulk free).
- Full target model: `clone_or_reference` never returns unmanaged; managed wrappers for unmanaged containers; property promote-on-get; etc.

[↑ Highlights](#highlights)

---

## C API docs and full package builds (issue [#1](https://github.com/afw-org/afw/issues/1))

**Issue [#1](https://github.com/afw-org/afw/issues/1)** / PR **[#132](https://github.com/afw-org/afw/pull/132)** (merged to `mgg-develop`)

Most of this is for **people who build on AFW in C** (extensions, commands, hosts) or who **build the package from source**. Adaptive Script-only users can skip the Doxygen detail.

### Package version

- Base package version is **0.12.2** (`afw-package.json`).
- After generate/install, `afw` / extension version strings and Doxygen’s project number follow that value (generate rewrites `*_version_info.h` and `Doxyfile` `PROJECT_NUMBER`).

### Building from source (`afwdev`)

| Profile | Typical use |
|---------|-------------|
| **`./afwdev build --cdev`** | Day-to-day C/Python: generate + cmake + install + parallel jobs (not handbook/JS) |
| **`./afwdev build --fulldev`** | Full package **dev install**: all contexts + generate + clean + install + clang scan + parallel jobs |

`--fulldev` is short for `--all --generate --clean --install --scan` (and enables `-j` like `--cdev`).  
**`--all` alone does not run generate or install** — after a version bump, use `--fulldev` (or pass `--generate` explicitly) so binaries pick up the new version. Pass **`-j N`** if you need a job cap.

Finish / PR-shaped verify is still: `./afwdev build --fulldev` then (when you want the heavy gate) `afwdev test -j --env-mode valgrind`.

### C API documentation (Doxygen)

- Builder-oriented tree under `build/docs/doxygen/html/` after a docs or fulldev build (`./afwdev build --docs --clean -j` to force Doxygen refresh).
- **Call macros** (`afw_<interface>_<method>(…)`) are the documented C API surface; descriptions come from interface XML.
- Short developer reading path: `src/afw/doc/developer/` (also linked from the Doxygen mainpage).
- Extension srcdirs stay self-contained relative to libafw core; package headers are package-private (see [C API cleanup](#libafw-c-api-cleanup-release-ready-surface)).

[↑ Highlights](#highlights)

---

## libafw C API cleanup (release-ready surface)

**C builders / out-of-tree extensions and commands** — Adaptive Script-only readers can skip.

Goal: move libafw toward a **release-ready supported C surface** — what you may rely on when writing extensions and commands — and stop treating every generated or historical header as product API. This is intentional cleanup on the path to a clearer 1.x-style boundary, not a one-off install tweak.

**Rebuild:** same [C rebuild rule](#c-programmers) as at the top of this note.

### What “supported” means

| Use this | Not product API |
|----------|-----------------|
| `#include "afw.h"` (call API + intentional `*_impl` helpers) | Core `*_internal.h`, `afw_internal.h` |
| Generated `*_impl_declares.h` when implementing a core interface | Generated register / bindings / const-object glue (`*_function_bindings_internal.h`, `*_const_objects_internal.h`, `*_generated_internal.h`) |
| Stable core string catalog (`afw_strings.h`) | Invent-for-C string labels (`afw_strings_internal.h` / `zz__*`) |
| Core **`AFW_DECLARE` / `AFW_DEFINE` / `AFW_BEGIN_DECLARES`** in **`afw_common.h`** | Package `*_declare_helpers.h` (see below) |
| | Deprecated leftovers (`afw_declare_helpers.h` if present, `afw_log_deprecated*`, `afw_model_location.h`, `afw_array_template.h`, …) |

**Only libafw has a real public C API.** Base-repo extension and command packages are **package-private** (load the DSO / Adaptive registration). Their headers are for that package tree, not a second install surface.

### Declare macros

- Core export macros live in hand-written **`afw_common.h`** (via `afw.h` / `afw_minimal.h`). Core **no longer generates** `afw_declare_helpers.h`.
- In-tree core uses **plain C** for former “internal declare” sites.
- Per-package **`*_declare_helpers.h` is not generated**. Prefer ordinary C (`AFW_DECLARE` / `AFW_DEFINE` in `afw_common.h`). If an old install still has those headers, prune them.

### Also on this rebuild line (ABI / headers)

These landed for product reasons too, but share the **same rebuild** for C consumers:

| Work | C impact (summary) |
|------|--------------------|
| [Object / array helpers](#object-and-array-helpers-issue-55) ([#55](https://github.com/afw-org/afw/issues/55)) | `afw_array_setter` reshape |
| [Value / memory](#value-lifetime--memory-management-issue-2--alphabeta) ([#2](https://github.com/afw-org/afw/issues/2)) | Pool/value lifetime and faces |
| [UTF-8 code-point sequences](#utf-8-code-point-sequences-issue-153) ([#153](https://github.com/afw-org/afw/issues/153)) | Iterator redesign; legacy cursor → **`afw_iterator_old`** |
| [Mutable object faces](#mutable-object-faces-issue-17) ([#17](https://github.com/afw-org/afw/issues/17)) | Face/value paths if you link those APIs |

### Upgrade hygiene

CMake install does **not** delete previously installed files. After upgrading, prune stale names under your include prefix (e.g. `/usr/local/include/afw/`) such as `afw_*_internal.h`, old `afw_function_bindings.h` / `afw_const_objects.h` / `afw_generated.h`, and `afw_declare_helpers.h` if present.

In-tree monorepo builds still see full source includes at **build** time. Maintainer notes: [`designs/libafw-headers-and-api-surface.md`](designs/libafw-headers-and-api-surface.md). Related: [C builders / afwdev](#c-api-docs-and-full-package-builds-issue-1) ([#1](https://github.com/afw-org/afw/issues/1)).

[↑ Highlights](#highlights)

---

## Adapter index filter/value `current::` (issue [#54](https://github.com/afw-org/afw/issues/54)) — partial

**Issue [#54](https://github.com/afw-org/afw/issues/54)** / PR **[#130](https://github.com/afw-org/afw/pull/130)** (core eval context; LMDB create path still has known issues)

If you author index **filter** / **value** scripts (definitions stored in the LMDB adapter’s internal config via `index_create`, not normal adapter conf), use **`current::`** while they evaluate:

| Variable | Meaning |
|----------|---------|
| **`current::object`** | Object being indexed |
| **`current::objectId`** | Its object id |
| **`current::objectType`** | Its object type id |
| **`current::key`** | Index definition key |

Bare ambient **`object`** (old unqualified scope push) is **not** set. Prefer `current::object` or `variable_get("current::object")`. If **value** is omitted, the property named by **key** is indexed without a script.

**Not fully productized yet:** LMDB `index_create` persistence / retroactive scan still has pre-existing txn issues; automated index smoke remains skipped until that is fixed (see [#57](https://github.com/afw-org/afw/issues/57)). Everyday LMDB CRUD does not require indexes.

[↑ Highlights](#highlights)

---

## List active qualified variables (issue [#9](https://github.com/afw-org/afw/issues/9))

**Issue [#9](https://github.com/afw-org/afw/issues/9)** / PR **[#129](https://github.com/afw-org/afw/pull/129)**. (Multi-frame **`::` get** alignment is separate — see subsection below and issue **[#15](https://github.com/afw-org/afw/issues/15)** / PR **[#135](https://github.com/afw-org/afw/pull/135)**.)

Scripts and tools can inspect active qualified variables as ordinary objects:

```adaptive
/* Variables for one qualifier (e.g. environment::, current::, request::) */
const env = qualifier("environment");
assert(env.HOME === environment::HOME);

/* No matching stack frame → undefined (not {}) */
assert(is_nullish(qualifier("no_such_qualifier")));

/* All active qualifiers → nested objects of their variables */
const all = qualifiers();
/* all.environment, all.request, all.current, … when those frames are on the stack */
```

### Behavior

| Rule | Detail |
|------|--------|
| **Fresh object each call** | Every `qualifier()` / `qualifiers()` builds a **new** memory object from the **current** stack (not a live proxy). Mutating a snapshot does not change later calls or `qualifier::name` access. |
| **Missing qualifier** | If no **visible** stack entry matches the name, `qualifier(name)` is **nullish** (`undefined`), not an empty object. `qualifiers()` **omits** inactive names (does not invent `{}`). |
| **Hot path unchanged** | Everyday `current::objectId` / `environment::HOME` still goes through stack **`get_cb`** only. Snapshots use a separate **`contribute_cb`** path intended for debug, tools, and tests—not tight production loops. |
| **First defining frame wins** | Both **get** (`qualifier::name`) and **snapshots** walk matching visible frames **newest → older**. The first frame that **defines** a name wins for that name. Get continues while a frame’s `get_cb` returns C `NULL` (not defined here); a present value—including permanent **`afw_value_undefined`** / **`afw_value_null`**—stops the walk. Snapshots contribute the same way (most recent definition wins per property). Nested pushes are cleaned up with the stack (e.g. `AFW_TRY`). |
| **`includeUntrusted`** | Optional boolean, default **false**. **Default** matches normal `qualifier::name` visibility right now. While the xctx is **secure**, set **true** so the snapshot matches what you would see with `::` if you were **less secure** (trusted **and** untrusted frames—not untrusted-only). When already not secure, true and false are the same. |
| **Can be large** | Snapshots copy variable bags into memory objects. `environment::` / `request::` (and similar) can be **big**; `qualifiers()` nests a full snapshot per active qualifier and multiplies cost. Prefer `qualifier::name` day to day; use list functions sparingly and do not retain or rebuild large snapshots in long-running scripts. |

Object-backed qualifiers (`environment::`, `request::`, `application::`, model `current::` runtime bags, …) contribute by walking their objects. Callback-backed frames (app `current::`, model `custom::`, log, context tables) contribute their known variable sets.

### Multi-frame get aligned with snapshots

Landed with conf/process ambient work (**[#15](https://github.com/afw-org/afw/issues/15)** / PR **[#135](https://github.com/afw-org/afw/pull/135)**), not as part of the original [#9](https://github.com/afw-org/afw/issues/9) snapshot API.

Older builds stopped **`qualifier::name` get** after the **first matching qualifier frame**, even when that frame did not define the name. Stacked `current::` (log write, model, `evaluate(..., additionalUntrustedQualifiedVariables)`, …) could hide older bindings such as `current::mode`. Get now matches snapshot semantics: walk newest → older and take the first frame that **defines** the name. Present undefined must use **`afw_value_undefined`**, not C `NULL` (C `NULL` means “not on this frame”).

[↑ Highlights](#highlights)

---

## Materializing retrieve: `maxObjects` (issue [#49](https://github.com/afw-org/afw/issues/49))

**Issue [#49](https://github.com/afw-org/afw/issues/49)** (partial)

`retrieve_objects` and `retrieve_objects_with_uri` build a **full result array** in memory. To keep large dumps from exhausting the server, they now accept an optional trailing parameter:

| Parameter | Default | Meaning |
|-----------|---------|---------|
| **`maxObjects`** | **100** | Maximum objects collected into the returned array |
| | **0** | Unlimited |
| Over max | — | Throws **`payload_too_large`** (`e.id === "payload_too_large"`) |

```adaptive
/* Default max 100 — large catalogs fail with payload_too_large */
retrieve_objects("afw", "_AdaptiveObjectType_");

/* Explicit unlimited when you intentionally want the full set */
retrieve_objects("afw", "_AdaptiveObjectType_", undefined, undefined, undefined, 0);

/* Cap a filtered retrieve */
retrieve_objects("data", "Person", { filter: { op: "eq", property: "status", value: "active" } },
    undefined, undefined, 50);
```

Same idea for URI form (parameter order: `uri`, `options?`, `adapterTypeSpecific?`, **`maxObjects?`**):

```adaptive
retrieve_objects_with_uri(anyURI("/afw/_AdaptiveObjectType_/"), undefined, undefined, 0);
```

### Progressive retrieve is not capped by `maxObjects`

These APIs are for large result sets **without** materializing one array on the server:

- `retrieve_objects_to_response` / `retrieve_objects_with_uri_to_response`
- `retrieve_objects_to_stream` / `…_to_stream`
- `retrieve_objects_to_callback` / `…_to_callback`

They still use the same adapter session underneath; only the **array-building** functions enforce `maxObjects`. Safe **release after write** on progressive write paths is issue **[#127](https://github.com/afw-org/afw/issues/127)** (next section). Broader long-running memory / OOM handling is issue **[#2](https://github.com/afw-org/afw/issues/2)**.

`maxObjects` is **not** an adapter conf property and **not** RQL/client paging—those remain longer-term [#49](https://github.com/afw-org/afw/issues/49) work.

### Admin / `@afw/client` after the default of 100

Core metadata catalogs (object types, etc.) are larger than 100. Materializing retrieves used by the admin SPA (Home boot `loadObjectTypes`, Documentation Schema via `useRetrieveObjects`) therefore failed with **`payload_too_large`** until the JS client was updated.

**`AfwModel`** (`@afw/client`) now sends **`maxObjects: 0`** (unlimited) for:

- `loadObjectTypes`
- `retrieveObjects` (default; callers can still pass a positive limit)

Rebuild/install the admin app (or full JS install) and hard-refresh the browser. Progressive `retrieve_objects_to_response` (already used by the Objects browser) remains the better pattern for large **instance** data; that client story is still open under [#49](https://github.com/afw-org/afw/issues/49).

[↑ Highlights](#highlights)

---

## Progressive retrieve release (issue [#127](https://github.com/afw-org/afw/issues/127))

**Issue [#127](https://github.com/afw-org/afw/issues/127)** — **closed** when this lands on `mgg-develop`.

Write-only progressive retrieve callbacks **release each object after a successful synchronous encode/flush**, matching the long-standing `to_stream` ownership contract:

- `retrieve_objects_to_response` / `retrieve_objects_with_uri_to_response` (re-enabled release after intermediate write)
- `retrieve_objects_to_stream` / `…_to_stream` (already released; unchanged contract)
- HTTP adapter collection GET object-list writer (aligned)

**Script `to_callback`** does **not** release after the callback returns (the script may retain the object face). Materializing `retrieve_objects` still holds references in the result array by design.

No Adaptive API change for callers: do not use an object after a progressive write callback has returned unless you held your own reference. Hermetic wire proof: `afwdev test -T src/afw/tests-extra/03-progressive-to-response` (orchestrated harness from PR **[#167](https://github.com/afw-org/afw/pull/167)**).

[↑ Highlights](#highlights)

---

## Adapter get/retrieve authorization (issue [#90](https://github.com/afw-org/afw/issues/90))

**Issue [#90](https://github.com/afw-org/afw/issues/90)**

Adapter get and retrieve always perform an action **`query`** authorization check on the resource before objects are returned. When adapter conf sets:

```json
"checkIndividualObjectReadAccess": true
```

each object is also checked with action **`read`** (object body available for the decision). Default is **`false`** (query only)—useful when decisions need per-object properties only if you opt in.

Session delivery was fixed so intermediate handling (e.g. `_AdaptiveServiceConf_` typing, object-type dedupe) still runs when individual read is **off**. Previously those steps could be skipped along with the read check.

Retrieve collection resource ids use a trailing slash (e.g. `/adapterId/ObjectType/`); get uses the full object path.

[↑ Highlights](#highlights)

---

## Compile-time template substitutions (issue [#97](https://github.com/afw-org/afw/issues/97))

**Issue [#97](https://github.com/afw-org/afw/issues/97)** (feature largely landed earlier as PR **[#100](https://github.com/afw-org/afw/pull/100)**; completed on this branch)

Templates support two substitution openers:

| Syntax | When it runs | Outer script locals |
|--------|----------------|---------------------|
| `#{ Script }` | During compile (or application conf load for conf templates) | **Not** visible (isolated nested script) |
| `${ Script }` | Each time the template is evaluated | Visible when the template is part of that script |

Bare `#{…}` is also a **Value** in a script (`return #{1 + 2};`). Bare `${…}` is only valid **inside** a template, not as a bare value.

Use compile-time substitution to freeze config (including one-shot values such as a UUID or a function built once at load). Use evaluation-time substitution when the value must change per access.

### Escaping openers

The openers are the two-character sequences `#{` and `${`. A backslash before `#` or `$` emits a literal `#` or `$` so the opener is not formed:

```adaptive
return `\#{not a sub} #{'is'}`;  /* → "#{not a sub} is" */
return `\${not a sub} ${'is'}`;  /* → "${not a sub} is" */
```

This now works in **backtick template strings** the same way as in raw templates (`template(...)` / conf template compile type). Normal string escapes in backticks (`\\`, `\n`, `` \` ``, …) are unchanged.

### Documentation

Language reference **Templates and Expressions** and a short note under **Qualified Variables** describe the two forms, isolation, conf lifecycle, and escapes. Full Syntax EBNF / railroad diagrams refresh on a docs build.

[↑ Highlights](#highlights)

---

## Process environment variables (issue [#71](https://github.com/afw-org/afw/issues/71))

`retrieve_objects("afw", "_AdaptiveEnvironmentVariables_")` now returns a **single** `current` object (process environment), not two identical ones. The `environment::` qualifier is unchanged.

Property **values** from the host environment are Adaptive **string** when the bytes are valid UTF-8 (NFC), otherwise **hexBinary** (raw octets preserved). Property **names** that are not valid UTF-8 use **`forced_safe`**: valid text stays, invalid runs are `^` + uppercase hex + `^`, and a caret in a valid name becomes `^^`. A bad value no longer prevents AFW from starting.

Request CGI/FCGI-like parameters remain under `_AdaptiveRequestProperties_` / `request::` (separate from process env).

[↑ Highlights](#highlights)

---

## Process ambient: `environment::` and `process::` (issues [#71](https://github.com/afw-org/afw/issues/71) / [#74](https://github.com/afw-org/afw/issues/74))

Process environment variables and invocation info are created at **environment create** (not per host) and pushed on **every** xctx via `afw_application_internal_push_qualifiers` (called from xctx finishup):

| Qualifier | Object | Contents |
|-----------|--------|----------|
| **`environment::`** | `/afw/_AdaptiveEnvironmentVariables_/current` | Process env vars (string or hexBinary per [#71](https://github.com/afw-org/afw/issues/71)) |
| **`process::`** | `/afw/_AdaptiveProcess_/current` | Invocation / process identity (partial [#74](https://github.com/afw-org/afw/issues/74)) |

### `process::` properties

| Property | Meaning |
|----------|---------|
| **`args`** | Array of command-line argument strings (`args[0]` is the program as invoked). Use `length(process::args)` for the count. |
| **`programName`** | Base name of `args[0]` (e.g. `afw`, `afwfcgi`) |
| **`pid`** | OS process id at environment create |
| **`cwd`** | Working directory **snapshot** at environment create (not updated after `chdir`) |
| **`afwVersion`** | Linked libafw version string |
| **`startTime`** | Local dateTime when the Adaptive environment was created |

Example:

```adaptive
assert(process::programName === "afw" || length(process::programName) > 0);
assert(length(process::args) >= 1);
const home = environment::HOME;
```

**Not on `process::`:** HTTP/CGI parameters (`request::`), server metrics (`_AdaptiveServer_/current`), or a live-updating cwd.

Hosts (`afw`, `afwfcgi`, …) no longer create their own process-env object. Context type **`process`** documents these bags; **`application`** parents it for the expression builder. Path-like conf templates use `contextType: "process"`.

**Deprecated on `current::`:** `current::pid` and `current::programName` still work but prefer **`process::pid`** and **`process::programName`**. Keep **`current::mode`** and **`current::xctxUUID`** — execution context, not process identity.

### Log conf `format` / `filter` context types

Specialized log conf object types (`_AdaptiveConf_log_standard`, `_syslog`, `_event_log`) set **`contextType`** on **`format`** and **`filter`** to the matching runtime context id (`logType-standard`, `logType-syslog`, `logType-event_log`). Those context types parent **application** (and thus **process**) and document log write bags (`current::message` / `source` / `xctxUUID`, `log::`, optional `custom::`). Property meta inherits the shared definitions from `_AdaptiveConf_log` via **`parentPaths`** (use object option **`composite: true`** to see full meta).

[↑ Highlights](#highlights)

---

## Crypto extension `afw_crypto` (issue [#74](https://github.com/afw-org/afw/issues/74) partial)

Optional loadable extension **`afw_crypto`** (`libafwcrypto`, OpenSSL **libcrypto**) adds Adaptive functions for “hide values in plain sight” composition with streams, files, and process env. Load with conf `"extensions": ["afw_crypto"]`.

| Function | Purpose |
|----------|---------|
| `crypto_version_info` | OpenSSL + extension version, algorithm list |
| `crypto_digest` | SHA-256 / SHA-512 (pure; no execute access) |
| `crypto_hmac` / `crypto_hmac_verify` | HMAC-SHA-256 / HMAC-SHA-512 |
| `crypto_import_key` / `crypto_generate_key` / `crypto_export_key` / `crypto_destroy_key` | Process keystore handles |
| `crypto_encrypt` / `crypto_decrypt` | AES-GCM (auto IV; tag on algorithm object) — hard path |
| `crypto_seal` / `crypto_unseal` | AES-GCM sealed bag / unseal (object or pure JSON string) — easy path |
| `crypto_derive_key` | PBKDF2-HMAC-SHA256 (default **600000** iterations, min 100000) |

Binary args accept **base64Binary** or **hexBinary**. Keys may be raw binary, a CryptoKey (`keyId`), or a reference such as `{ "from": "environment", "name": "APP_KEY", "encoding": "base64" }` (**live `getenv`**, not ambient `environment::`) or `{ "from": "file", "path": "…" }` under **`rootFilePaths`**.

Requires `libssl-dev` / `openssl-devel` at build time. Design notes: `designs/secrets-and-afw-crypto.md`. Interactive **`readpass`** remains open for [#74](https://github.com/afw-org/afw/issues/74).

### LDAP `bindParameters` (and other object-valued conf templates)

Some conf properties (notably LDAP adapter **`bindParameters`**) are **templates** evaluated at adapter start. A template that is **only a single substitution** with no surrounding text keeps the **data type of that result** — so it can return an **object** `{ "dn", "password" }`. Any extra text or multiple substitutions forces a **string**.

Combined with `open_file` / `read*` and `crypto_decrypt`, conf can avoid a cleartext bind password in JSON: keep a **sealed blob** on disk (under `rootFilePaths`), put only a **seal key** in the process environment, and evaluate a single-substitution template that decrypts and returns the bind object. Example shape (conceptual):

```text
"bindParameters": "${
  /* single substitution → object, not string */
  const key = crypto_import_key({
      \"from\": \"environment\",
      \"name\": \"AFW_SEAL_KEY\",
      \"encoding\": \"base64\"
  }, \"AES-GCM\");
  /* read sealed iv/tag/ciphertext from file under rootFilePaths … */
  const plain = crypto_decrypt({ \"name\": \"AES-GCM\", \"iv\": …, \"tag\": … }, key, ciphertext);
  return { \"dn\": \"cn=service,…\", \"password\": decode_to_string(plain) };
}"
```

Use **`decode_to_string(binary)`** for UTF-8 passwords (not `string(binary)`, which is base64 **printable** text). Prefer **`crypto_seal` / `crypto_unseal`** for the easy bag path; **`crypto_encrypt` / `crypto_decrypt`** when you need full algorithm control. Store portable sealed JSON with **`stringify(sealed)`** (binary fields become base64 JSON strings) then `crypto_unseal(key, jsonText)`. See **`src/afw_crypto/README.md`**.

Regression coverage: `src/afw_crypto/tests/crypto/crypto_bind_parameters_template.as`, `crypto_seal_unseal.as`.

[↑ Highlights](#highlights)

---

## `stringify`, `decompile`, compiler listing, and binary text

**Issue [#18](https://github.com/afw-org/afw/issues/18)** (stringify second parameter / replacer) — **closed** 2026-08-04 (PR **[#137](https://github.com/afw-org/afw/pull/137)**; re-verified). Broader decompile/listing work remains documented here for users.

These are easy to confuse; they do different jobs:

| Path | Output |
|------|--------|
| **`stringify(value [, replacer] [, whitespace])`** | **Pure JSON** text from an **evaluated** value. Adaptive types use their `jsonPrimitive` — e.g. `base64Binary` / `hexBinary` / `date` become **JSON strings**. Optional **replacer**: function `(key, value)` (root key is empty string; return **undefined** to omit an object property; array elements become **null**) or an **array of property names** to keep. Optional third parameter is whitespace/indent. |
| **`decompile(value [, whitespace])`** | **Adaptive compiled form** as text (functional / `#implementation_id(...)` forms such as `#script_function(...)`, `#block(...)`, calls). **Not** recovery of original source layout. Useful for debugging what compile produced and for **recompile** when the text is valid Adaptive (including pragma forms). Prefer this when you used to use `stringify` for Adaptive-looking output. |
| **Compiler listing** | Human-oriented dump from **`compile<script>(source, whitespace)`** (or other `compile<*>(…, listing)`): source interleaved with a value tree, plus **`---Symbols`** block tables. Used in Fiddle to understand what compile produced. **Not** recompilable and **not** JSON. Object-expression properties are listed without evaluating them (so free variables do not break the listing). |
| **`decode_to_string(binary)`** | Interprets **octets** as **UTF-8 text** (throws if invalid). Use when the binary is really a password or other UTF-8 payload (after decrypt/unseal). |
| **`string(binary)`** | **Base64 (or hex) printable** representation of the octets — not UTF-8 of the bytes. |

**Decompile / pragma recompile (advanced)**

- Goal is **compiled-value / behavior fidelity** (`decompile` → compile again → same decompile text for covered constructs), not pretty-print of original source.
- Synthetic **`#…`** forms in decompile text (e.g. `#block`, `#assignment_target`, `#list_expression`, `#script_function`, `#template_definition`, `#switch_default`, `#statements`) are **PragmaValues** the compiler understands for round-trip.
- **`#closure_binding`** and **`#function_thunk`** are **known rejects** with clear errors (runtime-only / C-side; cannot recompile from decompile text alone).
- Maintainer detail: `designs/issue-18-decompile-status.md`.

**Migration notes**

- If you previously relied on `stringify` for Adaptive-looking forms such as `date("…")` or `base64Binary("…")`, switch those call sites to **`decompile`**.
- If you needed UTF-8 text from binary, use **`decode_to_string`**, not `stringify` or `string`.
- Sealed crypto bags can be stored with **`stringify(sealed)`** directly; you no longer need a hand-built bag of `string(iv)` / `string(tag)` / … for pure JSON (that pattern still works).
- Use **listing** to inspect compile structure in Fiddle; use **decompile** for Adaptive compiled form as text; use **stringify** for portable JSON.
- Multi-byte Unicode in **evaluated** strings now survives **`stringify`** / Fiddle JSON (see **UTF-8 in JSON results** above).

Tests: `src/afw/tests/compiler/stringify.as`, `decompile.as`, `decompile_fidelity.as`, `pragma.as`, `listing.as`.

[↑ Highlights](#highlights)

---

## Adaptive Script types (issue [#28](https://github.com/afw-org/afw/issues/28))

**Issue [#28](https://github.com/afw-org/afw/issues/28) closed** (2026-08-12) after wrap-up PR **[#171](https://github.com/afw-org/afw/pull/171)** on `mgg-develop` (core earlier via **[#144](https://github.com/afw-org/afw/pull/144)** / **[#145](https://github.com/afw-org/afw/pull/145)**). Claimed type surface is release-grade; firm will-not-do fence in `typescript-differences.md`. Successors: compile-time optimize (design pad), app-shared Adaptive functions (**[#170](https://github.com/afw-org/afw/issues/170)**).

**Type syntax** uses **Adaptive data types** as leaves (`integer`, `string`, `any`, `void`, …) plus simple structured types. Examples: `integer[]`, `string[]`, `[integer, string]`, `integer|string`, `{ host: string, port?: integer }`, `(a: integer)=>integer`, plus script-local `type` / `interface` (**not** adaptive object types / OT catalogs). Array element types use postfix **`T[]` only** (not TypeScript `Array<T>`).

Old Adaptive Type spellings such as `(array of integer)` and `(object "SomeOT")` are **removed** (hard cut). Existing scripts that used those forms need updating; plain unannotated scripts are unchanged because **checking is off by default**.

**Opt in** primarily with environment / `flag_set` flags (handbook documents these). Optional **`#compile`** pragma overrides the same flag short names for that compile unit (Pattern B):

| Flag / pragma | Effect |
|---------------|--------|
| *(default)* | Parse and store types only |
| `compile:typeCheck` | Compile-time (when known) **and** runtime checks |
| `compile:typeCheckCompileOnly` | Compile-time only (**wins** if both mode flags set) |
| `compile:noImplicitAny` | Require annotations when checking is active |
| `compile:strictNullChecks` | Stricter null/undefined |
| `compile:strict` | typeCheck + noImplicitAny + strictNullChecks |
| `#compile typeCheck;` | Full typeCheck for that compile |
| `#compile typeCheckCompileOnly;` | Compile-only for that compile |
| `#compile noTypeCheck;` | Clears type-check cluster for that unit (not noOptimize) |
| `#compile typeCheck noImplicitAny;` | Full check + require annotations |
| `#compile typeCheck strictNullChecks;` | Full check + strict null/undefined |
| `#compile strict;` | Same as `compile:strict` |

Operands match flag short names (case-sensitive). Flags are **process defaults** (snapshotted at each compile start into the unit’s policy). `#compile` overrides **only that unit’s policy** and does not change process flags for siblings or the rest of the request. Mid-unit `#compile` applies **from that statement on** for later checks in the same source (already-parsed constructs are not re-checked). If a flag seems to do nothing, the unit may have `#compile noTypeCheck`, or the flag was set **after** that unit was already compiled. See `designs/pragma-hash-design.md`.

When checking is on, assignment and script function parameters are checked for leaf data types, unions/intersections, **object/interface shapes** (required props, property types, `extends`), **array elements** and **tuple length/positions** when the value is known (for example a literal), **function param/return shapes** (script functions/closures), **Pattern** element annotations on array/object destructure, **HOF FunctionSignature** formals projected to function Types, and **calls to known Adaptive functions** and typed script callables against formals (compile path when known). **Object literals** may not include properties outside the declared type (including as arguments); spreads/computed keys skip that extra-key check. At runtime, non-literal values may still be wider (structural). Type errors name missing properties, element indexes, and decompiled expected types where possible.

Handbook: Language Reference **Types** (flags and `#compile` options), **Language Features** Pragma (short `#compile` example), **Lexical** reserved names. Tests: `type_syntax.as`, `type_check_flags.as`, `type_check.as`, `type_check_multi_unit.as`.

Pathological nesting in a type annotation or destructure pattern (`((((… integer …))))`, `[[[[… x …]]]]`) is a **syntax error** (`Type or pattern nesting is too deep`) rather than a hang or crash. Ordinary scripts are nowhere near the limit.

`type` / `interface` names must be **declared before use** ([#188](https://github.com/afw-org/afw/issues/188)), like script values — not hoisted. A type may refer to **itself** in its own body (`type Node = { next?: Node }`). An unknown name is always a compile error. Bare alias cycles (`type A = A`, or a cycle only through `|` / `&`) are a compile error.

[↑ Highlights](#highlights)

---

## Conf path templates (issue [#15](https://github.com/afw-org/afw/issues/15))

**Issue [#15](https://github.com/afw-org/afw/issues/15) closed** (2026-08-04) after PR **[#135](https://github.com/afw-org/afw/pull/135)** on `mgg-develop`. Path-like conf properties use **`template`** (not hybrid) by design. No further conf host-path residuals planned at this time; curl `caInfo`/`caPath` remain plain runtime options.

Several conf properties that hold host paths or module paths are now **`template`** (or array of template). Plain strings still work as before; substitutions such as **`environment::`** (and other ambient qualifiers available after env create) are evaluated when the conf entry is processed.

| Property | Evaluated at | Full path? |
|----------|--------------|------------|
| File adapter **`root`** | Adapter start | Yes |
| Application **`rootFilePaths`** values | Application start | Yes |
| LMDB **`env.path`** | Adapter start | Yes |
| VFS **`vfsMap`** entries | Adapter start | Yes (host dir side) |
| LDAP **`url`** | Adapter start | No (network URL) |
| **`extensionModulePaths`** entries | Application start | No (often a soname) |
| Extension conf / manifest **`modulePath`** | Extension load | No |

Examples:

```json
"rootFilePaths": {
    "data": "${environment::HOME}/afw-data/"
}

"vfsMap": [
    "docs/=${environment::DATA}/docs/"
]

"url": "ldaps://${environment::LDAP_HOST}:636"
```

`environment::` and `process::` are available from environment create on all hosts (including conf-time templates), because they are installed on the base xctx before conf is applied.

[↑ Highlights](#highlights)

---

## File streams (`open_file` and friends)

**Issue [#103](https://github.com/afw-org/afw/issues/103) / PR [#120](https://github.com/afw-org/afw/pull/120)**

Adaptive Script file I/O is finished for beta: open, read, write, flush, and close through the stream category, with safe path resolution.

### Using `open_file`

Configure logical path prefixes on the application as `rootFilePaths` (type `_AdaptiveRootFilePaths_`). Each property name is a **prefix**; its value is a host directory template evaluated and absolutized at application start.

```json
"rootFilePaths": {
  "data": "/var/afw/data",
  "includes": "/var/afw/includes"
}
```

Then in script:

```adaptive
const sn = open_file("out", "data/report.txt", "w");
write(sn, "hello");
flush(sn);
close(sn);

const sn2 = open_file("in", "data/report.txt", "r");
const s = read(sn2, 100);
close(sn2);
```

### Path rules

- A logical path must **equal** a prefix or begin with `prefix/` (no leading `/` on the logical path in the modern form: use `data/foo`, not `/data/foo`).
- Among matching prefixes, the **longest** wins (e.g. `includes` beats `include` for `includes/a.as`).
- After resolve, the host path must stay under that root; `..` and other escapes are rejected.
- The same resolution is used by `compile_from_file` and `eval_from_file`.

### Error handling (breaking change)

- `open_file` and `stream` **throw** on failure (invalid path, open failure, stream already open, unknown streamId). They no longer return `-1`.
- **`get_stream_error` was removed.** Use `try` / `catch` and the thrown `_AdaptiveError_` (`e.message`, and where applicable `e.rv` / `e.rvDecoded` for errno-based I/O failures).
- Unfinished APIs were removed from the public surface for a leaner beta: **`open_uri`**, **`open_response`**.

Modes cover text and binary (`r`, `w`, `a`, `r+`, … and `rb`, `wb`, …). `open_file` requires execute access.

[↑ Highlights](#highlights)

---

## VFS adapter (`afw_vfs`)

**Issue [#79](https://github.com/afw-org/afw/issues/79)** (empty-file read and related hardening on the issue branch)

The VFS extension maps host directories to adaptive objects
(`_AdaptiveFile_vfs`). Recent work fixes reliability bugs and aligns multi-entry
`vfsMap` resolution with the same longest-prefix / containment ideas as
`rootFilePaths` (see File streams above).

### What you can rely on

- **Empty files:** reading a 0-byte file no longer fails with a pool allocate
  error. Writing empty or shorter content replaces the whole file (no leftover
  prior bytes).
- **`data` optional on file add/replace:** omitted `data` means an empty file
  (matches the object type default). Directories still ignore `data`.
- **`maxReadBytes`:** optional conf integer; default **64 MiB**. Files larger
  than the limit fail on read. Use `0` for unlimited (not recommended on
  long-running hosts).
- **`vfsMap` multi-entry:** longest matching `objectId_prefix` wins. Host roots
  are canonicalized at adapter start; resolved paths must stay under that root.
- **`retrieve_objects` `subdirectory`:** only map entries whose prefix matches
  the subdirectory path are used; the remainder is resolved under that host
  root (the map key is not appended twice).
- **Hidden names:** names starting with `.` are omitted from directory listings
  and retrieve unless `includeHidden` is true (`get_object` supports the same
  adapter-specific flag for directory objects).
- **`isDirectory`:** `true` for directories, `false` for regular files; queryable
  in retrieve criteria.
- **Deletes:** directories are non-recursive (must be empty). Missing objects
  report **`not_found`** where applicable.

### Minimal conf example

```json
{
  "type": "adapter",
  "adapterId": "vfs",
  "adapterType": "vfs",
  "vfsMap": [ "=./" ],
  "markExecutable": [ "*.as" ],
  "maxReadBytes": 67108864
}
```

Script sketch:

```adaptive
add_object("vfs", "_AdaptiveFile_vfs", {}, "temp/work/");
add_object("vfs", "_AdaptiveFile_vfs", { data: "hello" }, "temp/work/a.txt");
const o = get_object("vfs", "_AdaptiveFile_vfs", "temp/work/a.txt");
// o.data === "hello"
delete_object("vfs", "_AdaptiveFile_vfs", "temp/work/a.txt");
delete_object("vfs", "_AdaptiveFile_vfs", "temp/work/");
```

### Not a security boundary

VFS trusts its map roots and objectIds. Pre-existing symlinks under a host root
may be followed. Prefer trusted trees and objectIds; do not expose untrusted
paths as objectIds without additional controls.

Handbook: administrative guide **Adapters → VFS**. Tests:
`src/afw_vfs/tests/vfs_test.as`, `vfs_multimap.as`, `vfs_phase4.as`.

[↑ Highlights](#highlights)

---

## Pure-script model adapters

**Issue [#109](https://github.com/afw-org/afw/issues/109) / PR [#119](https://github.com/afw-org/afw/pull/119)**

A model adapter still loads an `_AdaptiveModel_` from `modelLocationAdapterId` / `modelId`. What changed is the backend requirement:

- **Hybrid (unchanged):** set `mappedAdapterId` and map-and-forward (or fall through with `current::useDefaultProcessing` / `model_default_*`).
- **Pure-script (new):** implement every operation you use in model object-type `on*` scripts and **omit `mappedAdapterId`**. No dummy file/LMDB backend is required.

If an operation needs default processing and `mappedAdapterId` is missing, AFW fails with a clear error (implement the `on*` hook or set `mappedAdapterId`).

In scripts, `current::mappedAdapterId` is nullish when unset (it does not throw).

`modelId` and `modelLocationAdapterId` remain required for both hybrid and pure-script models.

If an `on*` script is the whole implementation of a request (for example **`onGetObject`** for a missing person), use **`throw` … `id`** so the HTTP status matches what a client expects. A plain `throw "Person not found"` is **`throw`** and HTTP **400**. This sets **`not_found`** and HTTP **404** if the error is not caught:

```adaptive
/* onGetObject */
const person = /* … look up current::objectId … */;
if (is_nullish(person)) {
    throw "Person not found" id "not_found" data {
        objectId: current::objectId
    };
}
return person;
```

The same pattern works for **`denied`** (403), **`conflict`** (409), and the other names allowed on script `throw`. See [Error codes, try/catch, and HTTP](#error-codes-trycatch-and-http-issue-33).

[↑ Highlights](#highlights)

---

## Interactive `afw`: line editing and history

**Issue [#30](https://github.com/afw-org/afw/issues/30) / PR [#117](https://github.com/afw-org/afw/pull/117)**

When you run `afw` interactively on a real terminal (no file, no pipe, not `--local` chunk mode), builds linked with **libedit** offer:

- Emacs-style line editing
- Command history stored in **`~/.afw_history`** (when `HOME` is set)

Pipes, file input, and non-TTY use are unchanged. Builds without libedit still succeed; interactive history/editing simply is not available (the usage text says so).

To enable at build time on Debian/Ubuntu:

```bash
sudo apt-get install libedit-dev
```

Other distros: `libedit-dev` / `libedit-devel` as appropriate. See `src/afw/doc/building_on_linux.md` (and the macOS build notes).

[↑ Highlights](#highlights)

---

## `afw --allow` and YAML value output (issue [#14](https://github.com/afw-org/afw/issues/14))

**Issue [#14](https://github.com/afw-org/afw/issues/14)** — **closed** 2026-08-04 (feature long on tree; regression suite on `mgg-develop`; re-verified 22 yaml tests)

**Beta hygiene (this branch):**

- `afw_yaml_to_object` (**`raw_to_object`**) fixed so **file adapters with `contentType: yaml`** work; libyaml parser always deleted.
- Plain scalars: full-string **integer vs double** (`afw_number_parse`), `true`/`false`/`null`/`~`; quoted/literal/folded always strings; partial numbers like `123foo` stay strings.
- Empty array/object emit **`[]` / `{}`**; mapping value without key errors.
- No parse-time issue [#17](https://github.com/afw-org/afw/issues/17) faces (conf/store plain; adapter/journal faces on return).

The `afw` command can print evaluated adaptive values using any registered **content type**, not only JSON:

| Option | Meaning |
|--------|---------|
| **`-a` / `--allow` *type*** | Content type for **result** output (default **`json`**) |
| **`-t` / `--type` *type*** | Content type of the **configuration** file (default **`json`**) |

Load non-core content types with **`-e`** first. For YAML that is the **`afw_yaml`** extension:

```bash
# Short content-type id
afw -e afw_yaml -a yaml -s expression -x 'object({msg: string("line1\nline2"), n: 7})'

# Media type also works
afw -e afw_yaml -a application/x-yaml -s expression -x 'integer(42)'

# Conf encoded as YAML (still a list of conf objects)
afw -e afw_yaml -t yaml -f conf.yaml -s expression -x '1'
```

### YAML encoding notes (what you see)

YAML documents start with **`---`**. Common scalar forms:

| Adaptive value | YAML output style |
|----------------|-------------------|
| String **without** newlines | JSON-quoted (`"hello"`) |
| String **with** newlines | Literal block scalar **`|`** — **`|-`** strip trailing newlines, **`|+`** keep; a leading space sets an indent indicator (e.g. `|1-`) |
| Integer | Unquoted decimal (`42`) |
| Boolean / null | `true` / `false` / `null` |

Example (object with a multiline string and an integer):

```text
---
  msg: |-
    line1
    line2
  n: 7
```

Invalid `--allow` values fail with **`Invalid --allow content-type.`**

### Tests

Permanent suite under **`src/afw_yaml/tests/`** (tags `yaml`, `content_type`):

| File | Covers |
|------|--------|
| **`yaml_allow_output.py`** | `--allow` short/media ids, block scalars, primitives, objects/arrays, `-t yaml` conf |
| **`yaml_to_object.py`** | `raw_to_object` / file adapter `contentType: yaml` get/add, non-mapping root reject, [#17](https://github.com/afw-org/afw/issues/17) face isolation on get |

```bash
afwdev test -p afw_yaml --show-all
# or
afwdev test -p afw_yaml --tags yaml
```

Handbook `usage.xml` may still omit `-a` until a docs pass; live **`afw -h`** lists it.

[↑ Highlights](#highlights)

---

## JSON Schema for Adaptive object types

**Issue [#3](https://github.com/afw-org/afw/issues/3) / PR [#116](https://github.com/afw-org/afw/pull/116)**

Package-generated schemas under `generated/schemas/afw/` are improved for **editors** (e.g. VS Code completion, hovers, light validation) and for `afwdev validate`:

- No more invalid “`$ref` mixed with sibling keywords” shapes that break validators.
- Entity schemas promote type/properties to the document root for better editor apply behavior.
- Inheritance merges parent `propertyTypes` with **child overrides** so nested models validate correctly.
- `defaultValue` / possible values map more cleanly to JSON Schema `default` / `enum`.

If you edit Adaptive object JSON under `generate/objects/` (or rely on schema-backed validation), regenerate so your tree picks up the new schemas (`./afwdev build --cdev` or your usual generate path). Do not hand-edit `generated/schemas/`.

[↑ Highlights](#highlights)

---

## Reliability notes

### Default values from `property_get` / `variable_get`

Mutable defaults for `property_get` / `variable_get` (issue **[#110](https://github.com/afw-org/afw/issues/110)**) are isolated so later mutations do not poison other calls (important in long-running hosts and model `on*` handlers). On this branch, object/array defaults use a **memory face** (`afw_value_isolate_mutable_default`) rather than a full structural clone; scalars still clone. Regression tests in `property_get.as` / `variable_get.as` lock the isolation behavior.

### Type and pattern parse nesting

Pathological nesting in a type annotation or destructure pattern is a **syntax error** (`Type or pattern nesting is too deep`), not a hang or crash. See [Script types](#adaptive-script-types-issue-28).

### Empty-match `replace`

`replace` with an empty `match` and `limit = -1` inserts at every code-point boundary (including the end) instead of hanging. See [Empty-match `replace`](#empty-match-replace-issue-190).

### Tests under `src/*/tests`

Tracked suites under `src/*/tests` are permanent regression assets. `afwdev test` uses temporary work directories; it does not mean those sources are disposable. New YAML/`--allow` coverage lives under **`src/afw_yaml/tests/`** (issue **[#14](https://github.com/afw-org/afw/issues/14)**).

---

## Other behavior you might have relied on

Must-change items are at the [top](#must-change-read-this-first). These are easier to miss if you depended on an old quirk:

- **`rootFilePaths`:** use `my/file.txt` with a matching prefix (not `/my/file.txt`). Longest prefix wins; stay inside the host root. [File streams](#file-streams-open_file-and-friends)
- **VFS:** empty-file read works; replace/modify no longer leaves trailing bytes. Set `maxReadBytes` on servers. [VFS](#vfs-adapter-afw_vfs)
- **Templates:** `` `\#` `` / `` `\$` `` emit literal `#` / `$` (they used to be an invalid escape). [Templates](#compile-time-template-substitutions-issue-97)
- **JSON / Fiddle UTF-8:** multi-byte text in `stringify` / Fiddle should display correctly; do not rely on the old `\ufffffff0…` escapes. Out-of-tree Python `Session("local")` needs the updated client. [UTF-8 JSON](#utf-8-in-json-results-and-python-local-mode)
- **Schemas:** regenerate before depending on updated editor/validate behavior. [JSON Schema](#json-schema-for-adaptive-object-types)
- **Model conf:** pure-script adapters may drop `mappedAdapterId`; if you omit it, every used op must be implemented in `on*`. [Model adapters](#pure-script-model-adapters)

---

## Related issues and PRs

| Topic | Issue | PR |
|-------|-------|-----|
| Compile-time substitutions (docs / TemplateString escapes) | [#97](https://github.com/afw-org/afw/issues/97) | [#100](https://github.com/afw-org/afw/pull/100) (feature on `develop`), [#124](https://github.com/afw-org/afw/pull/124) (docs/escapes here) |
| File streams | [#103](https://github.com/afw-org/afw/issues/103) | [#120](https://github.com/afw-org/afw/pull/120) |
| VFS empty file / hardening | [#79](https://github.com/afw-org/afw/issues/79) | [#122](https://github.com/afw-org/afw/pull/122) |
| Optional `mappedAdapterId` | [#109](https://github.com/afw-org/afw/issues/109) | [#119](https://github.com/afw-org/afw/pull/119) |
| Default-clone regressions | [#110](https://github.com/afw-org/afw/issues/110) | [#118](https://github.com/afw-org/afw/pull/118) (tests; fix earlier on `develop`) |
| Interactive libedit | [#30](https://github.com/afw-org/afw/issues/30) | [#117](https://github.com/afw-org/afw/pull/117) |
| JSON Schema `$ref` / OT projection | [#3](https://github.com/afw-org/afw/issues/3) | [#116](https://github.com/afw-org/afw/pull/116) |
| Process environment (single `current`, UTF-8/hexBinary) | [#71](https://github.com/afw-org/afw/issues/71) | [#123](https://github.com/afw-org/afw/pull/123) |
| Materializing retrieve `maxObjects` | [#49](https://github.com/afw-org/afw/issues/49) | [#128](https://github.com/afw-org/afw/pull/128) (partial; shared with [#90](https://github.com/afw-org/afw/issues/90)) |
| `checkIndividualObjectReadAccess` wiring / tests | [#90](https://github.com/afw-org/afw/issues/90) | [#128](https://github.com/afw-org/afw/pull/128) (shared with [#49](https://github.com/afw-org/afw/issues/49)) |
| Progressive retrieve object release | [#127](https://github.com/afw-org/afw/issues/127) (closed) | PR **[#168](https://github.com/afw-org/afw/pull/168)** → `mgg-develop` |
| Long-running memory / OOM | [#2](https://github.com/afw-org/afw/issues/2) | [#133](https://github.com/afw-org/afw/pull/133) (partial α/β on `mgg-develop`; more open) |
| C API Doxygen / builders + `--fulldev` | [#1](https://github.com/afw-org/afw/issues/1) | [#132](https://github.com/afw-org/afw/pull/132) |
| Adapter index `current::` | [#54](https://github.com/afw-org/afw/issues/54) | [#130](https://github.com/afw-org/afw/pull/130) (partial; see [#57](https://github.com/afw-org/afw/issues/57)) |
| `qualifier` / `qualifiers` snapshots + admin `maxObjects: 0` | [#9](https://github.com/afw-org/afw/issues/9) | [#129](https://github.com/afw-org/afw/pull/129) |
| Multi-frame `::` get + conf path templates + `process::` ambient | [#15](https://github.com/afw-org/afw/issues/15) (also [#71](https://github.com/afw-org/afw/issues/71)/[#74](https://github.com/afw-org/afw/issues/74) partial) | [#135](https://github.com/afw-org/afw/pull/135) |
| Object / array helpers | [#55](https://github.com/afw-org/afw/issues/55) (closed) | [#134](https://github.com/afw-org/afw/pull/134) |
| `afw_crypto` + secrets composition | [#74](https://github.com/afw-org/afw/issues/74) (partial; stays open for readpass) | [#136](https://github.com/afw-org/afw/pull/136) |
| `stringify` / `decompile` / listing / binary text | [#18](https://github.com/afw-org/afw/issues/18) (closed) | [#137](https://github.com/afw-org/afw/pull/137) |
| Expression property names in object values | [#38](https://github.com/afw-org/afw/issues/38) (closed) | [#139](https://github.com/afw-org/afw/pull/139) |
| Param / catch Patterns + call-site spread | [#140](https://github.com/afw-org/afw/issues/140) (closed) | [#141](https://github.com/afw-org/afw/pull/141), [#142](https://github.com/afw-org/afw/pull/142) |
| `variable_exists` bound vs undefined | [#131](https://github.com/afw-org/afw/issues/131) (closed) | [#146](https://github.com/afw-org/afw/pull/146) |
| UTF-8 JSON emitters + Python local FIFO | — | on `mgg-develop` (post-[#142](https://github.com/afw-org/afw/pull/142)) |
| Permanent `src/*/tests` regression assets | — | [#121](https://github.com/afw-org/afw/pull/121) (docs only) |
| `afw --allow` + YAML block strings / integers | [#14](https://github.com/afw-org/afw/issues/14) (closed) | regression tests on `mgg-develop` |
| Meta on the wire / reserved `"_meta_"` (design) | [#138](https://github.com/afw-org/afw/issues/138) | — (open; not required for [#38](https://github.com/afw-org/afw/issues/38)) |
| Adaptive Script types | [#28](https://github.com/afw-org/afw/issues/28) **closed** (PR **[#171](https://github.com/afw-org/afw/pull/171)**) | [#144](https://github.com/afw-org/afw/pull/144) / [#145](https://github.com/afw-org/afw/pull/145) core; [#171](https://github.com/afw-org/afw/pull/171) fence + FunctionSignature + handbook |
| Mutable object faces (shared instances) | [#17](https://github.com/afw-org/afw/issues/17) (closed) | PR **[#150](https://github.com/afw-org/afw/pull/150)** → `mgg-develop` (this file + `designs/issue-17-mutable-object-faces.md`) |
| UTF-8 code-point sequences (`s[i]`, for-of, formals) | [#153](https://github.com/afw-org/afw/issues/153) | issue-#153 branch → `mgg-develop` (this file + `designs/utf8-code-point-sequences.md`) |
| Array semantics (dense arrays, elision, `create_array`) | [#39](https://github.com/afw-org/afw/issues/39) | on `mgg-develop` |
| Conversion functions (type-named; no `null()` / `function()`) | — | on `mgg-develop` (see `designs/conversion-functions.md`) |
| Runtime catalog / accessors | [#149](https://github.com/afw-org/afw/issues/149) (under [#2](https://github.com/afw-org/afw/issues/2)) | on `mgg-develop` |
| Orchestrated tests (`orchestration.yaml`, hosts local/afwfcgi, expect-stdout) | [#157](https://github.com/afw-org/afw/issues/157) | PR **[#167](https://github.com/afw-org/afw/pull/167)** → `mgg-develop` |
| `tests-extra/` opt-in + firehose leaves (blast retired) | — | PR **[#167](https://github.com/afw-org/afw/pull/167)** → `mgg-develop` |
| afwdev test recipe flags (`-T`, `--output`) | — | on `mgg-develop` (`designs/afwdev-test-recipe.md`) |
| Graceful process stop (SIGTERM/SIGINT) | [#158](https://github.com/afw-org/afw/issues/158) (closed) | PR **[#165](https://github.com/afw-org/afw/pull/165)** → `mgg-develop` |
| Function reference prototypes ([#28](https://github.com/afw-org/afw/issues/28) Type spelling) | [#28](https://github.com/afw-org/afw/issues/28) | generate/docs on `mgg-develop` |
| Remove deprecated `throw` / declare helpers | [#172](https://github.com/afw-org/afw/issues/172) | this branch |

---

## How this was produced

Diff basis: originally `git log develop..mgg-develop`; that line is now on **`develop`** ([PR #179](https://github.com/afw-org/afw/pull/179), merge commit, history preserved). Includes PRs **[#116](https://github.com/afw-org/afw/pull/116)**–**[#124](https://github.com/afw-org/afw/pull/124)**, **[#128](https://github.com/afw-org/afw/pull/128)**–**[#130](https://github.com/afw-org/afw/pull/130)**, **[#132](https://github.com/afw-org/afw/pull/132)**–**[#142](https://github.com/afw-org/afw/pull/142)**, **[#145](https://github.com/afw-org/afw/pull/145)**–**[#146](https://github.com/afw-org/afw/pull/146)**, **[#150](https://github.com/afw-org/afw/pull/150)**, **[#165](https://github.com/afw-org/afw/pull/165)**, **[#167](https://github.com/afw-org/afw/pull/167)**, and follow-up fixes from that campaign. For full commit history, see those PRs on the repository hosting Adaptive Framework.

### Maintaining the Highlights table (for agents)

When adding or rewriting `whats-new.md` content:

1. **Every user-facing theme gets a `##` detail section** (and optional `###` subsections). Put the short pitch in the Highlights table and the full story under the section. If existing C or scripts **must change**, add a row to [Must change](#must-change-read-this-first) in the same edit.
2. **Link the Area cell** to that section, and make the issue number a real GitHub URL (do not rely on `#N` autolink — it only works on github.com): `[**Short name**](#github-heading-slug) ([#N](https://github.com/afw-org/afw/issues/N))`. PRs use `/pull/N`. Leave “What changed” unlinked.
3. **End each detail `##` section** with `[↑ Highlights](#highlights)` (not after every `###`). Skip meta sections (Must change, Reliability, Other behavior, Related issues, How this was produced).
4. **Slug** = GitHub’s auto-anchor for the rendered heading (the `id` on github.com). Spaces become `-`, then punctuation/`backticks` drop; leftover hyphens are **not** collapsed (` / ` and ` — ` → `--`; `` `afw --allow` `` → `afw---allow`). Verify on the rendered page if unsure. Prefer **stable, boring headings** so slugs don’t thrash.
5. **One theme, one primary section.** If two table rows share a section (e.g. UTF-8 + Python local), both may link to the same `#…`. Prefer a `###` link only when the jump is much clearer.
6. **No orphan links.** If there is no detail section yet, either add one or leave the Area unlinked until there is.
7. **After renames**, update Highlights links in the same edit as the heading change.
