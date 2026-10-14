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
- Nested **`function`** values and **closures** that capture enclosing bindings (runtime + `closures.as` checklist green). Store-time bind for literals and nested assign is **#35** (no let/const hoisting). Handbook Features **Closure** section no longer claims Adaptive has no closures.

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
- Helpers as Adaptive functions (**#55** closed): `keys`, `values`, `entries`, `at`, `push`/`pop`/`shift`/`unshift`, `splice`, `freeze`, `every`/`some`. ECMAScript-named array functions take the array first, so `a->some(p)` works ([#599](https://github.com/afw-org/afw/issues/599); before that the array was second and `roles->some(…)` failed). Keep XACML-shaped names (`all_of` / `any_of`, …) as first-class too—not a rename-everything-to-JS project.
- Out-of-range **`at`** / bracket **`a[i]`**, empty **`pop`/`shift`**: **undefined** (nullish), not throw.
- Array literal **elision** (`['a', ,'b']`) → dense **undefined** slots (script / relaxedJson; not strict JSON). Assign **`a[length] = x`** appends; no gap fill. **`create_array(n)`** pre-sizes with undefined.

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
| **Index signatures** on object types (`{ [key: string]: T }`, numeric keys, or known fields plus an open tail) | Script object types list **known properties** only. Free-form objects: use data types **`object`** or **`any`**, or leave type checking off for that unit. Schemas the framework owns: **Adaptive object types** and adapters — script `interface` is not that catalog. The Adaptive **JavaScript client** may use index signatures when typing open Adaptive objects in the host app; that is a different layer. The index form is not Adaptive Script type syntax. |
| **Literal types** (`"read"`, `1`, `true` as types; unions like `"a" \| "b"`) | Type leaves are Adaptive **data types** only. There is no TypeScript-style value-level type lattice and no control-flow narrowing for discriminants. Use `string` / `integer` / `boolean` (and ordinary unions of those); check specific values at run time when needed; framework vocabularies often live as **allowed values** on Adaptive property / object types (same spirit as generated UI props that stay `string` with a prose list). |
| **`readonly` in script types** (properties, tuples, arrays, `ReadonlyArray`-style forms) | Not part of the Adaptive Script type language. Non-writable **Adaptive object** properties use Adaptive object types / property meta (for example **allow write**), enforced on **write paths at run time** and reflected in the admin UI — not a script type modifier. In script, use **`const`** and runtime helpers such as **freeze** for bindings and values. |
| **Richer tuple types** (optional elements such as `[T, U?]`, rest such as `[T, ...U[]]`, labeled `[x: T, y: U]`) | Adaptive Script has **fixed-length** tuples only: `[T, U, …]` with length and per-position checks. Open-ended sequences use **`T[]`**. Optional structure often fits an object shape better than a variable-length tuple. Rest on **function** parameter types is separate and already exists. |
| **Method / call / construct signatures in object types** (`{ run(a: integer): boolean }`, `{ (): void }`, `{ new (): T }`) | Object types list **properties** only (`name` / `name?` + `:` + Type). For a callable property write a **function Type**: `run: (a: integer) => boolean` — same idea as built-in function-parameter prototypes in the Function Reference. Adaptive has no class / `new` object model; methods are Adaptive functions (and optional `->` sugar), not prototype members. One form, not TypeScript method shorthand. |
| **`satisfies`, type predicates, assertion functions** (`satisfies`, `x is T`, `asserts x is T`, non-null `!`) | Not part of Adaptive Script. There is no TypeScript-style control-flow narrowing lattice. Declare types with `: Type` and use type checking when it is on and types are known. At run time use `is<T>(v)`, `v as T` (see [motivated differences](#motivated-differences-not-bugs)), conditions, and `throw`. |
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
| **Script function `: void`** | **Intentional, not ES.** Adaptive can declare `function f(): void { … }`. The call is the **void** singleton (same as `print()`), not `undefined` and not the body’s last assignment, so `f();` does not write the script result. ES has no function return type for that: `function f() {}` is `undefined`, which is a value. Untyped Adaptive `function f() { let x; }` is still `undefined` as the call result (and does write). Handbook Features **Functions and parameters**; tests `void_result.as` / `script_result.as`. |
| **`throw` / `catch`** | String message + optional data; fixed catch object shape |
| **Type checking** | **Opt-in** (not always-on `tsc`) |
| **Outside names** | **Qualifiers**, not globals |
| **Methods** | Adaptive functions + optional `->` sugar, not prototype walk. A data type function is a method of its first parameter: `x->f(a)` is `f(x, a)`, as Perl's `$x->f($a)` passes `$x` as the first argument (it works for script functions too), so ECMAScript-named built-ins take ECMAScript's receiver first (`map(a, f)`, `slice(s, 1)`). It is `->`, not `.`, because `.` is property access (`o.f(x)` calls property `f`): if `.` also found built-ins, data could shadow a built-in and a misspelled function could not be a compile-time error |
| **Script types vs Adaptive object types** | Script `interface` ≠ adapter object type catalog |
| **`type` / `interface` names are not hoisted** ([#188](https://github.com/afw-org/afw/issues/188)) | A name must already be declared in this compile unit (or be an Adaptive data type), like `let` / `const`. TypeScript hoists type aliases and interfaces so a later `type B` can satisfy an earlier use; Adaptive does **not**. **Self-ref in the same statement is allowed** (`type Node = { next?: Node }`). Unknown names are a compile error even with type checking off. Mutual / forward types (`type A = { b: B }; type B = { a?: A }`) are not supported — reopen only if we want an explicit mutual form. Bare alias cycles (`type A = A`, or a cycle only through union / intersection) are errors. |
| **Array element types** | **`T[]` only** — not `Array<T>`. TS treats both as the same; Adaptive keeps a single spelling (matches typical app/client `string[]` style and avoids a second path into generics) |
| **`null` vs `undefined`** | Both nullish; Adaptive `null` is a typed singleton; C APIs may use NULL for undefined—script authors should think in nullish + exists/get rules above |
| **String encoding** | Adaptive is **100% UTF-8** end-to-end. **`afw_utf8_t` values are always valid NFC UTF-8** (invalid UTF-8 must not appear as a string value—that’s a serious bug). ECMAScript engines typically expose a **UTF-16 code unit** model (one logical character may be two 16-bit units / a surrogate pair). Adaptive **length**, **substring**, **index_of**, **`s[i]`**, **for-of** (strings), spread, and empty-separator **split** walk **Unicode code points** in that UTF-8, not UTF-16 code units and not “raw octet indexes” for character steps. A `\u` surrogate pair in source decodes to one code point: `"\uD83D\uDE00"` has length **1** in Adaptive and 2 in ECMAScript. |
| **`\u` / `\u{…}` / `\xHH` / `\0` / identity / line-continuation escapes** | Script string escapes support **`\u`**, **`\u{…}`**, **`\xHH`**, **`\0`** (null, not followed by a digit), classic singles (`\n`, …), **identity NonEscapeSequence** (`\A` → `A`), and **LineContinuation** (`\` + LF/CR/LS/PS contributes no characters). Digits **`1–9`** after `\` are still invalid (no legacy octal). The **runtime value** is still UTF-8, not a UTF-16 buffer. |
| **Typed literals** ([#581](https://github.com/afw-org/afw/issues/581)) | **Adaptive-only syntax.** A string literal followed immediately by a data type id or short form (`"2026-10-11T12:00:00Z"dateTime`, `'…'dt`, `"https://example.com/"uri`) is a value of that data type, read at compile time; bad text is a compile error. TS/JS has no equivalent (the closest is a tagged template, `` tag`…` ``, which runs a function at run time). Adaptive keeps data types such as dateTime, anyURI, and ipAddress distinct from string, so it needs a way to write their values. TS tooling will not parse these. |
| **`v as T` checks at run time** ([#582](https://github.com/afw-org/afw/issues/582)) | Same spelling and precedence as a TypeScript type assertion (`a + b as integer` asserts the sum; `x as T == y` is `(x as T) == y`), but TypeScript's only tells the compiler, and Adaptive's **checks**: it returns `v` if `v` is `T` and throws `argument_error` if not. It **never converts**: `"2026-10-11" as date` is an error that says to write `"2026-10-11"date` or `to_date(v)`, and `5 as double` says `to_double(v)`. **`null` is its own data type**, so `null as string` is an error, as under `strictNullChecks`; `undefined as T` is an error too. Under `#compile typeCheck`, a value whose type is already known not to be `T` is a compile error with the same hint. For now `T` is a data type (or an alias of one); `v as string[]`, unions, and interfaces are a compile error, and so is `as any`. TypeScript reads `v as T + 1` as `(v as T) + 1`; Adaptive asks for the parentheses, and `(v as object).name` uses the asserted value. Each `v as T` is the function `as_T(v)`; `decompile` writes `(v as T)`. Test without throwing with `is<T>(v)` (or `is_T(v)`), which is true exactly when `v as T` does not throw (false for undefined). |
| **`to_T(s)` reads text strictly** ([#588](https://github.com/afw-org/afw/issues/588)) | ECMAScript's `Number(" 12 ")` is `12`, `Number("")` is `0`, and `Boolean("false")` is `true`. Adaptive's `to_T(s)` reads text with data type `T`'s parser and throws on anything else: `to_integer(" 12")`, `to_integer("1.5")`, and `to_double("5")` (integer text is not a double) are errors. `boolean(v)` is `Boolean()` truthiness, as in ECMAScript; reading boolean text is **`string_to_boolean(s)`** (`"false"` is `false`, `"maybe"` is an error). |
| **No `Number(x)`-style data type functions** ([#589](https://github.com/afw-org/afw/issues/589)) | ECMAScript's `Number(x)` and `String(x)` convert whatever they get. Adaptive has no `integer(x)`, `double(x)`, `dateTime(x)`, or other function named after a data type that converts: say what you mean with a typed literal (`"2026-10-11"date`), `to_T(v)` (`to_integer(s)`, `to_double(i)`), or `v as T`. Calling one is a compile error that names the replacement. `boolean(v)` (`Boolean()` truthiness), `string(…)`, `object(…)`, `array(…)`, and the source holders (`json`, `script`, `template`, …) stay. |
| **A lone `${…}` template keeps its type** | A template literal (and a `template` value when evaluated) that is only one substitution, `` `${v}` ``, is the value `v` itself, whatever its data type (`` `${[1,2]}` `` is an array). Text around it, or more than one substitution, concatenates into a string, as in JS. A JS template literal is always a string; use `string(v)` or `to_string(v)` when you need one. |
| **Typed declarations, parameters and returns are checked at run time** ([#583](https://github.com/afw-org/afw/issues/583)) | TypeScript erases types, so `function n(x: number)` called from untyped code with `"12"` gets the string. Adaptive checks a declared data type, `T[]` of a data type, or a union of these at run time **even with type checking off**, and **never converts**: `n(1.9)` for `x: integer` and `const d: dateTime = "abc"` are errors that say what to write (`to_integer(v)`, `"…"dateTime`). Built-in parameters work the same way. Object shapes, tuples and function types are checked at run time only under `typeCheck` ([#597](https://github.com/afw-org/afw/issues/597)). |
| **`+` on strings takes strings only** ([#583](https://github.com/afw-org/afw/issues/583)) | ECMAScript's `"a" + 1` is `"a1"`. In Adaptive it is an error, at compile time when the data types are known and at run time otherwise, the same rule as `+` for integers and doubles. Write `concat("a", 1)`, `string("a", 1)`, `"a" + to_string(1)`, or a template `` `a${1}` ``. |
| **Arrays know their data type** ([#583](https://github.com/afw-org/afw/issues/583)) | A plain array (`[1, 2]`) may hold anything and reports the data type of what it holds, so `integer[]` is checked without walking it. **`array<T>(…)`** makes a typed array that rejects a value of another data type when it is added. ECMAScript has no typed array of general values (only `Int32Array` and the like). |
| **A string is an array only where ECMAScript iterates one** ([#583](https://github.com/afw-org/afw/issues/583)) | As in ECMAScript, a string is a sequence of code points for `s[i]`, `length`, `for…of` and spread (`[...s]`, `f(...s)`). `join` and the XACML-order walkers `any_of` / `all_of` also accept a string. `map`, `filter`, `find`, `some`, `every` and `reduce` take an array only, as in ECMAScript, where `"abc".map` does not exist ([#599](https://github.com/afw-org/afw/issues/599)): write `[...s]->map(f)`. Names that are ECMAScript String methods keep the string meaning: `slice("abc", 1)` is `"bc"` and `at("abc", 1)` is `"b"`, counting code points. Anywhere else a string where an array is required is an error: the XACML-derived set functions (`is_in`, `intersection`, …), `map` and the other ECMAScript-named walkers, `reverse`, `sort`, and a typed script parameter `p: string[]` (TypeScript rejects that too). Write `[...s]`. Function metadata marks the parameters that accept one: `iterableAsArray`. |
| **Callback arguments: built-ins get the value alone** ([#599](https://github.com/afw-org/afw/issues/599)) | A script function passed to `map`, `filter`, `find`, `find_index`, `some`, `every` gets `(value, index, array)` (`reduce`: `(accumulator, value, index, array)`), as in ECMAScript. A **built-in** gets the value alone (`reduce`: accumulator and value), because its other parameters mean something else: ECMAScript's `["1", "2"].map(parseInt)` is `[1, NaN]`; Adaptive's `map(a, add<integer>)` is an error ("expects 2 required parameters") and `map(a, to_string)` works. |
| **`sort` with no comparator orders values, not strings** ([#599](https://github.com/afw-org/afw/issues/599)) | ECMAScript's `[10, 9, 1].sort()` converts to strings and gives `[1, 10, 9]`. Adaptive's `sort([10, 9, 1])` orders as `<` does: `[1, 9, 10]`; a mix of data types is an error, as for `<`. A comparator may return a boolean (true when the first comes first) or an integer (`x - y`); a double is an error (ECMAScript reads `0.5` as "after"). `undefined` entries go last and are not compared, as in ECMAScript. |
| **`concat` of an array takes only arrays** ([#599](https://github.com/afw-org/afw/issues/599)) | ECMAScript's `a.concat(x)` flattens `x` if it is an array and appends it otherwise, so the data decides. Adaptive's `concat(a, x)` with an array first requires every value to be an array; write `[...a, v]` or `push(a, v)` to add one value. `concat` with anything else first converts every value to a string (`concat("a", 1)` is `"a1"`), as ECMAScript's string `concat` does. |
| **`index_of` finds `NaN`** ([#599](https://github.com/afw-org/afw/issues/599)) | ECMAScript's `[NaN].includes(NaN)` is `true` but `[NaN].indexOf(NaN)` is `-1` (`indexOf` uses `===`). Adaptive's `includes(a, v)` is always `index_of(a, v) >= 0`: both find `NaN`, and both compare by value (`index_of([[1]], [1])` is `0`). `<`, `==`, `===` and their friends are false for `NaN` as in ECMAScript; `sort` puts `NaN` after every other double; `max<double>` / `min<double>` with a `NaN` are `NaN`, as `Math.max` / `Math.min`. |
| **`#compile strictDouble`** ([#628](https://github.com/afw-org/afw/issues/628)) | Opt-in, like a C compiler option; no ECMAScript equivalent. Doubles follow IEEE 754's default handling, as ECMAScript does, unless a script says `#compile strictDouble;`: then IEEE 754's invalid operation (`0 / 0`, `Infinity - Infinity`, `Math.sqrt(-1)`) and division by zero (`1 / 0`, `Math.log(0)`) exceptions are errors, and `<`, `<=`, `>`, `>=` are IEEE 754's signaling comparisons (an error for `NaN`). Everything else IEEE 754 defines is unchanged. |
| **Lengths and case mapping on code points; strict pads and entries** ([#622](https://github.com/afw-org/afw/issues/622)) | `pad_start` / `pad_end` count code points, and an empty fill string when padding is needed is an error (ECMAScript returns a shorter string). `from_entries` requires `[string, value]` pairs (ECMAScript pads or ignores other lengths and converts the key); `group_by` keys are strings, not converted. `hypot()` needs a value (ECMAScript's is `0`). `normalize("NFD")` is an error, since an Adaptive string is always NFC. Math functions take doubles only: `sqrt(4)` says to write `to_double(v)`. |
| **`with_at` is ECMAScript's `with`** ([#599](https://github.com/afw-org/afw/issues/599)) | `with` is a reserved word, so `a.with(i, v)` is `with_at(a, i, v)`. `toSorted`, `toReversed`, `toSpliced` are `to_sorted`, `to_reversed`, `to_spliced`; `findIndex` is `find_index`; `Object.fromEntries`, `Object.groupBy` are `from_entries`, `group_by`; `Math.sqrt` is `sqrt` and `Math.PI` is `#pi` ([#622](https://github.com/afw-org/afw/issues/622)). |
| **Regexp matches are objects** ([#617](https://github.com/afw-org/afw/issues/617)) | ECMAScript's `s.match(re)` is an array with extra properties (`index`, `input`, `groups`). Adaptive arrays have no properties, so `match` and `match_all` return an `_AdaptiveRegexpMatch_` object with the same meanings: `m.captures[n]` is ECMAScript's `m[n]`, `m.index` (code points, not UTF-16 units) and `m.groups` (the named groups) are the same. There is no `input`, and a named group that did not take part is absent from `groups` (ECMAScript: present and undefined); reading it gives undefined either way. |
| **`split` / `replace`: a string is literal text, a regexp is a pattern** ([#617](https://github.com/afw-org/afw/issues/617)) | As in ECMAScript, a string search is literal text; but ECMAScript still reads `$&`, `$1`, `$$` in the replacement when the search is a string. In Adaptive the replacement of a string search is literal too, and only a regexp search reads `$n`, `${name}`, and `\$` (AFW's spellings of `$n`, `$<name>`, `$$`). A regexp has no `g` flag: `replace` takes a limit (-1 for every match), and `match_all` finds every match. |
| **A replacement function gets the match object** ([#625](https://github.com/afw-org/afw/issues/625)) | ECMAScript calls `s.replace(re, fn)`'s `fn` with `(match, p1, …, pN, offset, string, groups)`, so its parameters depend on the number of groups. Adaptive calls it with one `_AdaptiveRegexpMatch_` object (`m.match`, `m.captures[n]`, `m.index`, `m.groups`), and it must return a string (ECMAScript converts whatever it returns). |
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
| Options object parameter | Pattern formal + default `= {}` |
| Optional property type | `port?: integer` |
| `obj.method(a)` | `method(obj, a)` or `obj->method(a)` when available |
| `Object.keys` / `entries` | `keys(o)` / `entries(o)` |
| Array stack ops | `push` / `pop` / `shift` / `unshift` / `splice` / `at` |
| `arr.map(f)`, `arr.filter(p)`, `arr.every(p)`, … | `a->map(f)` or `map(a, f)` (array first; also `all_of` / `any_of` family, function first) |
| `arr.sort()` / `arr.toSorted(cmp)` | `sort(a)` (values, not strings) / `to_sorted(a, cmp)` |
| Rest / spread | `...rest` formal; `f(...arr)` at call; `[...arr]` |
| `Array.from(s)` / `[...s]` (the characters of a string) | `[...s]` (code points) |
| `"a" + 1` | `concat("a", 1)`, `string("a", 1)`, or `` `a${1}` `` |
| An array that only holds integers | `array<integer>(1, 2)` |
| `x ?? y` / `o?.p` | Same operators |
| Catch fields | `catch ({ message, data })` |
| Process / env | `process::…`, `environment::…` |
| `new Date("2026-10-11T12:00:00Z")` / a URL string | Typed literal: `"2026-10-11T12:00:00Z"dateTime` (or `dt`), `"https://example.com/"uri` |
| `x as Date` / `typeof x === "number"` | `x as dateTime` (checked at run time, never converts) / `is<integer>(x)`, `is<double>(x)` |
| Typecheck like `tsc` | Opt-in flags / `#compile typeCheck` |
| `type` / `interface` used above its declaration | Declare the name **first**. Same-statement self-ref (`type Node = { next?: Node }`) is the exception |
| Modules / classes | Framework + objects/functions—not `import` / `class` |

---

## Reserved words (subset)

Many JS/TS keywords are reserved even when unimplemented (`class`, `async`, `await`, `import`, `export`, `typeof`, `var`, `with`, `this`, …) so copy-paste fails early. See Language Reference **Lexical**. **Reserved ≠ implemented.** `as` is implemented (`v as T`).

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
| **#35** (open; store-time bind landed); **#28**, **#14**, **#18**, **#38**, **#55**, **#131**, **#140**, **#9**, **#110** (closed) | Closures (#2 / #35) vs landed types, helpers, exists, Patterns, snapshots, get defaults |

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
