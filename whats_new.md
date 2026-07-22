# What’s new on `mgg-develop`

This note is for **AFW users** (script authors, model authors, operators, and people editing Adaptive object JSON). It covers changes on **`mgg-develop`** since it branched from **`develop`** (merge-base `f945f97c`, mid‑July 2026).

Internal agent rules, Cursor docs, and pure test-infrastructure work are omitted unless they affect runtime or tooling you use day to day.

---

## Highlights

| Area | What changed |
|------|----------------|
| **Qualified variables** | `qualifier()` / `qualifiers()` return **fresh listable snapshots** (issue **#9**) |
| **Retrieve arrays** | Optional **`maxObjects`** on materializing `retrieve_objects` (default **100**; issue **#49**) |
| **Admin / JS client** | `AfwModel` passes **`maxObjects: 0`** for full metadata catalogs so admin loads after #49 |
| **Adapter auth** | `checkIndividualObjectReadAccess` wiring fixed + tests (issue **#90**) |
| **File streams** | Working `open_file` with hardened `rootFilePaths`; stream errors **throw** |
| **VFS adapter** | Empty files, safe full-file write, multi-map path rules, `maxReadBytes` |
| **Model adapters** | `mappedAdapterId` is **optional** for pure-script models |
| **`afw` CLI** | Optional interactive line editing and command history |
| **JSON Schema** | Cleaner editor schemas for Adaptive object types |
| **Process env** | One `current` on retrieve (issue **#71**); values are string if valid UTF-8 else hexBinary |
| **Templates** | Compile-time substitution `#{…}` docs and tests; backtick `` `\#` `` / `` `\$` `` match raw templates (issue **#97**) |
| **C builders / afwdev** | Richer C API Doxygen, package **0.12.2**, `afwdev build --fulldev` (issue **#1**) |

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

**Issue #9**

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
| **Most recent wins** | Get (`qualifier::name`) uses the **most recent** matching visible stack entry for that qualifier. A snapshot walks **all** matching visible entries (most recent first); each may contribute, and the most recent definition wins per property name. Nested pushes are cleaned up with the stack (e.g. `AFW_TRY`). |
| **`includeUntrusted`** | Optional boolean, default **false**. **Default** matches normal `qualifier::name` visibility right now. While the xctx is **secure**, set **true** so the snapshot matches what you would see with `::` if you were **less secure** (trusted **and** untrusted frames—not untrusted-only). When already not secure, true and false are the same. |
| **Can be large** | Snapshots copy variable bags into memory objects. `environment::` / `request::` (and similar) can be **big**; `qualifiers()` nests a full snapshot per active qualifier and multiplies cost. Prefer `qualifier::name` day to day; use list functions sparingly and do not retain or rebuild large snapshots in long-running scripts. |

Object-backed qualifiers (`environment::`, `request::`, `application::`, model `current::` runtime bags, …) contribute by walking their objects. Callback-backed frames (app `current::`, model `custom::`, log, context tables) contribute their known variable sets.

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

## File streams (`open_file` and friends)

**Issue #103 / PR #120**

Adaptive Script file I/O is finished for beta: open, read, write, flush, and close through the stream category, with safe path resolution.

### Using `open_file`

Configure logical path prefixes on the application as `rootFilePaths` (type `_AdaptiveRootFilePaths_`). Each property name is a **prefix**; its value is the host directory.

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

Tracked suites under `src/*/tests` are permanent regression assets. `afwdev test` uses temporary work directories; it does not mean those sources are disposable.

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
| Long-running memory / OOM | #2 | — (open follow-up) |
| C API Doxygen / builders + `--fulldev` | #1 | #132 |
| Adapter index `current::` | #54 | #130 (partial; see #57) |
| `qualifier` / `qualifiers` snapshots | #9 | #129 (includes admin `maxObjects: 0` client fix) |
| Permanent `src/*/tests` regression assets | — | #121 (docs only) |

---

## How this was produced

Diff basis: `git log develop..mgg-develop` and the corresponding code/metadata changes (including PRs **#116**–**#124**, **#128**–**#130**, **#132**). For full commit history, see those PRs on the repository hosting Adaptive Framework.
