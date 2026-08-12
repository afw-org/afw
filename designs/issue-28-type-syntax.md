# Issue #28 — Type syntax and opt-in checking

**Branch (wrap-up):** `issue-#28-wrap-up` (off `mgg-develop`)  
**Earlier line:** `issue-#28` + `issue-#28-pragma-cleanup` (PRs **#144**, **#145**)  
**GitHub #28:** **Still open** — core shipped; this branch aims to **totally close** the issue (gaps + decided-not + packaging). Quality bar: release-grade for the **claimed** Adaptive type surface; beta/RC later = external validation, not a second type epic.

## Flexible plan (living candidates — not a locked script)

Cadence: *Flexible plan, one step, then re-decide* (mantras pad). After each step: **“what do you think we should do next?”**

| # | Candidate step | Status |
|---|----------------|--------|
| 1 | **Inventory** — claimed surface / fix / decided-not / probe | Done (draft in pad) |
| 1b | **Gray-zone type syntax** — one by one: support as claimed surface vs **no** (how Adaptive works); TypeScript-author expectation given what Adaptive is/isn’t; record in this pad + `typescript-differences.md` (plain language; definitive product wording, not “not yet”) | **In progress** |
| 1c | **FunctionSignature formals** — (A) documented functor prototypes must be valid Adaptive Script Types (validate/generate guard); (B) under typeCheck, project FunctionSignature → function Type and compile-check known function arguments (script functions first). Align script formals with built-in HOF docs. Not TypeScript generics. | **Queued after 1b** (user wants B) |
| 2 | **Highest-value gap vertical(s)** — remaining G1/G2/… after 1c as needed | Pending |
| 3 | **Decided-not + pad/issue hygiene** — remaining nos; fix stale issue body (`Array<T>`, etc.) | Pending (partial with 1b) |
| 4 | **Close package** — issue body, labels, `open-issues-status`, ready to close #28 | Pending |
| 5 | **Verify** — `type_*` suite; pre-PR fulldev + valgrind as usual | Pending |

Order may change after any step.

### FunctionSignature / polymorphic note (for step 1c)

- Built-in HOF formals: `dataType: function` + `dataTypeParameter` FunctionSignature string (e.g. `(...values: any) => boolean`). Current set **parses** as Types; guard so docs cannot drift.
- Compile typeCheck v1 projects FunctionSignature as leaf **`function` only** — B upgrades that to full function Type checks when the arg is known.
- **Adaptive polymorphic** is **not** TypeScript generics. Call/name form: `FunctionName ::= Identifier ( '<' DataType '>' )?` (e.g. `add<integer>(…)`). Docs/declarations use placeholder **`function id <dataType>(…)`** (no markdown backticks in Adaptive-shaped prototypes) and Supported `<dataType>` lists; specialized declarations look like `function add<integer>(…)`. Parameter Type spelling may use placeholder name **`dataType`** when `polymorphicDataType` is true. Script **type** grammar does **not** include user `f<T>`; angle brackets on **built-in function names** are data-type specialization, still live in the parser.
- **Application-shared Adaptive functions + script poly** — **future** GitHub **[#170](https://github.com/afw-org/afw/issues/170)**: conf/application way to supply functions all scripts can use; polymorphic script functions are useful **as part of that**, not alone. Not #28 merge bar. When touching typeCheck specialize, Adaptive formal projection, call create, or function registration, prefer designs that do not hard-code “C definitions only.”

### Gray-zone decisions (step 1b)

**Lens (all three):**

1. Would a TypeScript-literate author **reasonably expect** this after reading what Adaptive Script is and is not?  
2. Shared surface only when we support a construct — then match reasonable TypeScript behavior for *that* construct. Prefer motivated differences; document non-goals in **plain language** (no casual short forms like “OT” / “TS” in author-facing notes).  
3. **Jeremy’s stack:** check how TypeScript (and JavaScript) are used in the admin app and Adaptive JavaScript client (`src/afw_app`, `src/afw_client`, components). That is evidence of AFW authoring practice — not a requirement that Adaptive Script clone the client type system. Handbook “Jeremy rules” still apply to user-facing prose.

| Item | Decision | Recorded |
|------|----------|----------|
| Index signatures (`{ [key: string]: T }`, …) | **No.** Closed property lists only; free-form → `object` / `any` / checking off; framework schemas → Adaptive object types / adapters. (Client TypeScript may use index signatures for open Adaptive objects — different layer.) | This pad + `typescript-differences.md` |
| Literal types (`"read"`, `1`, `"a" \| "b"`, …) | **No.** Leaves are Adaptive **data types**, not singleton values. No TypeScript-style literal unions. App/client style: `string` + prose or `allowedValues` / object-type data. | This pad + `typescript-differences.md` |
| `readonly` (props / tuples / arrays) | **No** in **script type** syntax. Non-writable **Adaptive object** properties use **runtime / object-type meta** (e.g. allow write), not script `readonly`. | This pad + `typescript-differences.md` |
| Tuple optional / rest / labels | **No.** Fixed **`[T, U, …]`** only. Open-ended → **`T[]`**; optional structure → object shapes. | This pad + `typescript-differences.md` |
| `typeof` in type position | **No.** Keyword reserved (fail early); not the JavaScript operator; not a TypeScript type query. Write types with data type names / shapes / `type`·`interface`. | This pad + `typescript-differences.md` |
| Method / call signatures in object types | **No.** Properties only: **`name: Type`**. Callables: **`run: (a: T) => R`**. No method shorthand, bare call signatures, or `new` signatures. | This pad + `typescript-differences.md` |
| Assertions / `satisfies` / type predicates | **No.** No `as T`, `satisfies`, `x is T`, or `asserts`. Annotations + typeCheck when types known; runtime guards with conditions / helpers / `throw`. No control-flow narrowing lattice. | This pad + `typescript-differences.md` |
| `enum` | pending | — |
| Interface merging | pending | — |
| `implements` | pending (likely covered by no classes) | — |
| Import types | pending (likely covered by no modules) | — |
| `never` | pending | — |

**Wording:** Gray-zone **no** means *how Adaptive Script works* for beta/release, not “not yet.” A future change needs an explicit new issue and decision — not silent reopen.

## Decisions

- **Hard cut** of old Adaptive Type spelling (`(array of T)`, `(object "OT")`, `meta {…}`).
- Reshape **`afw_value_type_t`** (drop param union + `value_meta_object`); structured graph in `afw_value_internal.h`.
- Leaves = permanent **`afw_data_type_*`** pointers (`any` / `void` / … by address).
- Missing annotation → **`any`** (error when `noImplicitAny` and checking active).
- Script-local **`type` / `interface` (+ multi `extends`)**; **not** adaptive object types / OT catalogs.
- Arrays: **`T[]` only** (no TypeScript **`Array<T>`**); tuples **`[T,U]`**; unions **`|`**; intersections **`&`**.
- Function types: **`(a: T) => R`** — script functions/closures checked structurally (params contravariant, return covariant); other function values only need data type `function`.
- **Checking default off**; opt-in via **flags** (handbook) and optional **`#compile`** pragma (per compile unit; Pattern B — see `designs/pragma-hash-design.md`).
- Object/interface structural: required props + property types + `extends`.
- **Excess properties (compile):** object **literals** may not include keys outside the type; nested literals checked; **spreads / computed keys skip**; **runtime** assign of non-literals stays open (adaptive-friendly).

## Companion work

- Adaptive function compile formals: `designs/adaptive-function-compile-typecheck.md` (shipped on this branch).
- Compile-time **optimize** from known types: **not** this issue — separate pad/issue later (`designs/compile-optimize-notes.md`).
- Pragma / `#` control surface: `designs/pragma-hash-design.md`, `designs/compile-contextual-audit.md`, `designs/decompile-compiler-internal-inventory.md`.

## Key files

| Area | Path |
|------|------|
| Type graph | `src/afw/value/afw_value_internal.h` (`afw_value_type_t`) |
| Parse Type | `src/afw/compile/afw_compile_parse_expression.c` |
| type/interface statements | `src/afw/compile/afw_compile_parse_script.c` |
| `#compile` pragma | `src/afw/compile/afw_compile_parse_pragma.c` |
| Compiler-internal `#…` accept | `src/afw/compile/afw_compile_parse_compiler_internal.c` |
| Assignability / excess / Adaptive formals | `src/afw/value/afw_value_type_check.c` |
| Call create (Adaptive formal gate) | `src/afw/value/afw_value_call_built_in_function.c` |
| Decompile | `src/afw/value/afw_value_decompile.c` |
| Flags | `src/afw/flag/afw_flag.c`, `generate/strings/strings.txt` |
| Handbook | `src/afw/doc/reference/language/types.xml` (flags-first authoring) |
| Tests | `type_syntax.as`, `type_check_flags.as`, `type_check.as`, `type_check_multi_unit.as` |
| User note | `whats-new.md` (Adaptive Script types) |

## Type-check flags (default off)

| Flag | Effect |
|------|--------|
| *(neither mode)* | **off** — parse/store types only |
| `compile:typeCheckCompileOnly` | compile-time only (**wins** if both mode flags set) |
| `compile:typeCheck` | compile + runtime |
| `compile:noImplicitAny` | require annotations when checking is active |
| `compile:strictNullChecks` | stricter null/undefined assignability |
| `compile:strict` | typeCheck + noImplicitAny + strictNullChecks |

Helpers: `afw_value_type_check_*` / `afw_value_type_is_assignable` in `afw_value.h`.

**Where checks run**

- **Runtime** (mode `on`): assignment, script function parameters, and function return values.
- **Compile** (mode `on` or `compileOnly`): const/let/assign when RHS type is known (literals; typed symbols via type-to-type); return expressions; call sites when the callee is a known script function (named `function` form); known Adaptive function formals (create with `allow_optimize`).

### Compile-time vs runtime Adaptive value checking (overall)

Three jobs people lump together as “type checking.” Keep them separate when closing #28 or teaching.

| Job | When | What |
|-----|------|------|
| **Script type checking** (#28) | Opt-in. Compile when the type is known; full `typeCheck` also checks at runtime for assigns / script formals / returns | Script-local annotations, shapes, data-type leaves, patterns, known callees |
| **Adaptive function formals** (built-ins) | **Compile** when callee + arg types known (`allow_optimize` gate); **not** a second #28 layer on Adaptive execute | Projected metadata; runtime still does Adaptive convert / implementation checks as always |
| **Adaptive objects** (object types, property meta, allow write, …) | **Runtime** (and admin UI from meta) when a real object + type/meta are available | Dynamic catalogs, adapters, models — not full compile-time import of the object type catalog into the script checker |

**One line:** Script types are an optional static/local layer. Adaptive functions keep their runtime behavior and gain early compile checks when enough is known. Adaptive object rules (including write policy) are meta- and write-path-driven because that world is dynamic.

Author-facing thin version: root [`typescript-differences.md`](../typescript-differences.md) (Types). Companion design: [`adaptive-function-compile-typecheck.md`](adaptive-function-compile-typecheck.md).

**What is checked**

- Leaf data types; unions / intersections.
- Object / interface shapes: required properties, property value types, `extends` bases (when the value is known).
- Array element types; tuple length + per-position types (when known).
- **Function types:** script functions/closures — param types (contravariant) and return type (covariant).
- **Returns:** declared return type vs `return` expression / expression-body (compile) and result value (runtime).
- **Patterns:** array/object destructure element annotations and symbol types on Pattern leaves.
- **Call sites:** known named script functions; known Adaptive functions (projected formals / returns).
- Error text: composites report missing property, element index, tuple length, or decompiled expected type.

**Pragma:** `#compile` + flag short names (`typeCheck`, `typeCheckCompileOnly`, `noImplicitAny`, `strictNullChecks`, `strict`, `noOptimize`, **`noTypeCheck`**, …). Flags are **process defaults** snapshotted at each compile start into the unit’s policy; `#compile` mutates **only that unit** (including mid-unit “from here on”). **`noTypeCheck`** clears the type-check cluster on unit policy (does not clear `noOptimize`). Retired: bare `#compile off;`, old `#typecheck`. See `designs/pragma-hash-design.md`. Handbook teaches **flags**; pragma is optional for tests and compact scripts.

## Tests layout

| File | Role |
|------|------|
| `type_syntax.as` | Parse/store/decompile only (checking off) |
| `type_check_flags.as` | Flag + pragma contract; mid-unit; process isolation patterns |
| `type_check.as` | Rules under `#compile typeCheck` in the unit under test |
| `type_check_multi_unit.as` | Nested / sibling units; definition-unit policy vs process flags |

## Wrap-up inventory (step 1)

Shared map for closing #28. **“Not yet / residual” is not the same as “decided not to”** (mantras).

### Claimed surface (shipped — must stay true)

| Area | Evidence |
|------|----------|
| Type syntax (leaves, `T[]`, tuples, unions, intersections, object shapes, function types, `type` / `interface` + `extends`) | Parser + `type_syntax.as` |
| Hard cut old Adaptive Type spelling; no `Array<T>` | Parse errors + tests |
| Opt-in flags + `#compile` Pattern B (unit policy, mid-unit, isolation) | `type_check_flags.as`, `type_check_multi_unit.as` |
| Assignability: leaves, unions/intersections, shapes, arrays/tuples when known | `type_check.as` |
| Function types / script returns (compile + runtime under full typeCheck) | `type_check.as` |
| Pattern leaf annotations (array + object rename cases) | `type_check.as` |
| Excess on object **literals** (incl. call-site); spreads/vars skip; runtime open | `type_check.as` |
| Call sites: **named** script functions; **Adaptive** formals/returns/arity (compile, `allow_optimize`) | `type_check.as` + Adaptive pad |
| Docs: handbook Types, `whats-new`, design pads | present on `mgg-develop` |

### Fix / probe candidates (this branch — designed surface only)

| ID | Item | Notes | Priority |
|----|------|--------|----------|
| **G0** | **FunctionSignature: valid Type + compile check** | Step **1c**. (A) validate FunctionSignature strings parse as Types. (B) project to function Type; typeCheck known functor args (esp. script functions) against built-in HOF formals. See gray-zone discussion + polymorphic note. | High (after 1b) |
| **G1** | **Call-site formals from a typed function variable** | Pad residual: named / early-bound callees today. Authors write `const f: (a: integer)=>integer = …; f("x")` and may expect a check. **Probe first** (failing test): if thin (function-type on callee symbol), fix; if needs produce-type/optimize, reclassify to decided-not or tiny follow-on — do **not** pull optimize into #28. | High |
| **G2** | **Pattern annotation coverage vs claim** | Basic array/object pattern tests exist. Probe param Patterns, nested, rest, catch Patterns if type annotations are stored but unchecked. Grow only where syntax stores a type and claim says enforced. | Medium |
| **G3** | **Contract tests for every claimed rule** | Spot-check pad “What is checked” vs `type_*` names; add missing happy/error pairs only for real holes (not TS completeness). | Medium |
| **G4** | **Error message consistency** | Composites already mention property/index/expected type in places; fix only confusing/missing cases found while probing G1–G3. | Low (opportunistic) |
| **G5** | **Stale tracker / pad text** | GitHub issue body still lists `Array<T>`; open-issues “left open for residuals”; pad header until close. Part of step 3. | Hygiene |

### Decided not to (write down; do not implement under #28)

Reopen only by **explicit** new decision. Author-facing wording lives in root [`typescript-differences.md`](../typescript-differences.md) (plain language).

| Decision | Why |
|----------|-----|
| Advanced TypeScript type system (generics, `keyof`, conditionals, mapped types, …) | Different product; Adaptive data-type leaves + structural shapes |
| TypeScript **`Array<T>`** spelling | Hard cut; **`T[]` only** |
| **Index signatures** (`{ [key: string]: T }`, numeric keys, known props + open tail) | Script object types are **closed property lists**. Free-form bags: `object` / `any` or checking off. Framework-owned schemas: Adaptive object types / adapters — not a second index-signature language in script. Not a TypeScript-shaped “should fix”; unclaimed. (Admin **client** TypeScript uses index signatures for open Adaptive objects; that is the host app layer, not Adaptive Script types.) |
| **Literal types** (`"read"`, `1`, `true` as types; `"a" \| "b"` literal unions) | Type leaves are Adaptive **data types**, not individual values. No checker narrowing / discriminated-union story claimed. In-tree client/admin style prefers data kinds, prose allowed values, object-type `allowedValues`, occasional TypeScript `enum` — not literal unions. Not a “should fix.” |
| **`readonly` in script types** (properties, tuples, arrays, `ReadonlyArray`-style) | Script type language has no mutability lattice. Non-writable **Adaptive object** properties are product-real but live as object-type / property meta (e.g. **allow write**) enforced on **write paths at runtime** (and UI from meta) — same dynamism reason Adaptive object catalog checks are not full compile-time script checking. Script-side tools remain **`const`** and runtime helpers such as **freeze**. Not a “should fix” for script syntax. |
| **Richer tuple forms** (optional elements `U?`, rest `...` in tuple types, labeled `[x: T, y: U]`) | Claimed surface is **fixed-length** `[T, U, …]` only (length + per-position checks). TypeScript optional/rest/label tuple algebra is a larger feature; open-ended sequences use **`T[]`**; optional structure often fits object shapes. Function-type rest formals stay separate. Not a “should fix.” |
| **`typeof` in type position** (and JavaScript value `typeof`) | Keyword **reserved** so copy-paste fails early; **reserved ≠ implemented**. Not the JavaScript operator; not a TypeScript type query that pulls a value’s type into the type language. Prefer `: Type`, `type` / `interface` aliases. Not a “should fix.” |
| **Method / call / construct signatures in object types** | Properties only: `name` / `name?` + `:` + Type. Use **function Types** for callables (`handler: (a: integer) => boolean`), matching built-in HOF FunctionSignature style. No `{ run(a: integer): boolean }`, bare `{ (): void }`, or `{ new (): T }`. One form over dual TypeScript sugar. |
| **Type assertions / `satisfies` / type predicates / assertion functions** | Not part of Adaptive Script. No `as T`, non-null `!`, `satisfies`, `x is T`, or `asserts x is T`. No control-flow type narrowing. Use `: Type` + typeCheck when known; runtime conditions, Adaptive helpers, and `throw` for guards. |
| Adaptive **object type catalog → script type import** | Separate design family; script `interface` / shapes stay local |
| **Convert-aware** Adaptive formal checks; **runtime typeCheck layer** on Adaptive function execute | Compile-only Adaptive formal checks; runtime stays Adaptive function behavior |
| Compile-time **optimize** / produce-type percolation from known types | Separate track (`designs/compile-optimize-notes.md`); not type-check surface; amplifies specialize-when-known for poly (built-in and future script) |
| **Application-shared Adaptive functions** (conf); **script polymorphic** as part of that | Desired future; **[#170](https://github.com/afw-org/afw/issues/170)** (living summary in issue body). Not TypeScript generics; not #28 merge bar. Keep registration/typeCheck paths extensible. |
| **Excess** on non-literals / spreads / computed keys | Deliberate adaptive-friendly openness |
| Full TypeScript-checker inference / control-flow narrowing | Not claimed; would redefine the language |

### Explicitly not a #28 gap

- **#42** error-message quoting style (historical comment on #28).
- Closures lifetime / escape (**#2** / **#35**).
- Handbook Features prose lag elsewhere (fix only if it **contradicts** Types for #28).

### Inventory outcome (step 1)

- Core product bar is **already shipped**; wrap-up is **finish claimed gaps + record nos + close**.
- Highest-value code vertical likely **G1** after a live fail-test probe.
- Hope: **no follow-on issue** if G1 is thin or reclassified to decided-not with a written why.

## Verify

```bash
./afwdev build --cdev
afwdev test -j --srcdir-pattern afw --test-pattern 'type_'
# pre-PR: ./afwdev build --fulldev ; afwdev test -j --env-mode valgrind
```
