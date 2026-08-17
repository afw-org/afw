# Adaptive Script and TypeScript / JavaScript

**Audience:** maintainers and AI assistants working on **`develop`** (beta readiness); useful secondary reading for script authors who already know TypeScript.  
**Status:** living decision notes. Not polished product handbook. Update when we learn or fix something.

---

## Purpose (read this first)

**Adaptive Script is not TypeScript and not JavaScript (ECMAScript).**

It is a **compiled** language hosted by Adaptive Framework (C runtime, Adaptive values/functions, qualified variables, adapters/models). It is not a browser or Node runtime and will not grow a global object, prototypes, or an ambient module system to look like those hosts.

**Product goal for shared surface:** when Adaptive **supports** TypeScript- or JS-looking syntax (or a built-in that plays the same role), it should behave as a TypeScript-literate author would reasonably expect **for that construct**. Prefer **motivated** Adaptive differences over accidental ones. Document deliberate non-goals instead of half-implementing the JS platform.

### How this file is used on `develop`

`develop` work is aimed at getting AFW **beta-ready** (or closer). Sessions will include language and built-in fixes that are **not always tied to a single GitHub issue**. This file records:

| Section | Use when deciding… |
|---------|---------------------|
| [What already feels TypeScript-shaped](#what-already-feels-typescript-shaped) | What we treat as supported / don’t regress |
| [Should fix](#should-fix-typescript-shaped-alignment-backlog) | Unmotivated gaps worth fixing for beta (syntax, builtins, docs) |
| [Explicitly will not do](#explicitly-will-not-do) | Platform and sugar we refuse so we don’t thrash |
| [Motivated differences (not bugs)](#motivated-differences-not-bugs) | Intentional Adaptive behavior someone might mis-file as a bug |

GitHub issues still matter; this **augments** them with standing decisions and a backlog that can outlive or precede issue text. When a fix lands, move it out of Should fix and, if user-visible, note it in `whats-new.md` as usual.

### Not Jeremy’s differences document

Jeremy has (or will have) his own work to finish a differences document aimed more at authors (see GitHub **[#22](https://github.com/afw-org/afw/issues/22)** — Adaptive Script vs ECMAScript). **That is not what this file is.**

This file is maintainer/beta **decision hygiene** while we implement and align. He may find it useful as raw material when he writes the polished doc. Core Language Reference should still describe Adaptive primarily on its own terms, without requiring ES/TS vocabulary.

---

## Mental model

| | TypeScript / JavaScript (typical) | Adaptive Script |
|--|-----------------------------------|-----------------|
| Execution | Engines with mutable globals, prototypes | **Compile** to Adaptive value graphs; evaluate in **xctx** scopes / pools |
| Values | Language objects + builtins on prototypes | **`afw_value`** + registered **data types** + **Adaptive functions** |
| “Methods” | Often `obj.method` via prototype / class | **`fn(value, …)`** or **`value->fn(…)`** when the function is a data-type method |
| Names outside locals | Global object, imports, `this` | **Lexical** `let`/`const` + **qualified variables** `qualifier::name` |
| Types | Full TS checker (or none in JS) | Annotations always parse; **checking opt-in** |
| Modules | `import` / `export` | No script module import; conf, qualifiers, Adaptive APIs |
| Classes / `new` | Central | No classes, no `new` / prototypal inheritance |
| Async | `async`/`await`, Promises | No async/await (keywords may be reserved) |

Script **types** (`type` / `interface`, shapes, unions) are for script checking. They are not adapter **object types** / OT catalogs. Adaptive data types and functions remain environment-first (including XACML-facing heritage where relevant).

---

## What already feels TypeScript-shaped

Supported surface we intend to keep. Prefer these when writing script that should feel natural next to admin TypeScript.

### Bindings and control flow

- **`let` / `const`** (no `var`). Semicolons **required**. Bare expressions are not statements. One statement may declare several names (`let a = 1, b = 2` / `const a = 1, b = 2`; **#62**).
- **`if` / `while` / `do` / `for` / `for-of` / `switch` / `try` / `throw` / `return` / `break` / `continue`**. C-style **`for` init** is one `let` / `const` (including `for (let i = 0, j = 1; …)`) or assignment(s); not `for (let i = 0, let j = 1; …)`. **Loop labels** (`outer: for` / `break outer` / `continue outer`) as in TS/JS; **not** on blocks or `if`. One label per loop.
- **`for-of`** over **arrays** (by element) and **strings** (by Unicode **code point** / UTF-8 character). Objects: walk **`keys` / `values` / `entries`** (no `for-in`).
- Nested **`function`** values and **closures** that capture enclosing bindings (runtime + `closures.as` — **25** pass; **11** skips are escape/lifetime under **#2**, not “no closures”). Issue **#35** left open for that residual bar. Handbook Features **Closure** section updated (no longer claims “no closures”).

### Nullish and short-circuit forms

- **`null`**, **`undefined`**, **`true` / `false`**.
- **`??`** nullish coalescing; **`?.`** optional chaining (forms the parser accepts).
- **`is_nullish(v)`** / **`is_defined(v)`** for values.
- **`property_get` / `variable_get`** with defaults when the **name/key is missing** (not when the value is undefined). **`variable_exists` / `property_exists`** mean bound/present, not “value defined.”

### Objects, arrays, Patterns

- Object/array literals; property names may be identifier, string, or **`[expression]`** (**#38** closed).
- **Destructuring Patterns** on `let`/`const`, assignment (object Patterns often need parens), `for`/`for-of` heads, **function parameters**, and **`catch`** (**#140** closed).
- Options-object style:

  ```adaptive
  function connect({ host, port = 443 } = { host: "localhost" }) {
      return host + ":" + string(port);
  }
  ```

- Rest/spread: **`...rest` formals**, call-site **`f(...arr)`**, rest in Patterns.
- Helpers as Adaptive functions (**#55** closed): `keys`, `values`, `entries`, `at`, `push`/`pop`/`shift`/`unshift`, `splice`, `freeze`, `every`/`some` (also `->` method sugar when registered). Keep XACML-shaped names (`all_of` / `any_of`, …) as first-class too—not a rename-everything-to-JS project.
- Out-of-range **`at`** / bracket **`a[i]`**, empty **`pop`/`shift`**: **undefined** (nullish), not throw.
- Array literal **elision** (`['a', ,'b']`) → dense **undefined** slots (script / relaxed_json; not strict JSON). Assign **`a[length] = x`** appends; no gap fill. **`create_array(n)`** pre-sizes with undefined.

### Functions and exceptions

- **`function`** declarations and function values; Expression parameter defaults (prior params visible).
- No ES **`arguments`** — use **`...rest`**. No meaningful **`this`**.
- **`throw "message"`** + optional data; **`catch (e)`** or **`catch ({ message, data })`**. Fixed Adaptive error shape, not arbitrary JS throwables.

### Types (**#28**)

TypeScript-like **spelling** where Adaptive supports it; Adaptive **semantics** and **opt-in** checking. Prefer one clear form over dual TS sugar.

| Feature | Adaptive |
|---------|----------|
| Annotations | `: Type` on bindings, params, returns, Pattern leaves |
| Arrays / tuples | **`T[]`**, fixed **`[T, U]`** (not `Array<T>` — see [motivated differences](#motivated-differences-not-bugs)). No optional/rest/labeled tuple forms (see [will not do](#explicitly-will-not-do)) |
| Unions / intersections | `A \| B`, `A & B` |
| Object shapes / optional props | **Closed** lists: `{ host: string, port?: integer }` — known names only (optional with `?`). **No** index signatures. Callable fields use a **function Type** after the colon (`run: (a: integer) => integer`), not TypeScript method/call signatures (see [will not do](#explicitly-will-not-do)) |
| Function **types** | `(a: integer) => integer` |
| Function **values** | `function (a: integer): integer { … }` |
| Aliases / interfaces | `type`, `interface` + `extends`. **Declared before use** (not hoisted). A type may refer to **itself** in its own body. See [motivated differences](#motivated-differences-not-bugs) |
| Checking | Off by default; `compile:typeCheck*`, `noImplicitAny`, `strictNullChecks`, `strict`; `#compile` / `noTypeCheck` |
| Old Adaptive Type spelling | Hard cut: no `(array of T)`, no `(object "SomeObjectType")` |

Leaves are Adaptive **data types** (`integer`, `string`, …), not TypeScript `number`, DOM libraries, or **literal types** such as `"read"` or `1` (see [will not do](#explicitly-will-not-do)).

#### Compile-time vs runtime (Adaptive values — overall)

TypeScript authors often assume “types mean mostly compile-time.” Adaptive splits the work:

| Job | When it runs | Role |
|-----|----------------|------|
| **Script type checking** | Off by default; when on, compile if the type is known, and (unless compile-only) also at run time for assigns / script parameters / returns | Script-local shapes and data types you wrote in the script |
| **Call-site formals** | Under typeCheck: bindings with a **function Type** annotation are checked against that annotation; script **definition** early-bind (optimize/formals from the implementation) only for **`const` / `function`**, not **`let`** (reassignment must remain valid) | Prefer `const f: (a: T) => R = …` or `function f…` when you want both annotation and known definition |
| **Adaptive function parameters** (built-ins) | Extra checks at **compile** when the function and argument types are known; HOF **FunctionSignature** strings project to function Types; **run time** still does the usual Adaptive convert and implementation checks | Not a second full type system on every execute |
| **Adaptive objects** (object types, property meta, write rules such as allow write) | **Run time** (and the admin UI from meta) when the object and its type/meta are available | Catalogs, adapters, and models are dynamic — not fully imported into the script type checker at compile |

So: optional script types for local contracts; Adaptive functions keep runtime behavior with earlier compile help when possible; Adaptive object rules stay meta- and write-path-driven. Longer maintainer note: [`designs/issue-28-type-syntax.md`](designs/issue-28-type-syntax.md).

**Admin / client note (decision hygiene):** the Adaptive JavaScript client models open Adaptive objects with TypeScript index signatures and mostly **data-kind** types (`string`, unions of classes, occasional `enum`). The admin app is largely JavaScript + PropTypes. That host-app style informs “what AFW developers reach for”; it does **not** mean Adaptive Script must grow the same type machinery. Script types stay the small surface in the table above.

### Host context (not globals)

- **`qualifier::name`** for host/context data (`current::`, `process::`, `environment::`, …).
- **`qualifier` / `qualifiers`** snapshots (**#9**): fresh objects, can be large; missing → nullish, not `{}`.

---

## Should fix (TypeScript-shaped alignment backlog)

**Unmotivated** gaps: same or analogous syntax / builtins should not trap a TS-literate author. Work these down for beta readiness whether or not a single issue owns the whole row. When fixed, remove or strike the item and add tests; link issues when they exist.

### Priority guide

| Priority | Meaning |
|----------|---------|
| **P0** | Wrong “exists” / binding story; confuses optional params and undefined |
| **P1** | Same root cause in type-check or get-defaults; misleading function briefs |
| **P2** | Docs lag, polish, long-burn test conversion |

### Bindings and reflection (syntax + builtins)

| Item | Priority | Notes / issue |
|------|----------|----------------|
| **`variable_exists`** = name **bound**, not “value pointer non-NULL” | **Done** (branch `issue-#131-variable-exists`) | Uninit `let` / optional formals / explicit undefined all bound. **#131** |
| **`variable_get(name, default)`** default only if **not bound** | **Done** | Uninit no longer takes default; missing name still does. |
| Optional formals: binding exists when arg omitted | **Done** | Covered in `variables.as`. |
| Type-check: C **NULL** uninit vs **`undefined` singleton** | **Done** | Assignability treats C NULL as undefined (same nullish / `strictNullChecks` rules). |
| Light Adaptive function **descriptions** | **Done** | `variable_*`, `property_*` exists/get/is_not_null, `is_defined`, `is_nullish` briefs. |

**Semantics (landed):**

| API | Meaning |
|-----|---------|
| `variable_exists("name")` | Bound (lexical symbol or defined on a qualifier frame), including value undefined/null |
| `is_defined` / `is_nullish` | Value |
| `property_exists` | Key present (already true for undefined values) |
| `variable_get` / `property_get` + default | Default only if name/key **missing**, not if value is undefined |

### Built-ins and authoring experience

| Item | Priority | Notes |
|------|----------|--------|
| Shared script default `{}` / `[]` | P1/doc | Literal defaults may be shared compiled values; built-in get defaults are cloned (**#110**). Don’t re-break; document in briefs if needed. |
| Familiar names where Adaptive already has the concept | Ongoing | Prefer adding thin aliases (`every`/`some`) over forcing only XACML spellings; do **not** delete heritage functions for beta. |
| `evaluate` vs JS `eval` | P2/doc | No free outer locals; say so in description if anyone confuses them. |

### Handbook and tests lag

| Item | Priority | Notes |
|------|----------|--------|
| Features / Types | Ongoing | Keep Adaptive-first; deep ES essays belong in Jeremy’s #22 doc or here as maintainer notes. Features **Closure** handbook text corrected 2026-08-10. |
| `test262/` skips | Long-burn | Convert when Adaptive behavior is decided; permanent non-support → [Will not do](#explicitly-will-not-do) or Jeremy’s doc—not silent forever skips. Case-level done log since `mgg-develop` fork: [`src/afw/tests/test262/changes.md`](src/afw/tests/test262/changes.md). |

### Types residuals (alignment, not full TS)

| Item | Priority | Notes |
|------|----------|--------|
| Pattern annotation checking coverage | Low | Leaf annotations when the pattern binds (shipped with **#28**). Not a full TypeScript Pattern checker; no promise of deeper structural pattern types without a new decision. |
| Surprising assignability vs runtime nullish | P1 | Same as C NULL / uninit above. |

---

## Explicitly will not do

How Adaptive Script **works** for these items — not a temporary “not yet” list. Changing any of this is a **new product decision** (typically a new issue), not silent drift. Do not implement “a little JS runtime” under the banner of TypeScript-shaped syntax.

### Platform (hard no)

| Non-goal | Why |
|----------|-----|
| Global object / `globalThis` / free assignable globals | Host model is qualifiers + Adaptive APIs |
| Prototypes, `class`, `new`, `super`, mutable builtin constructors | Not an ES object model; methods are Adaptive functions |
| `this` binding rules | No meaning in Adaptive Script |
| `import` / `export` / npm-style modules | Share via framework/conf/qualifiers |
| `async` / `await`, Promises, ES generators as the concurrency model | Not the Adaptive host model |
| ES `arguments` object | Use formal `...rest` |
| Becoming a JS engine or browser/DOM runtime | Out of scope for AFW core |

### Language sugar we are not chasing (unless product reopens)

| Non-goal | Notes |
|----------|--------|
| Arrow **functions** as values (`=>`) | Explicit non-goal for now; **`=>` in function types** is supported |
| `var`, automatic semicolon insertion, expression-as-statement | Semicolons required; statements are statements |
| `for-in` | Use `keys` / `values` / `entries` + `for-of` |
| Full advanced TypeScript type system | No merge-bar for generics, `keyof`, conditionals, mapped types, `ReadonlyArray`, etc. |
| TypeScript **`Array<T>`** type spelling | Use **`T[]` only**; same meaning in TypeScript, one form in Adaptive |
| **Index signatures** on object types (`{ [key: string]: T }`, numeric keys, or known fields plus an open tail) | Script object types list **known properties** only. Free-form bags: use data types **`object`** or **`any`**, or leave type checking off for that unit. Schemas the framework owns: **Adaptive object types** and adapters — script `interface` is not that catalog. The Adaptive **JavaScript client** may use index signatures when typing open Adaptive objects in the host app; that is a different layer. The index form is not Adaptive Script type syntax. |
| **Literal types** (`"read"`, `1`, `true` as types; unions like `"a" \| "b"`) | Type leaves are Adaptive **data types** only. There is no TypeScript-style value-level type lattice and no control-flow narrowing for discriminants. Use `string` / `integer` / `boolean` (and ordinary unions of those); check specific values at run time when needed; framework vocabularies often live as **allowed values** on Adaptive property / object types (same spirit as generated UI props that stay `string` with a prose list). |
| **`readonly` in script types** (properties, tuples, arrays, `ReadonlyArray`-style forms) | Not part of the Adaptive Script type language. Non-writable **Adaptive object** properties use Adaptive object types / property meta (for example **allow write**), enforced on **write paths at run time** and reflected in the admin UI — not a script type modifier. In script, use **`const`** and runtime helpers such as **freeze** for bindings and values. |
| **Richer tuple types** (optional elements such as `[T, U?]`, rest such as `[T, ...U[]]`, labeled `[x: T, y: U]`) | Adaptive Script has **fixed-length** tuples only: `[T, U, …]` with length and per-position checks. Open-ended sequences use **`T[]`**. Optional structure often fits an object shape better than a variable-length tuple. Rest on **function** parameter types is separate and already exists. |
| **Method / call / construct signatures in object types** (`{ run(a: integer): boolean }`, `{ (): void }`, `{ new (): T }`) | Object types list **properties** only (`name` / `name?` + `:` + Type). For a callable property write a **function Type**: `run: (a: integer) => boolean` — same idea as built-in function-parameter prototypes in the Function Reference. Adaptive has no class / `new` object model; methods are Adaptive functions (and optional `->` sugar), not prototype members. One form, not TypeScript method shorthand. |
| **Type assertions, `satisfies`, type predicates, assertion functions** (`x as T`, `satisfies`, `x is T`, `asserts x is T`, non-null `!`) | Not part of Adaptive Script. There is no TypeScript-style assertion or control-flow narrowing lattice. Declare types with `: Type` and use type checking when it is on and types are known. At run time use conditions, Adaptive helpers, and `throw`. |
| **`enum` / `const enum`** | Not part of Adaptive Script. Use Adaptive data types and ordinary values; constrained vocabularies often live as **allowed values** on Adaptive property / object types (or runtime checks), not an `enum` type form. |
| **Interface / declaration merging** | One `type` or `interface` definition per name in a compile unit; redeclaration is an error. No TypeScript declaration merging. |
| **`implements`** | No classes or `new`. Script `interface` is a structural type shape only — nothing implements it in a class sense. |
| **Import types / type-only `import` / `export`** | No script module system. Share configuration, qualifiers, Adaptive APIs, and (later) application-shared functions — not TypeScript modules. |
| **`never`** | No TypeScript bottom type `never`. Adaptive data types include `void`, `unknown`, `any`, and the rest of the catalog as defined. |
| **`typeof` / `instanceof`** (value operators **and** TypeScript **type-query** `typeof`) | Keywords may be **reserved** so mistaken copy-paste fails early (**reserved ≠ implemented**). Adaptive Script does **not** implement the JavaScript `typeof` / `instanceof` operators, and does **not** support TypeScript type queries such as `type T = typeof x` or `: typeof y`. Write types with Adaptive data type names, structural types, and script `type` / `interface` aliases. Host TypeScript/JavaScript may still use value `typeof` in the admin client — different layer. |
| TDZ (ReferenceError on uninit `let`/`const`) | Adaptive reads uninit as undefined today; changing that is a product decision, not an assumed “fix” |

### Not “make every name look like JS”

- Keep Adaptive / XACML-shaped function ids where they are real product surface.
- test262 path names like `Array.prototype.*` are **historical labels** only—not a promise of prototype APIs.

---

## Motivated differences (not bugs)

Same or similar spelling, **intentional** Adaptive behavior. Do not “fix” these toward JS unless product changes the language model.

| Topic | Adaptive choice |
|-------|-----------------|
| **`===` / `!==` on objects and arrays** | **Structural** (deep) equality, not reference identity |
| **Uninitialized `let`** | Readable as **undefined** (no TDZ); self-init `let x = x` is allowed |
| **`const` reassignment** | **Rejected** (`Cannot assign to const variable "…"` / `read_only`); for-of **rebinds** const head each iteration without treating that as user assign |
| **Assignment is a statement, not an expression** | **Decided not to nest.** A **statement chain** `x = y = 1;` (and compound `+=` / `??=` on the rightmost assign) is supported (**#62**). ES also allows values like `(x = 1) > x`. Adaptive does **not**: assignment inside a larger expression is almost always a typo (`=` vs `===` / `==`). `x = (y = 1)` / `let x = y = 1` stay illegal (`let`/`const` RHS is still an Expression). Order-of-evaluation tests use throw/`safe_evaluate` side effects instead. Not a compatibility gap to close. |
| **`throw` / `catch`** | String message + optional data; fixed catch object shape |
| **Type checking** | **Opt-in** (not always-on `tsc`) |
| **Outside names** | **Qualifiers**, not globals |
| **Methods** | Adaptive functions + optional `->` sugar, not prototype walk |
| **Script types vs Adaptive object types** | Script `interface` ≠ adapter object type catalog |
| **`type` / `interface` names are not hoisted** ([#188](https://github.com/afw-org/afw/issues/188)) | A name must already be declared in this compile unit (or be an Adaptive data type), like `let` / `const`. TypeScript hoists type aliases and interfaces so a later `type B` can satisfy an earlier use; Adaptive does **not**. **Self-ref in the same statement is allowed** (`type Node = { next?: Node }`). Unknown names are a compile error even with type checking off. Mutual / forward types (`type A = { b: B }; type B = { a?: A }`) are not supported — reopen only if we want an explicit mutual form. Bare alias cycles (`type A = A`, or a cycle only through union / intersection) are errors. |
| **Array element types** | **`T[]` only** — not `Array<T>`. TS treats both as the same; Adaptive keeps a single spelling (matches typical app/client `string[]` style and avoids a second path into generics) |
| **`null` vs `undefined`** | Both nullish; Adaptive `null` is a typed singleton; C APIs may use NULL for undefined—script authors should think in nullish + exists/get rules above |
| **String encoding** | Adaptive is **100% UTF-8** end-to-end. **`afw_utf8_t` values are always valid NFC UTF-8** (invalid UTF-8 must not appear as a string value—that’s a serious bug). ECMAScript engines typically expose a **UTF-16 code unit** model (one logical character may be two 16-bit units / a surrogate pair). Adaptive **length**, **substring**, **index_of**, **for-of** (strings), and empty-separator **split** walk **Unicode code points** in that UTF-8, not UTF-16 code units and not “raw octet indexes” for character steps. |
| **`\u` / `\u{…}` / `\xHH` / `\0` / identity / line-continuation escapes** | Script string escapes support **`\u`**, **`\u{…}`**, **`\xHH`**, **`\0`** (null, not followed by a digit), classic singles (`\n`, …), **identity NonEscapeSequence** (`\A` → `A`), and **LineContinuation** (`\` + LF/CR/LS/PS contributes no characters). Digits **`1–9`** after `\` are still invalid (no legacy octal). The **runtime value** is still UTF-8, not a UTF-16 buffer. |
| **Leading/trailing-dot numeric literals** | Same family as ES **DecimalLiteral**: `.5`, `.1e1`, `1.`, `1.e10` are valid (in addition to `0.5` / `1.5e1`). Integers without `.`/`e` stay `integer`; forms with fraction or exponent are `double`. |
| **Substring search (`replace`, non-empty `split` sep, …)** | May scan UTF-8 **octet-by-octet** to find a well-formed needle via `memcmp`—valid for well-formed UTF-8 (false mid-sequence starts won’t match a multi-byte needle). That is search, not “character length = octets.” |
| **Empty-match `replace`** ([#190](https://github.com/afw-org/afw/issues/190)) | An empty `match` is a match at a **code-point boundary** (including start and end). Default `limit` 1 inserts at the start (`"xabc"`). `limit = -1` inserts at every boundary (`"xaxbxcx"`). Same family as ES `replace` / `replaceAll` with `""`, except Adaptive walks **code points** (a supplementary character is one insert-around, not a split surrogate pair). Empty-separator `split` already splits by code point. |
| **Utf8 sequence plan** | Index + array consumers for all utf8-backed types (not retype to `array`): **[#153](https://github.com/afw-org/afw/issues/153)** / [`designs/utf8-code-point-sequences.md`](designs/utf8-code-point-sequences.md) |
| **Closure lifetime** | Supported; long-running hosts still care about pools / **#2** |
| **Default `{}` / `[]` literals** | May be shared when used as script defaults—by-design hazard unless we later change compile policy |

Strings stay **immutable** (no `s[i] = …`). Character access is via functions (`substring`, …) or **for-of**, not bracket mutation.

---

## Quick “how do I write X?” map

| You want (TS habit) | Adaptive |
|---------------------|----------|
| Options bag parameter | Pattern formal + default `= {}` |
| Optional property type | `port?: integer` |
| `obj.method(a)` | `method(obj, a)` or `obj->method(a)` when available |
| `Object.keys` / `entries` | `keys(o)` / `entries(o)` |
| Array stack ops | `push` / `pop` / `shift` / `unshift` / `splice` / `at` |
| `arr.every` / `some` | `every` / `some` (also `all_of` / `any_of` family) |
| Rest / spread | `...rest` formal; `f(...arr)` at call |
| `x ?? y` / `o?.p` | Same operators |
| Catch fields | `catch ({ message, data })` |
| Process / env | `process::…`, `environment::…` |
| Typecheck like `tsc` | Opt-in flags / `#compile typeCheck` |
| `type` / `interface` used above its declaration | Declare the name **first**. Same-statement self-ref (`type Node = { next?: Node }`) is the exception |
| Modules / classes | Framework + objects/functions—not `import` / `class` |

---

## Reserved words (subset)

Many JS/TS keywords are reserved even when unimplemented (`class`, `async`, `await`, `import`, `export`, `typeof`, `var`, `with`, `this`, …) so copy-paste fails early. See Language Reference **Lexical**. **Reserved ≠ implemented.**

---

## Related materials

| Resource | Role |
|----------|------|
| Language Reference (handbook) | Normative Adaptive description |
| [`whats-new.md`](whats-new.md) | User-facing `develop` notes |
| [`beta-backlog.md`](beta-backlog.md) | Maintainer brain dump |
| [`designs/`](designs/) | Issue/theme pads (#28, #140, pragma, …) |
| [`.cursor/rules/afw-adaptive-script.mdc`](.cursor/rules/afw-adaptive-script.mdc) | Authoring quirks |
| [`.cursor/rules/afw-qualified-variables.mdc`](.cursor/rules/afw-qualified-variables.mdc) | Qualifier get vs snapshots |
| GitHub **#22** | Jeremy’s ES differences **document** work (separate from this file) |
| **#35** (open); **#28**, **#14**, **#18**, **#38**, **#55**, **#131**, **#140**, **#9**, **#110** (closed) | Closures residuals (#2 escape) vs landed types, helpers, exists, Patterns, snapshots, get defaults |

---

## How to maintain this file

1. **Fixed an unmotivated gap?** Remove it from Should fix; add a line under “already feels TypeScript-shaped” if it is new surface; tests + whats-new when user-visible.  
2. **Rejected a TS/JS request?** Add it under Explicitly will not do (or Motivated differences) with one line why.  
3. **Not sure yet?** Leave under Should fix with “decide” priority, or park in `beta-backlog.md` / a design pad—don’t pretend it is Will not do.  
4. Prefer short Adaptive examples over long ES essays.  
5. Do not replace the handbook or Jeremy’s eventual #22 author doc; keep this as beta/maintainer decision support.

---

## One-sentence summary

**Adaptive Script is its own compiled language:** reuse TypeScript-looking syntax and familiar builtins where they map cleanly; fix accidental traps for beta; refuse the JavaScript platform model so AFW stays metadata-driven, host-safe, and consistent.
