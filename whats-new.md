# What’s new on `mgg-develop`

This note is for **AFW users** (script authors, model authors, operators, and people editing Adaptive object JSON). It covers changes on **`mgg-develop`** since it branched from **`develop`** (merge-base `f945f97c`, mid‑July 2026).

Internal agent rules, Cursor docs, and pure test-infrastructure work are omitted unless they affect runtime or tooling you use day to day.

---

## Rebuild out-of-tree commands and extensions

Several changes on this branch update **libafw** public interfaces and generated bindings (value/memory work, **array setter** reshape for issue **#55**, and related headers).

If you maintain **anything that links AFW outside a full in-tree rebuild** — extension **DSOs**, custom **commands**, or other binaries that load `libafw` — **rebuild and reinstall them against this AFW install**. Mixing old DSOs/commands with a new `libafw` (or the reverse) can fail at load time or misbehave at runtime.

In-tree extensions and the `afw` / `afwfcgi` commands built with the same `./afwdev build --cdev` / `--fulldev` install are fine. Individual sections below also call this out where the ABI surface changed.

Most work on this line needs **only a recompile** against the new install. **C code** that used the old first-class **`afw_iterator`** name for the legacy opaque cursor must use **`afw_iterator_old`** after **#153** (new keyless **`afw_iterator`** is a different type). Array/object `get_next_*` call sites that already used the old cursor style need that rename, not a behavior rewrite.

---

## Highlights

Each **Area** links to the detail section below (when there is one). Detail
sections end with **[↑ Highlights](#highlights)** to return here.

| Area | What changed |
|------|----------------|
| [**Object / array helpers (#55)**](#object-and-array-helpers-issue-55) | `keys` / `values` / `entries`, `at`, `push`/`pop`/`shift`/`unshift`, `splice`, `freeze`, `every`/`some` — **recompile** out-of-tree commands/extensions |
| [**Expression property names (#38)**](#expression-property-names-in-object-values-issue-38) | Object values may use `{ [expression]: value }` (same idea as `obj[expr]` get/set) |
| [**Qualifier snapshots (#9)**](#list-active-qualified-variables-issue-9) | **`qualifier(name)`** / **`qualifiers()`** return **fresh listable objects** (not live proxies); optional **`includeUntrusted`**; missing name → **nullish**; can be **large** |
| [**Multi-frame `::` get**](#multi-frame-get-aligned-with-snapshots) | Stacked same-name qualifiers: first **defining** frame wins (was “first matching frame only”); aligned with snapshot semantics (landed with #15 work) |
| [**Retrieve arrays (#49)**](#materializing-retrieve-maxobjects-issue-49) | Optional **`maxObjects`** on materializing `retrieve_objects` / `…_with_uri` (default **100**, **0** = unlimited; over max → **`payload_too_large`**) |
| [**Admin / JS client**](#admin-afwclient-after-the-default-of-100) | `AfwModel` sends **`maxObjects: 0`** for full metadata catalogs so admin loads after the #49 default of 100 |
| [**Adapter auth (#90)**](#adapter-getretrieve-authorization-issue-90) | `checkIndividualObjectReadAccess` wiring fixed + tests (action **`read`** as well as **`query`**) |
| [**File streams (#103)**](#file-streams-open_file-and-friends) | Working `open_file` with hardened `rootFilePaths`; stream errors **throw** (not `-1` / `get_stream_error`) |
| [**Conf path templates (#15)**](#conf-path-templates-issue-15) | Path-like conf properties are **templates** at create/start; host dirs often resolved to full path; VFS `vfsMap` / LDAP `url` too |
| [**VFS adapter (#79)**](#vfs-adapter-afw_vfs) | Empty files, safe full-file write, multi-map path rules, `maxReadBytes` |
| [**Model adapters (#109)**](#pure-script-model-adapters) | `mappedAdapterId` is **optional** for pure-script models |
| [**`afw` CLI**](#interactive-afw-line-editing-and-history) | Optional interactive line editing and history (#30); **`--allow` / `-a`** for result content type (YAML block strings, issue **#14**) — see also [YAML / `--allow`](#afw-allow-and-yaml-value-output-issue-14) |
| [**JSON Schema (#3)**](#json-schema-for-adaptive-object-types) | Cleaner editor schemas for Adaptive object types |
| [**Process env (#71)**](#process-environment-variables-issue-71) | One `current` on `_AdaptiveEnvironmentVariables_` retrieve; values string if valid UTF-8 else hexBinary |
| [**`process::` (#74 partial)**](#process-ambient-environment-and-process-issues-71-74) | Ambient `args`, `programName`, `pid`, `cwd`, `afwVersion`, `startTime` at env create (with `environment::`) |
| [**`afw_crypto` (#74 partial)**](#crypto-extension-afw_crypto-issue-74-partial) | Optional extension: AES-GCM encrypt/decrypt/**seal**/**unseal**, digest/HMAC, keystore, key refs, PBKDF2; LDAP `bindParameters` recipe |
| [**Templates (#97)**](#compile-time-template-substitutions-issue-97) | Compile-time substitution `#{…}` docs and tests; backtick `` `\#` `` / `` `\$` `` match raw templates |
| [**Adapter index `current::` (#54 partial)**](#adapter-index-filtervalue-current-issue-54-partial) | Index filter/value scripts see **`current::object`**, `objectId`, `objectType`, `key` (not bare ambient `object`) |
| [**C builders / afwdev (#1)**](#c-api-docs-and-full-package-builds-issue-1) | Richer C API Doxygen, package **0.12.2**, `afwdev build --fulldev` |
| [**Value / memory (α/β, #2)**](#value-lifetime-memory-management-issue-2-alphabeta) | Incremental work: permanent scalar reuse, dual-face object/array values, safer managed object value release; **`afw_pool_release` returns pool or NULL**; managed object faces pin base — **recompile** out-of-tree commands/extensions |
| [**`stringify` / `decompile` / listing (#18)**](#stringify-decompile-compiler-listing-and-binary-text) | **`stringify`** pure JSON (+ replacer); **`decompile`** Adaptive compiled form; **compile listing** human tree+symbols; **`decode_to_string`** UTF-8 from octets |
| [**UTF-8 in JSON / Fiddle**](#utf-8-in-json-results-and-python-local-mode) | Multi-byte UTF-8 survives **`stringify`**, Fiddle results, and other JSON emitters (signed-char octet bug) |
| [**Python `Session("local")`**](#utf-8-in-json-results-and-python-local-mode) | Local FIFO client uses **binary octet** framing so large/UTF-8 responses no longer hang |
| [**Param / catch Patterns (#140)**](#function-parameter-and-catch-patterns-issue-140) | Function/lambda params + `catch` Patterns; Expression defaults; call-site `f(...arr)`; computed/string keys; type syntax for later checking |
| [**`variable_exists` bound vs value (#131)**](#variable_exists-bound-vs-undefined-issue-131) | `variable_exists` is **bound** (true for uninit / undefined); `variable_get` default only if **not bound**; light function briefs |
| [**Script types (#28)**](#adaptive-script-types-issue-28) | Type annotations on Adaptive dataType leaves + shapes; opt-in `compile:typeCheck*` flags (and optional `#compile` pragma); hard cut of `(array of …)` / `(object "OT")` |
| [**Function reference prototypes**](#function-reference-prototypes-28-spelling) | Generated Adaptive function prototypes (admin Function Reference, Monaco, C Declaration comments) use **#28 Type** spelling (`T[]`, `(…) => R`); OT ids stay as `//` notes on multi-line forms |
| [**Mutable object faces (#17)**](#mutable-object-faces-issue-17) | Literals, no clone-on-bind, adapter get/retrieve/callback, defaults, journal (incl. consumer), nested faces — drop many manual `clone()` calls (**PR #150** → `mgg-develop`) |
| [**Array semantics (#39)**](#array-semantics-issue-39) | Literal elision → undefined; assign-append at `length`; **`create_array(n)`**; dense arrays only (no sparse / no `in`/`delete`) |
| [**Conversion functions**](#conversion-functions-type-named) | Type-named converts; no `null()` / `function()` converts; `array` is constructor; source types hold text for `compile` |
| [**UTF-8 code-point sequences (#153)**](#utf-8-code-point-sequences-issue-153) | Utf8-backed values as **immutable code-point sequences**: `s[i]`, for-of, array formals / HOFs; C **`afw_iterator`** redesign (**recompile** out-of-tree; rename legacy cursor to **`afw_iterator_old`**) |
| [**afwdev advanced-test (#157)**](#experimental-afwdev-advanced-test-issue-157) | **\*\*\* Experimental \*\*\***: hermetic `afwfcgi` multi-step tests via `advanced-test.yaml` / `.json` — for comment; may change |
| [**afwdev blast**](#experimental-afwdev-blast) | **\*\*\* Experimental \*\*\***: on-demand random suite firehose at afwfcgi — not part of `test -j` |
| [**afwdev test/blast recipe flags**](#afwdev-testblast-recipe-flags) | **\*\*\* Experimental \*\*\***: `--tests-path`/`-T`, `--output` / `--output-format` for machine summaries |
| [**Runtime catalog / accessors (#149)**](#runtime-catalog-accessors-issue-149-phase-1) | Phase 1: lock+copy adapter/auth **`referenceCount`**; accessor registry. Phase 2: rich **objectOptions** on permanent shells (e.g. EnvironmentRegistry/`current` + metaFull+normalize) no longer throw “must have a pool” |

---

## \*\*\* Experimental \*\*\* afwdev advanced-test (issue #157)

**Status: experimental** — ship for **comment and use**, not a frozen green contract. Marker names, schema, and runner behavior may change over the next months as maintainers exercise multi-request / process-lifetime tests (including work toward **#149** and **#2**). Many early design choices are expected to stick; treat the *capability* (hermetic server + multi-step fixtures) as durable, not every field name.

| | |
|--|--|
| **What** | Under `src/*/tests/`, a directory with **`advanced-test.yaml`** or **`advanced-test.json`** is one **leaf** test: harness starts installed **`afwfcgi`**, drives it with a FastCGI client, runs ordered **`eval` / `script`** steps, tears down. |
| **When it runs** | Normal **`afwdev test` / `afwdev test -j`** (default `--env-mode afw`). Requires **PyYAML** and **`afwfcgi` on PATH** (build with install). |
| **Examples** | `src/afw/tests/advanced/` (smoke, multi-request file adapter, multi-eval lifetime, JSON marker sample). |
| **How to write tests** | Builder page [`src/afw/doc/developer/writing-tests.md`](src/afw/doc/developer/writing-tests.md) (Doxygen related page **Writing tests** after docs build). |
| **Design / feedback** | [`designs/afwdev-advanced-test.md`](designs/afwdev-advanced-test.md), GitHub **[#157](https://github.com/afw-org/afw/issues/157)**. |

Not a replacement for ordinary `.as` test scripts. Live `--env-mode afwfcgi` still means “shared stack”; advanced leaves stay **hermetic** under default mode.

**[↑ Highlights](#highlights)**

---

## \*\*\* Experimental \*\*\* afwdev blast

**Status: experimental** — on-demand only; **not** part of `afwdev test -j`.

Randomly sends suite Adaptive `test_script` sources at **afwfcgi** for a duration and/or request count (language gate stays Jeremy’s `test`).

```bash
afwdev blast                    # :8080/afw, 5m, concurrency=2×CPUs
afwdev blast -d 30m             # short aliases; -c/-n override auto
afwdev blast -f path/to/afw.conf -m 500   # managed spawn (-n defaults to CPUs)
```

Defaults favor docker/dev + classic load (threads≈CPUs, in-flight≈2×CPUs). Fixture-heavy tests skipped unless `--include-fixtures`. Design: [`designs/afwdev-blast.md`](designs/afwdev-blast.md). Signals: [#158](https://github.com/afw-org/afw/issues/158).

**[↑ Highlights](#highlights)**

---

## afwdev test/blast recipe flags

**Status: experimental** (same family as advanced-test / blast).

| Flag | Who | Role |
|------|-----|------|
| **`-T` / `--tests-path`** | `afwdev test` and `afwdev blast` | Exclusive opt-in trees (e.g. `src/afw/tests_special/…`); default `test -j` never scans those roots |
| **`--output` / `--output-format`** | both | Write a machine summary (`json`, `json-compact`, or `text`) to a path or `-` |

Recipes: [`designs/afwdev-test-recipe.md`](designs/afwdev-test-recipe.md).

**[↑ Highlights](#highlights)**

---

## Runtime catalog / accessors (issue #149)

Child of **#2** memory work. Issue **#149** stays open for further accessor work.

| | |
|--|--|
| **`referenceCount` on catalog adapter / auth handler objects** (phase 1) | Snapshot under the existing anchor lock (no longer a racy live integer read on get) |
| **`_AdaptiveRuntimeValueAccessor_`** (phase 1) | First-class registry objects describing each named runtime value accessor (including whether it copies under lock / returns a live reference) |
| **Rich objectOptions on permanent / const shells** (phase 2) | `get_object` with **metaFull+normalize** (and similar) on large permanent views such as **`/afw/_AdaptiveEnvironmentRegistry_/current`** no longer fails with **Object must have a pool**. Option processing materializes mutable propertyTypes onto the **view pool** instead of writing permanent OT meta. |
| **Still open** | Further per-type accessor safety, full-registry materialize cost — see issue and [`designs/runtime-objects-and-environment.md`](designs/runtime-objects-and-environment.md) |

If you hold Adaptive values from `/afw/…` adapter/auth objects across **service stop**, treat **metrics** / **properties** as valid only while the instance is active (or you hold a session ref); **`referenceCount`** is a safe integer snapshot.

**[↑ Highlights](#highlights)**

---

## UTF-8 code-point sequences (issue #153)

Adaptive values whose internal form is **`afw_utf8_t`** (`string`, `anyURI`, and other utf8-backed types) are treated as **immutable sequences of Unicode code points** (not UTF-16 code units, not raw bytes).

| Topic | Behavior |
|-------|----------|
| **`length` / `substring` / `index_of` / …** | Indexes and length are **code points** (as before; kept consistent). |
| **`s[i]`** | Read one code point as a **one-code-point string**; out of range → **undefined** (soft, like arrays). Negative indexes count from the end. **No** `s[i] = …` (immutable). |
| **`for-of`** | Walks **code points** (same element shape as `s[i]`). Also still walks **arrays**. Not plain objects. |
| **Array formals / HOFs** | When a built-in expects an **array of values** (e.g. `map` / `filter` / `reduce`) or a script formal is **`array` / `T[]` / tuple**, a utf8 value is accepted as that sequence (temporary materialize). Does **not** permanently retype the string to `array`. **XACML bag** formals still treat a scalar string as **bag-of-one**, not code-point expansion. |
| **Search** | `includes` (with optional start), `replace`, and non-empty `split` search at **code-point boundaries**. |
| **Substring / replace / repeat returns** | Polymorphic string ops return **`string`**, not the input’s specialized utf8 type. |

Storage remains **valid NFC UTF-8**. The value’s **data type stays** `string` / `anyURI` / … — it is not retyped to `array`.

**C / extensions:** keyless **`afw_iterator`** + data-type `optional_initialize_iterator`; value helpers `afw_value_has_iterator` / `initialize_iterator` / `as_array_sequence`. Legacy opaque cursor type renamed **`afw_iterator_old`**. **Recompile** out-of-tree commands/extensions. Maintainer pad: [`designs/utf8-code-point-sequences.md`](designs/utf8-code-point-sequences.md). Tests: `src/afw/tests/language/script/string_code_points.as`.

Residuals (not required for this language story): lazy array **face** over utf8; shared `afw_utf8_*` index helpers; produce-type percolation on call IR (see `designs/compile-optimize-notes.md` / #28).

---

[↑ Highlights](#highlights)

## Array semantics (issue #39)

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

---

## Conversion functions (type-named)

Many Adaptive **data types** have a same-named **conversion function** `T(value)` that produces a value of type **T** (or fails with `cast_error`). Meta types (`any`, `undefined`, `void`, …) never had one.

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

---

[↑ Highlights](#highlights)

## Mutable object faces (issue #17)

> **Status:** **Landed on `mgg-develop`** via [PR #150](https://github.com/afw-org/afw/pull/150) (2026-08-06). Maintainer design pad: [`designs/issue-17-mutable-object-faces.md`](designs/issue-17-mutable-object-faces.md).

### What problem this is about

Adaptive Script often hands you an **object** or **array** that is really a **shared instance** or a **non-bag impl** under the hood:

- The same **object or array literal** in a function or loop can be **one bag** reused across evaluations, so mutating it “sticks” the next time.
- **Binding** used to **clone** as a safety net; that clone-on-bind is **gone** for objects **and** arrays — isolation comes from **faces** on literals, defaults, and script-facing returns.
- **Defaults** on helpers such as `property_get` / `variable_get` (issue **#110**) get a **mutable face** (not a deep clone of the whole graph).
- **Adapter get/retrieve** may return a **view** or other low-cost implementation. ECMAScript authors often expect “I got an object → I can set properties” and used to wrap in **`clone()`** by hand.

Product goal:

> When you work with an object (or array) in script, you should usually get a **mutable face that is safe for you to change**, without poisoning the next evaluation, the compile-time bag, or the shared base the platform still owns — and without needing `clone()` just to set a property.

Under the hood a face is a **memory wrapper** (local sets; get falls through or materializes entries; nested objects/arrays get **fresh faces** so typed `map` / index paths do not share nested bags). Authors do not need a wrap API for the happy path.

### Where you can drop manual `clone()`

| Path | Notes |
|------|--------|
| **Object / array literals** (const/let, returns, multi-call) | Platform isolates shared compile-time bags |
| **`get_object` / `get_object_with_uri`** | Mutable face over the adapter object (including views). **Exception:** `{ reconcilable: true }` keeps the entity/view for `reconcile_object` — use `clone()` if you also want a free-form mutable bag. |
| **`retrieve_objects` / `retrieve_objects_with_uri`** | Each object in the result array is a face |
| **`retrieve_objects_to_callback` / `_with_uri_to_callback`** | Object passed to the callback is a face |
| **`property_get` / `variable_get` object or array defaults** | Missing/unbound default is a **face** |
| **`journal_get_*`**, **consumer** gets, **after_cursor**, **advance** | Response objects are faces |

Example: `let o = get_object(...); o.foo = 1;` — no `clone(get_object(...))` required for that mutate-on-face pattern.

### `clone()` vs `freeze` vs `const` vs faces

| Tool | Meaning |
|------|---------|
| **Face (platform)** | Mutable local layer; base not poisoned; **not** a deep copy and **not** store write-through |
| **`clone()`** | Explicit **deep** independent copy of a graph (or when you still need a free bag over reconcilable/entity paths) |
| **`freeze`** | Explicit **immutability** of a value graph (or as documented for that API) |
| **`const`** | **Binding-level** only — the name cannot be reassigned; nested properties may still be mutable unless frozen |

**Still use `clone()` when:** you want a true deep independent copy; you need a free-form bag while keeping reconcilable identity separate; or you need a full snapshot for other reasons.

### What this is *not*

- **`retrieve_*_to_response` / `_to_stream`** — write/encode only; no script-owned face.
- **Journal entry returns** from add/modify/replace/… — fresh memory “receipts,” not store rows.
- **Faces are not write-through** to the adapter store — persist with add / modify / replace / update.
- **YAML conf / content-type parse** stays plain objects (no parse-time faces); script isolation for YAML-backed **file** data still goes through adapter get faces when applicable.
- **Runtime / `afw` catalog** objects (environment registry, live maps) are a separate lifetime topic — see issue **#149** (under **#2**), not this faces feature.

### Migration / habits

- Drop redundant `clone()` around **get_object / retrieve / callback / journal get** and literal isolation paths once you confirm behavior on this tree.
- Out-of-tree commands/extensions: rebuild if they link object/array face APIs; pure script authors follow this section.

---

[↑ Highlights](#highlights)

## Function reference prototypes (#28 spelling)

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

---

[↑ Highlights](#highlights)
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

[↑ Highlights](#highlights)
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

[↑ Highlights](#highlights)
## `variable_exists`: bound vs undefined (issue #131)

**Issue #131** on `mgg-develop` (branch `issue-#131-variable-exists`).

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

---

[↑ Highlights](#highlights)
## Object and array helpers (issue #55)

**Issue #55** — **closed** 2026-08-04 (landed via PR **#134** on `mgg-develop`).

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

[↑ Highlights](#highlights)
## Expression property names in object values (issue #38)

**Issue #38** — **closed** 2026-08-04 (landed via PR **#139** on `mgg-develop`).

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

[↑ Highlights](#highlights)
## Value lifetime / memory management (issue #2) — alpha/beta

**Issue #2** — work in progress on `mgg-develop` via branch `issue-#2` (partial land; design continues).

This is **not** a finished memory-management productization. Treat it as **alpha/beta** on the maintainer develop line: useful foundation and mostly behavior-compatible for in-tree tests, but the long-running escape / assign / scope-release story is **not** complete.

### What landed so far (high level)

- Prefer **shared permanent Adaptive values** (generate bag / `afw_v_*`) for known scalars instead of allocating fresh ones where safe (null, boolean true/false, many const_objects properties).
- **Object and array instances** more consistently expose a dual Adaptive value face (`->value`) with a lifetime-matched permanent/managed/unmanaged inf.
- **Managed object values**: container-aware `optional_release` / `clone_or_reference` paths that do **not** free an embedded dual-face header; `create_managed_object` requires a non-null object and takes a container hold.
- **`afw_pool_release`**: returns the pool if still referenced, or **NULL** if that call destroyed the pool (C API; ignore return if you do not care). Used so managed **object faces** can hold one reference on the wrapped base and drop it only when the face pool is destroyed. Unmanaged faces still borrow. Array faces remain pool-owned for now.
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

[↑ Highlights](#highlights)
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

[↑ Highlights](#highlights)
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

[↑ Highlights](#highlights)
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

[↑ Highlights](#highlights)
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

[↑ Highlights](#highlights)
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

[↑ Highlights](#highlights)
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

[↑ Highlights](#highlights)
## Process environment variables (issue #71)

`retrieve_objects("afw", "_AdaptiveEnvironmentVariables_")` now returns a **single** `current` object (process environment), not two identical ones. The `environment::` qualifier is unchanged.

Property **values** from the host environment are Adaptive **string** when the bytes are valid UTF-8 (NFC), otherwise **hexBinary** (raw octets preserved). Property **names** that are not valid UTF-8 appear as `_NONUTF8_` plus uppercase hex of the raw name. A bad value no longer prevents AFW from starting.

Request CGI/FCGI-like parameters remain under `_AdaptiveRequestProperties_` / `request::` (separate from process env).

---

[↑ Highlights](#highlights)
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

[↑ Highlights](#highlights)
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

[↑ Highlights](#highlights)
## `stringify`, `decompile`, compiler listing, and binary text

**Issue #18** (stringify second parameter / replacer) — **closed** 2026-08-04 (PR **#137**; re-verified). Broader decompile/listing work remains documented here for users.

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

[↑ Highlights](#highlights)
## Adaptive Script types (issue #28)

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

When checking is on, assignment and script function parameters are checked for leaf data types, unions/intersections, **object/interface shapes** (required props, property types, `extends`), **array elements** and **tuple length/positions** when the value is known (for example a literal), **function param/return shapes** (script functions/closures), **Pattern** element annotations on array/object destructure, and **calls to known Adaptive functions** against metadata formals (compile path only). **Object literals** may not include properties outside the declared type (including as arguments); spreads/computed keys skip that extra-key check. At runtime, non-literal values may still be wider (structural). Type errors name missing properties, element indexes, and decompiled expected types where possible.

Handbook: Language Reference **Types** (flags and `#compile` options), **Language Features** Pragma (short `#compile` example), **Lexical** reserved names. Tests: `type_syntax.as`, `type_check_flags.as`, `type_check.as`, `type_check_multi_unit.as`.

---

[↑ Highlights](#highlights)
## Conf path templates (issue #15)

**Issue #15 closed** (2026-08-04) after PR **#135** on `mgg-develop`. Path-like conf properties use **`template`** (not hybrid) by design. No further conf host-path residuals planned at this time; curl `caInfo`/`caPath` remain plain runtime options.

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

[↑ Highlights](#highlights)
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

[↑ Highlights](#highlights)
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

[↑ Highlights](#highlights)
## Pure-script model adapters

**Issue #109 / PR #119**

A model adapter still loads an `_AdaptiveModel_` from `modelLocationAdapterId` / `modelId`. What changed is the backend requirement:

- **Hybrid (unchanged):** set `mappedAdapterId` and map-and-forward (or fall through with `current::useDefaultProcessing` / `model_default_*`).
- **Pure-script (new):** implement every operation you use in model object-type `on*` scripts and **omit `mappedAdapterId`**. No dummy file/LMDB backend is required.

If an operation needs default processing and `mappedAdapterId` is missing, AFW fails with a clear error (implement the `on*` hook or set `mappedAdapterId`).

In scripts, `current::mappedAdapterId` is nullish when unset (it does not throw).

`modelId` and `modelLocationAdapterId` remain required for both hybrid and pure-script models.

---

[↑ Highlights](#highlights)
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

[↑ Highlights](#highlights)
## `afw --allow` and YAML value output (issue #14)

**Issue #14** — **closed** 2026-08-04 (feature long on tree; regression suite on `mgg-develop`; re-verified 22 yaml tests)

**Beta hygiene (this branch):**

- `afw_yaml_to_object` (**`raw_to_object`**) fixed so **file adapters with `contentType: yaml`** work; libyaml parser always deleted.
- Plain scalars: full-string **integer vs double** (`afw_number_parse`), `true`/`false`/`null`/`~`; quoted/literal/folded always strings; partial numbers like `123foo` stay strings.
- Empty array/object emit **`[]` / `{}`**; mapping value without key errors.
- No parse-time issue-#17 faces (conf/store plain; adapter/journal faces on return).

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
| **`yaml_to_object.py`** | `raw_to_object` / file adapter `contentType: yaml` get/add, non-mapping root reject, #17 face isolation on get |

```bash
afwdev test -p afw_yaml --show-all
# or
afwdev test -p afw_yaml --tags yaml
```

Handbook `usage.xml` may still omit `-a` until a docs pass; live **`afw -h`** lists it.

---

[↑ Highlights](#highlights)
## JSON Schema for Adaptive object types

**Issue #3 / PR #116**

Package-generated schemas under `generated/schemas/afw/` are improved for **editors** (e.g. VS Code completion, hovers, light validation) and for `afwdev validate`:

- No more invalid “`$ref` mixed with sibling keywords” shapes that break validators.
- Entity schemas promote type/properties to the document root for better editor apply behavior.
- Inheritance merges parent `propertyTypes` with **child overrides** so nested models validate correctly.
- `defaultValue` / possible values map more cleanly to JSON Schema `default` / `enum`.

If you edit Adaptive object JSON under `generate/objects/` (or rely on schema-backed validation), regenerate so your tree picks up the new schemas (`./afwdev build --cdev` or your usual generate path). Do not hand-edit `generated/schemas/`.

---

[↑ Highlights](#highlights)
## Reliability notes

### Default values from `property_get` / `variable_get`

Mutable defaults for `property_get` / `variable_get` (issue **#110**) are isolated so later mutations do not poison other calls (important in long-running hosts and model `on*` handlers). On this branch, object/array defaults use a **memory face** (`afw_value_isolate_mutable_default`) rather than a full structural clone; scalars still clone. Regression tests in `property_get.as` / `variable_get.as` lock the isolation behavior.

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
| Object / array helpers | #55 (closed) | #134 |
| `afw_crypto` + secrets composition | #74 (partial; stays open for readpass) | #136 |
| `stringify` / `decompile` / listing / binary text | #18 (closed) | #137 |
| Expression property names in object values | #38 (closed) | #139 |
| Param / catch Patterns + call-site spread | #140 (closed) | #141, #142 |
| `variable_exists` bound vs undefined | #131 (closed) | #146 |
| UTF-8 JSON emitters + Python local FIFO | — | on `mgg-develop` (post-#142) |
| Permanent `src/*/tests` regression assets | — | #121 (docs only) |
| `afw --allow` + YAML block strings / integers | #14 (closed) | regression tests on `mgg-develop` |
| Meta on the wire / reserved `"_meta_"` (design) | #138 | — (open; not required for #38) |
| Adaptive Script types | #28 (open; core shipped) | issue-#28 / #145 line on `mgg-develop` |
| Mutable object faces (shared instances) | #17 (closed) | PR **#150** → `mgg-develop` (this file + `designs/issue-17-mutable-object-faces.md`) |
| UTF-8 code-point sequences (`s[i]`, for-of, formals) | #153 | issue-#153 branch → `mgg-develop` (this file + `designs/utf8-code-point-sequences.md`) |

---

## How this was produced

Diff basis: `git log develop..mgg-develop` and the corresponding code/metadata changes (including PRs **#116**–**#124**, **#128**–**#130**, **#132**–**#142**, and follow-up fixes on `mgg-develop`). For full commit history, see those PRs on the repository hosting Adaptive Framework.

### Maintaining the Highlights table (for agents)

When adding or rewriting `whats-new.md` content:

1. **Every user-facing theme gets a `##` detail section** (and optional `###` subsections). Put the short pitch in the Highlights table and the full story under the section.
2. **Link the Area cell** to that section: `[**Short name (#N)**](#github-heading-slug)`. Leave “What changed” unlinked.
3. **End each detail `##` section** with `[↑ Highlights](#highlights)` (not after every `###`). Skip meta sections (Reliability, Breaking checklist, Related issues, How this was produced).
4. **Slug** = GitHub auto-anchor for the `##` title: lower-case, strip punctuation/`backticks`, spaces → `-` (verify in the rendered page if unsure). Prefer **stable, boring headings** so slugs don’t thrash.
5. **One theme, one primary section.** If two table rows share a section (e.g. UTF-8 + Python local), both may link to the same `#…`. Prefer a `###` link only when the jump is much clearer.
6. **No orphan links.** If there is no detail section yet, either add one or leave the Area unlinked until there is.
7. **After renames**, update Highlights links in the same edit as the heading change.
