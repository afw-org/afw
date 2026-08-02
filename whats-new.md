# What’s new on `mgg-develop`

This note is for **AFW users** (script authors, model authors, operators, and people editing Adaptive object JSON). It covers changes on **`mgg-develop`** since it branched from **`develop`** (merge-base `f945f97c`, mid‑July 2026).

Internal agent rules, Cursor docs, and pure test-infrastructure work are omitted unless they affect runtime or tooling you use day to day.

---

## Rebuild out-of-tree commands and extensions

Several changes on this branch update **libafw** public interfaces and generated bindings (value/memory work, **array setter** reshape for issue **#55**, and related headers).

If you maintain **anything that links AFW outside a full in-tree rebuild** — extension **DSOs**, custom **commands**, or other binaries that load `libafw` — **rebuild and reinstall them against this AFW install**. Mixing old DSOs/commands with a new `libafw` (or the reverse) can fail at load time or misbehave at runtime.

In-tree extensions and the `afw` / `afwfcgi` commands built with the same `./afwdev build --cdev` / `--fulldev` install are fine. Individual sections below also call this out where the ABI surface changed.

---

## Highlights

| Area | What changed |
|------|----------------|
| **Object / array helpers (#55)** | `keys` / `values` / `entries`, `at`, `push`/`pop`/`shift`/`unshift`, `splice`, `freeze`, `every`/`some` — **recompile** out-of-tree commands/extensions |
| **Expression property names (#38)** | Object values may use `{ [expression]: value }` (same idea as `obj[expr]` get/set) |
| **Qualifier snapshots (#9)** | **`qualifier(name)`** / **`qualifiers()`** return **fresh listable objects** (not live proxies); optional **`includeUntrusted`**; missing name → **nullish**; can be **large** |
| **Multi-frame `::` get** | Stacked same-name qualifiers: first **defining** frame wins (was “first matching frame only”); aligned with snapshot semantics (landed with #15 work) |
| **Retrieve arrays (#49)** | Optional **`maxObjects`** on materializing `retrieve_objects` / `…_with_uri` (default **100**, **0** = unlimited; over max → **`payload_too_large`**) |
| **Admin / JS client** | `AfwModel` sends **`maxObjects: 0`** for full metadata catalogs so admin loads after the #49 default of 100 |
| **Adapter auth (#90)** | `checkIndividualObjectReadAccess` wiring fixed + tests (action **`read`** as well as **`query`**) |
| **File streams (#103)** | Working `open_file` with hardened `rootFilePaths`; stream errors **throw** (not `-1` / `get_stream_error`) |
| **Conf path templates (#15)** | Path-like conf properties are **templates** at create/start; host dirs often resolved to full path; VFS `vfsMap` / LDAP `url` too |
| **VFS adapter (#79)** | Empty files, safe full-file write, multi-map path rules, `maxReadBytes` |
| **Model adapters (#109)** | `mappedAdapterId` is **optional** for pure-script models |
| **`afw` CLI** | Optional interactive line editing and history (#30); **`--allow` / `-a`** for result content type (YAML block strings, issue **#14**) |
| **JSON Schema (#3)** | Cleaner editor schemas for Adaptive object types |
| **Process env (#71)** | One `current` on `_AdaptiveEnvironmentVariables_` retrieve; values string if valid UTF-8 else hexBinary |
| **`process::` (#74 partial)** | Ambient `args`, `programName`, `pid`, `cwd`, `afwVersion`, `startTime` at env create (with `environment::`) |
| **`afw_crypto` (#74 partial)** | Optional extension: AES-GCM encrypt/decrypt/**seal**/**unseal**, digest/HMAC, keystore, key refs, PBKDF2; LDAP `bindParameters` recipe |
| **Templates (#97)** | Compile-time substitution `#{…}` docs and tests; backtick `` `\#` `` / `` `\$` `` match raw templates |
| **Adapter index `current::` (#54 partial)** | Index filter/value scripts see **`current::object`**, `objectId`, `objectType`, `key` (not bare ambient `object`) |
| **C builders / afwdev (#1)** | Richer C API Doxygen, package **0.12.2**, `afwdev build --fulldev` |
| **Value / memory (α/β, #2)** | Incremental work: permanent scalar reuse, dual-face object/array values, safer managed object value release — **recompile** out-of-tree commands/extensions |
| **`stringify` / `decompile` / listing (#18)** | **`stringify`** pure JSON (+ replacer); **`decompile`** Adaptive compiled form; **compile listing** human tree+symbols; **`decode_to_string`** UTF-8 from octets |
| **UTF-8 in JSON / Fiddle** | Multi-byte UTF-8 survives **`stringify`**, Fiddle results, and other JSON emitters (signed-char octet bug) |
| **Python `Session("local")`** | Local FIFO client uses **binary octet** framing so large/UTF-8 responses no longer hang |
| **Param / catch Patterns (#140)** | Function/lambda params + `catch` Patterns; Expression defaults; call-site `f(...arr)`; computed/string keys; type syntax for later checking |
| **Script types (#28)** | TS-like type syntax (Adaptive dataType leaves); opt-in typeCheck flags / `#typecheck`; hard cut of `(array of …)` / `(object "OT")` |

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

---

## Function parameter and catch Patterns (issue #140)

**Issue #140** — PRs **#141** (params + catch Patterns) and **#142** (call-site spread, Pattern keys, catch decompile, type syntax) on `mgg-develop`.

Adaptive Script already allowed list/object **Patterns** on `let` / `const`, assignment, and `for` / `for-of` heads. The same Patterns may now appear:

- In **function and lambda parameter lists** (nested rename, holes, rest, property defaults, **computed/string keys**).
- In **`catch (…)`** bindings (e.g. `catch ({ message, data })`).
- **Call-site spread:** `f(...arr)` / `f(a, ...rest, b)` expands an array into separate arguments.

Parameter **defaults are Expressions**. Pattern leaves and whole formals may carry **type annotations** (syntax for upcoming compile-time checking; not enforced yet). Options-style example:

```adaptive
function connect({ host, port = 443 } = { host: "localhost" }) {
    return host + ":" + string(port);
}
function sum3(a, b, c) { return a + b + c; }
assert(sum3(...[1, 2, 3]) === 6);
```

Not included: arrow functions, ES `arguments` object (use formal `...rest`). Language Reference: Function statement; Features — Exception Handling, Functions and parameters.

---

## Object and array helpers (issue #55)

**Issue #55** on `mgg-develop` (branch `issue-#55`).

Adaptive Script gains common object/array helpers and stack/queue-style mutators. These are **Adaptive functions** registered in the environment (with optional `value->method(...)` sugar when the function is a data-type method).

### Rebuild / recompile requirement

This work changes the **C `afw_array_setter` interface** (e.g. `add_value` → `push_value`, new `pop_value` / `shift_value`, signed indexes) and adds core function bindings. **Out-of-tree commands and extensions must be recompiled** against the new libafw (see the top callout).

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

---

## Expression property names in object values (issue #38)

**Issue #38** on `mgg-develop` (branch `issue-#38`).

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
- A **literal** `_meta_: { … }` property in source still installs **sideband meta** (not a normal property). A **bracket** name that evaluates to `"_meta_"` sets a **normal** property under that name — map content types that reserve `"_meta_"` on the wire are a separate topic (issue **#138**).
- Handbook: Language Reference **Objects and Arrays** and **Features** (working with objects).

Tests: `src/afw/tests/language/script/object_expression_names.as`.

---

## Value lifetime / memory management (issue #2) — alpha/beta

**Issue #2** — work in progress on `mgg-develop` via branch `issue-#2` (partial land; design continues).

This is **not** a finished memory-management productization. Treat it as **alpha/beta** on the maintainer develop line: useful foundation and mostly behavior-compatible for in-tree tests, but the long-running escape / assign / scope-release story is **not** complete.

### What landed so far (high level)

- Prefer **shared permanent Adaptive values** (generate bag / `afw_v_*`) for known scalars instead of allocating fresh ones where safe (null, boolean true/false, many const_objects properties).
- **Object and array instances** more consistently expose a dual Adaptive value face (`->value`) with a lifetime-matched permanent/managed/unmanaged inf.
- **Managed object values**: container-aware `optional_release` / `clone_or_reference` paths that do **not** free an embedded dual-face header; `create_managed_object` requires a non-null object and takes a container hold.
- Living design notes for maintainers: `designs/memory-management.md` (not user docs).

### Rebuild / recompile requirement

Installing a build that includes this work updates **libafw** (and related generated headers/bindings). If you maintain **out-of-tree** or separately built:

- extension **`.so` / DSOs**,
- custom **commands**,
- or any binary that **links the AFW shared library**,

you **must rebuild and reinstall those against the new AFW install**. Mixing an older extension/command with a newer `libafw` (or the reverse) can fail at load time or misbehave at runtime.

In-tree extensions and the `afw` / `afwfcgi` commands built with the same `./afwdev build --cdev` / `--fulldev` install are fine.

### Not done yet (do not rely on)

- Value **create** always returning existing dual-face identity (no double-wrap) — planned next (**1d**).
- Script **assign** via `clone_or_reference` (objects/arrays as shared references; scalars typically cloned when escaping).
- Scope teardown walking each variable with value release (today: scope subpool bulk free).
- Full target model: `clone_or_reference` never returns unmanaged; managed wrappers for unmanaged containers; property promote-on-get; etc.

---

## C API docs and full package builds (issue #1)

**Issue #1** / PR **#132** (merged to `mgg-develop`)

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
- Extension srcdirs stay self-contained relative to libafw core; their Doxygen groups live in the extension public headers.

---

## Adapter index filter/value `current::` (issue #54) — partial

**Issue #54** / PR **#130** (core eval context; LMDB create path still has known issues)

If you author index **filter** / **value** scripts (definitions stored in the LMDB adapter’s internal config via `index_create`, not normal adapter conf), use **`current::`** while they evaluate:

| Variable | Meaning |
|----------|---------|
| **`current::object`** | Object being indexed |
| **`current::objectId`** | Its object id |
| **`current::objectType`** | Its object type id |
| **`current::key`** | Index definition key |

Bare ambient **`object`** (old unqualified scope push) is **not** set. Prefer `current::object` or `variable_get("current::object")`. If **value** is omitted, the property named by **key** is indexed without a script.

**Not fully productized yet:** LMDB `index_create` persistence / retroactive scan still has pre-existing txn issues; automated index smoke remains skipped until that is fixed (see #57). Everyday LMDB CRUD does not require indexes.

---

## List active qualified variables (issue #9)

**Issue #9** / PR **#129**. (Multi-frame **`::` get** alignment is separate — see subsection below and issue **#15** / PR **#135**.)

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

Landed with conf/process ambient work (**#15** / PR **#135**), not as part of the original #9 snapshot API.

Older builds stopped **`qualifier::name` get** after the **first matching qualifier frame**, even when that frame did not define the name. Stacked `current::` (log write, model, `evaluate(..., additionalUntrustedQualifiedVariables)`, …) could hide older bindings such as `current::mode`. Get now matches snapshot semantics: walk newest → older and take the first frame that **defines** the name. Present undefined must use **`afw_value_undefined`**, not C `NULL` (C `NULL` means “not on this frame”).

---

## Materializing retrieve: `maxObjects` (issue #49)

**Issue #49** (partial)

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

They still use the same adapter session underneath; only the **array-building** functions enforce `maxObjects`. (Safe release after progressive write is tracked separately as issue **#127**. Broader long-running memory / OOM handling is issue **#2**.)

`maxObjects` is **not** an adapter conf property and **not** RQL/client paging—those remain longer-term #49 work.

### Admin / `@afw/client` after the default of 100

Core metadata catalogs (object types, etc.) are larger than 100. Materializing retrieves used by the admin SPA (Home boot `loadObjectTypes`, Documentation Schema via `useRetrieveObjects`) therefore failed with **`payload_too_large`** until the JS client was updated.

**`AfwModel`** (`@afw/client`) now sends **`maxObjects: 0`** (unlimited) for:

- `loadObjectTypes`
- `retrieveObjects` (default; callers can still pass a positive limit)

Rebuild/install the admin app (or full JS install) and hard-refresh the browser. Progressive `retrieve_objects_to_response` (already used by the Objects browser) remains the better pattern for large **instance** data; that client story is still open under #49.

---

## Adapter get/retrieve authorization (issue #90)

**Issue #90**

Adapter get and retrieve always perform an action **`query`** authorization check on the resource before objects are returned. When adapter conf sets:

```json
"checkIndividualObjectReadAccess": true
```

each object is also checked with action **`read`** (object body available for the decision). Default is **`false`** (query only)—useful when decisions need per-object properties only if you opt in.

Session delivery was fixed so intermediate handling (e.g. `_AdaptiveServiceConf_` typing, object-type dedupe) still runs when individual read is **off**. Previously those steps could be skipped along with the read check.

Retrieve collection resource ids use a trailing slash (e.g. `/adapterId/ObjectType/`); get uses the full object path.

---

## Compile-time template substitutions (issue #97)

**Issue #97** (feature largely landed earlier as PR **#100**; completed on this branch)

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

---

## Process environment variables (issue #71)

`retrieve_objects("afw", "_AdaptiveEnvironmentVariables_")` now returns a **single** `current` object (process environment), not two identical ones. The `environment::` qualifier is unchanged.

Property **values** from the host environment are Adaptive **string** when the bytes are valid UTF-8 (NFC), otherwise **hexBinary** (raw octets preserved). Property **names** that are not valid UTF-8 appear as `_NONUTF8_` plus uppercase hex of the raw name. A bad value no longer prevents AFW from starting.

Request CGI/FCGI-like parameters remain under `_AdaptiveRequestProperties_` / `request::` (separate from process env).

---

## Process ambient: `environment::` and `process::` (issues #71 / #74)

Process environment variables and invocation info are created at **environment create** (not per host) and pushed on **every** xctx via `afw_application_internal_push_qualifiers` (called from xctx finishup):

| Qualifier | Object | Contents |
|-----------|--------|----------|
| **`environment::`** | `/afw/_AdaptiveEnvironmentVariables_/current` | Process env vars (string or hexBinary per #71) |
| **`process::`** | `/afw/_AdaptiveProcess_/current` | Invocation / process identity (partial #74) |

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

---

## Crypto extension `afw_crypto` (issue #74 partial)

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

Requires `libssl-dev` / `openssl-devel` at build time. Design notes: `designs/secrets-and-afw-crypto.md`. Interactive **`readpass`** remains open for #74.

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

---

## `stringify`, `decompile`, compiler listing, and binary text

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

---

## Adaptive Script types (issue #28)

**Type syntax** is TypeScript-like, with **Adaptive data types** as leaves (`integer`, `string`, `any`, `void`, …)—not JavaScript primitives. Examples: `integer[]`, `Array<string>`, `[integer, string]`, `integer|string`, `{ host: string, port?: integer }`, `(a: integer)=>integer`, plus script-local `type` / `interface` (**not** adaptive object types / OT catalogs).

Old Adaptive Type spellings such as `(array of integer)` and `(object "SomeOT")` are **removed** (hard cut). Existing scripts that used those forms need updating; plain unannotated scripts are unchanged because **checking is off by default**.

**Opt in** with flags or the `#typecheck` pragma:

| Flag / pragma | Effect |
|---------------|--------|
| *(default)* | Parse and store types only |
| `compile:typeCheck` | Compile-time (when known) **and** runtime checks |
| `compile:typeCheckCompileOnly` | Compile-time only (**wins** if both mode flags set) |
| `compile:noImplicitAny` | Require annotations when checking is active |
| `compile:strictNullChecks` | Stricter null/undefined |
| `compile:strict` | typeCheck + noImplicitAny + strictNullChecks |
| `#typecheck;` / `#typecheck on;` | Full typeCheck for rest of that compile |
| `#typecheck compileOnly;` | Compile-only for that compile |
| `#typecheck off;` | Off for that compile (also clears noImplicitAny / strictNullChecks) |
| `#typecheck on noImplicitAny;` | Full check + require annotations |
| `#typecheck on strictNullChecks;` | Full check + strict null/undefined |
| `#typecheck strict;` | Full + noImplicitAny + strictNullChecks |
| `#typecheck on, noImplicitAny;` | Commas between tokens optional |

When checking is on, assignment and script function parameters are checked for leaf data types, unions/intersections, **object/interface shapes** (required props, property types, `extends`), **array elements**, **tuple length/positions**, **function param/return shapes** (script functions/closures), and **Pattern** element annotations on list/object destructure. Extra object properties are allowed (structural typing). Type errors name missing properties, element indexes, and decompiled expected types where possible.

Handbook: Language Reference → **Types**. Tests: `src/afw/tests/compiler/type_syntax.as`, `type_check.as`. Design pad: `designs/issue-28-type-syntax.md`.

---

## Conf path templates (issue #15)

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

---

## File streams (`open_file` and friends)

**Issue #103 / PR #120**

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

---

## VFS adapter (`afw_vfs`)

**Issue #79** (empty-file read and related hardening on the issue branch)

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

---

## Pure-script model adapters

**Issue #109 / PR #119**

A model adapter still loads an `_AdaptiveModel_` from `modelLocationAdapterId` / `modelId`. What changed is the backend requirement:

- **Hybrid (unchanged):** set `mappedAdapterId` and map-and-forward (or fall through with `current::useDefaultProcessing` / `model_default_*`).
- **Pure-script (new):** implement every operation you use in model object-type `on*` scripts and **omit `mappedAdapterId`**. No dummy file/LMDB backend is required.

If an operation needs default processing and `mappedAdapterId` is missing, AFW fails with a clear error (implement the `on*` hook or set `mappedAdapterId`).

In scripts, `current::mappedAdapterId` is nullish when unset (it does not throw).

`modelId` and `modelLocationAdapterId` remain required for both hybrid and pure-script models.

---

## Interactive `afw`: line editing and history

**Issue #30 / PR #117**

When you run `afw` interactively on a real terminal (no file, no pipe, not `--local` chunk mode), builds linked with **libedit** offer:

- Emacs-style line editing
- Command history stored in **`~/.afw_history`** (when `HOME` is set)

Pipes, file input, and non-TTY use are unchanged. Builds without libedit still succeed; interactive history/editing simply is not available (the usage text says so).

To enable at build time on Debian/Ubuntu:

```bash
sudo apt-get install libedit-dev
```

Other distros: `libedit-dev` / `libedit-devel` as appropriate. See `src/afw/doc/building_on_linux.md` (and the macOS build notes).

---

## `afw --allow` and YAML value output (issue #14)

**Issue #14** (feature on the tree for a long time; **regression suite** added on `mgg-develop`)

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

Permanent suite: **`src/afw_yaml/tests/yaml_allow_output.py`** (tags `yaml`, `content_type`).

```bash
afwdev test -p afw_yaml --show-all
# or
afwdev test -p afw_yaml --tags yaml
```

Covers `--allow` short/media ids, invalid allow, JSON path sanity, block-scalar chomping/indent, integers and other primitives, objects/arrays, and `-t yaml` conf parse. Handbook `usage.xml` may still omit `-a` until a docs pass; live **`afw -h`** lists it.

---

## JSON Schema for Adaptive object types

**Issue #3 / PR #116**

Package-generated schemas under `generated/schemas/afw/` are improved for **editors** (e.g. VS Code completion, hovers, light validation) and for `afwdev validate`:

- No more invalid “`$ref` mixed with sibling keywords” shapes that break validators.
- Entity schemas promote type/properties to the document root for better editor apply behavior.
- Inheritance merges parent `propertyTypes` with **child overrides** so nested models validate correctly.
- `defaultValue` / possible values map more cleanly to JSON Schema `default` / `enum`.

If you edit Adaptive object JSON under `generate/objects/` (or rely on schema-backed validation), regenerate so your tree picks up the new schemas (`./afwdev build --cdev` or your usual generate path). Do not hand-edit `generated/schemas/`.

---

## Reliability notes

### Default values from `property_get` / `variable_get`

The clone-on-return fix for mutable defaults (issue **#110**) landed on `develop` before this branch. On `mgg-develop`, dedicated Adaptive Script regression tests lock that behavior in: a default object or array returned for a missing property/variable is isolated so later mutations do not poison other calls (important in long-running hosts and model `on*` handlers).

### Tests under `src/*/tests`

Tracked suites under `src/*/tests` are permanent regression assets. `afwdev test` uses temporary work directories; it does not mean those sources are disposable. New YAML/`--allow` coverage lives under **`src/afw_yaml/tests/`** (issue **#14**).

---

## Breaking / migration checklist

1. **Stream errors:** replace any `open_file` / `stream` checks for `-1` and any use of **`get_stream_error`** with `try` / `catch`.
2. **Removed functions:** stop calling **`open_uri`**, **`open_response`**, **`get_stream_error`** if you had experimental callers.
3. **`rootFilePaths`:** prefer logical paths like `my/file.txt` with a matching prefix property; rely on longest-prefix match and stay inside the host root.
4. **Model conf:** pure-script adapters may drop `mappedAdapterId`; hybrid adapters keep it. If you omit it, every used op must be implemented in `on*`.
5. **Schemas:** regenerate before depending on updated editor/validate behavior.
6. **VFS:** if you relied on reading empty files failing, or on replace/modify
   leaving trailing bytes when shortening content, update callers—those cases
   now succeed with correct full-file content. Prefer setting `maxReadBytes`
   appropriately for server hosts.
7. **Template strings:** if you relied on `` `\#…` `` or `` `\${…}` `` failing
   with “Invalid escape code,” they now emit literal `#` / `$` (opener
   suppress). Real `#{…}` / `${…}` substitutions are unchanged.
8. **`retrieve_objects` / `retrieve_objects_with_uri`:** default **`maxObjects` is 100**.
   Full catalog dumps (e.g. all `_AdaptiveObjectType_` or `_AdaptiveFunction_`
   objects) must pass **`maxObjects: 0`** (or a higher explicit cap), or use a
   progressive `retrieve_objects_to_*` API. Over the limit throws
   **`payload_too_large`**. Language bindings expose the same optional parameter
   (e.g. Python `maxObjects=0`).
9. **Individual object read auth:** if you set `checkIndividualObjectReadAccess`
   to `true`, ensure policies handle action **`read`** as well as **`query`**.
10. **JSON / Fiddle non-ASCII:** multi-byte UTF-8 in results should display correctly;
    if you relied on broken `\ufffffff0…` escapes from old `stringify`, update
    callers. Out-of-tree **Python** `Session("local")` needs the updated client
    for large or UTF-8-heavy local responses.

---

## Related issues and PRs

| Topic | Issue | PR |
|-------|-------|-----|
| Compile-time substitutions (docs / TemplateString escapes) | #97 | #100 (feature on `develop`), #124 (docs/escapes here) |
| File streams | #103 | #120 |
| VFS empty file / hardening | #79 | #122 |
| Optional `mappedAdapterId` | #109 | #119 |
| Default-clone regressions | #110 | #118 (tests; fix earlier on `develop`) |
| Interactive libedit | #30 | #117 |
| JSON Schema `$ref` / OT projection | #3 | #116 |
| Process environment (single `current`, UTF-8/hexBinary) | #71 | #123 |
| Materializing retrieve `maxObjects` | #49 | #128 (partial; shared with #90) |
| `checkIndividualObjectReadAccess` wiring / tests | #90 | #128 (shared with #49) |
| Progressive retrieve object release | #127 | — (open follow-up) |
| Long-running memory / OOM | #2 | #133 (partial α/β on `mgg-develop`; more open) |
| C API Doxygen / builders + `--fulldev` | #1 | #132 |
| Adapter index `current::` | #54 | #130 (partial; see #57) |
| `qualifier` / `qualifiers` snapshots + admin `maxObjects: 0` | #9 | #129 |
| Multi-frame `::` get + conf path templates + `process::` ambient | #15 (also #71/#74 partial) | #135 |
| Object / array helpers | #55 | #134 |
| `afw_crypto` + secrets composition | #74 (partial; stays open for readpass) | #136 |
| `stringify` / `decompile` / listing / binary text | #18 | #137 |
| Expression property names in object values | #38 | #139 |
| Param / catch Patterns + call-site spread | #140 | #141, #142 |
| UTF-8 JSON emitters + Python local FIFO | — | on `mgg-develop` (post-#142) |
| Permanent `src/*/tests` regression assets | — | #121 (docs only) |
| `afw --allow` + YAML block strings / integers | #14 | — (feature earlier; regression tests on `mgg-develop`) |
| Meta on the wire / reserved `"_meta_"` (design) | #138 | — (open; not required for #38) |

---

## How this was produced

Diff basis: `git log develop..mgg-develop` and the corresponding code/metadata changes (including PRs **#116**–**#124**, **#128**–**#130**, **#132**–**#142**, and follow-up fixes on `mgg-develop`). For full commit history, see those PRs on the repository hosting Adaptive Framework.
