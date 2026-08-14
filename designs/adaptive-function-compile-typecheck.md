# Adaptive function compile-time type checking

**Status:** **Shipped** with **#28** (core PRs **#144**/**#145**; FunctionSignature HOF projection + call-site formals in wrap-up PR **#171**).  
**Depends on:** Issue **#28** script type system (syntax, `afw_value_type_t`, opt-in flags, assignability, object-literal excess) — **closed**.  
**Related later (separate):** Compile-time optimize (`designs/compile-optimize-notes.md`) — amplifies how often types are known; **not** part of #28. App-shared Adaptive functions / script poly → **#170**.  
**Goal fit:** Complete Adaptive Script’s **opt-in type system** so known calls into **adaptive (built-in / registered) functions** are checked at compile the same *way* as script function calls, without expanding function metadata into full script structural types.

---

## 1. Problem

#28 checks script-local annotations and script function formals/returns. Adaptive functions already have rich **dataType** metadata and solid **runtime** behavior (arity, convert, polymorphic dispatch, impl checks). What is missing:

- **Compile-time** checking of arguments (and useful return typing) when the callee is a known adaptive function and argument result types are known.
- A **static, generate-time** projection of formals/returns into something `#28` can consume — not a second type language in JSON, and not OT structural import.

---

## 2. Goals and non-goals

### Goals

1. When `compile:typeCheck` or `compile:typeCheckCompileOnly` is active, **known** adaptive function calls are checked at **compile** using projected formals/returns.
2. Projection comes primarily from **afwdev generate** (static `const` data); extensions use the same model as core.
3. Polymorphic hubs specialize when parameter 1’s **result data type is known** without requiring evaluation (via `afw_value_get_data_type` / `afw_value_get_info`).
4. Runtime adaptive execute paths stay as they are — **no** second `#28` runtime assign layer on adaptive calls.
5. Naming and headers follow project convention: `afw_<area>_…`, public `afw_*.h`, internal `afw_*_internal.h`.

### Non-goals (v1)

- Expanding `_AdaptiveFunctionGenerate_` / `_AdaptivePolymorphicFunction_` with full script types (unions, interfaces, tuples, …).
- OT → script `interface` import or property-level checks from OT ids in `dataTypeParameter`.
- Parsing `FunctionSignature` strings into full script function types.
- Convert-aware compile checks (strict leaf assignability only).
- Runtime `#28` checks on adaptive function parameters.
- Full type inference for untyped arguments.
- Native Windows dual resolve paths (native Windows unsupported; static cross-refs to libafw are fine on supported platforms).
- Requiring compile-time optimize (optimize later only increases how often types are known).

---

## 3. Architecture overview

```text
  generate (function_bindings.py + data type bindings)
       │
       ▼
  Static afw_value_function_definition_t
    parameters[] → outer data_type + optional resolved parameter data_type*
    returns → same
       │
       ▼
  Call site at compile (typeCheck compile on)
       │
       ├─ resolve callee → function_definition (or specialized method)
       ├─ for each formal: build/use leaf (or array-of-leaf) expectation
       ├─ for each arg: if arg result type known → afw_value_type_check_compile_assignable
       └─ if polymorphic and param1 type unknown → skip specialized formal checks
```

Script type graph (`afw_value_type_t`) remains the check language. Adaptive formals are **projected** into simple leaves / `T[]`, not full OT shapes.

---

## 4. Decisions (answered)

### 4.1 Ownership and API surface

| Piece | Owner |
|-------|--------|
| Assignability / compile check entry points | **`afw_value`** — existing `afw_value_type_check_*` in `afw_value.h` / `afw_value_type_check.c` |
| Formal metadata fields | Existing **`afw_value_function_parameter_t`** (value layer; already public) |
| Call-site wiring | **`afw_value_call_built_in_function_*`** create (and related early-bound call paths) |
| Internal helpers / macros | **`afw_value_internal.h`** (or next to type check if already internal-only) |

**Gate macro** (name indicative; exact spelling at implement):

```c
/* In afw_value_internal.h (or public only if needed outside value/) */
#define AFW_VALUE_TYPE_CHECK_ADAPTIVE_FUNCTION_FORMALS(xctx) \
    (afw_value_type_check_compile_enabled(xctx))
```

Later optional flag (not v1): e.g. `compile:noAdaptiveFunctionTypeCheck` inverted into the macro. Default: **on whenever compile type checking is on**.

### 4.2 Struct layout (generate contract)

#### On `afw_value_function_parameter_t`

Add (name at implement; intent fixed):

```c
/**
 * If non-NULL: dataTypeParameter was resolved at generate to this Adaptive
 * data type (today: ArrayOf element type). NULL means ignore the parameter
 * string for compile type projection.
 */
const afw_data_type_t *data_type_parameter_data_type;
```

- Keep existing `data_type`, string `dataTypeParameter`, optional flags.
- **Returns** use the same struct → same field for return ArrayOf element types when present.
- Zero / NULL safe for older or incomplete definitions.

#### On `afw_data_type_t` (optional but recommended in same effort)

Keep `data_type_parameter_type` string for meta/docs. Add a small **generated** discriminator for checks without `strcmp`:

```c
/* Example shape — exact enum in generate */
typedef enum afw_data_type_parameter_kind_e {
    afw_data_type_parameter_kind_none = 0,
    afw_data_type_parameter_kind_array_of,
    afw_data_type_parameter_kind_object_type,
    afw_data_type_parameter_kind_function_signature,
    afw_data_type_parameter_kind_media_type,
    afw_data_type_parameter_kind_source_parameter,
    afw_data_type_parameter_kind_type
} afw_data_type_parameter_kind_t;

afw_data_type_parameter_kind_t data_type_parameter_kind; /* generated */
```

Filled from `_AdaptiveDataTypeGenerate_` `dataTypeParameterType` (`ArrayOf`, `ObjectType`, …). Few data types — cheap permanent data.

### 4.3 Projection rules

Interpretation of `dataTypeParameter` **must** use the **outer** formal’s data type mode (`data_type_parameter_kind` / `data_type_parameter_type`), not “string looks like a type name” alone.

| Outer data type mode | Generate `data_type_parameter_data_type` | Compile expectation |
|----------------------|------------------------------------------|---------------------|
| **ArrayOf** | If parameter string is a registered data type id → that `afw_data_type_*` pointer; else NULL | `array` + element leaf when pointer set; else bare `array` |
| **ObjectType** | Always NULL | Leaf **`object`** only (OT id not imported) |
| **FunctionSignature** | Always NULL (v1) | Leaf **`function`** only |
| **MediaType** | Always NULL | Outer leaf only (`string` / binary / …) |
| **SourceParameter** | Always NULL | Outer leaf only (`script`, `template`, …) |
| **Type** (`unevaluated`) | NULL (v1) | Outer / `any` as appropriate |
| none | NULL | Outer leaf only |

**Nested ArrayOf** (array of array of …): **v1 single level only** — one resolved element data type pointer. Deeper nesting remains bare `array` or future work.

**Polymorphic formals** (`polymorphicDataType` true): after specialization to data type **T**, formal is checked as **T** (and return as **T** when `returns.polymorphicDataType`). Do not rely on the static ArrayOf pointer for those slots.

### 4.4 When a call is eligible for compile checks

Check when **all** of:

1. `AFW_VALUE_TYPE_CHECK_ADAPTIVE_FUNCTION_FORMALS(xctx)` is true.
2. Callee resolves to a concrete **`afw_value_function_definition_t`** at the call site (or a specialized method after polymorphic resolve).
3. For each formal being checked, the **argument’s result data type is known** (or the arg is an inspectable literal for structural container checks), same opportunistic rule as script compile assignable.

**Callee resolution includes:**

| Form | Eligible |
|------|----------|
| Direct built-in name (`length`, `flag_set`, …) | Yes |
| Explicit specialization `add<integer>(…)` | Yes (already non-hub) |
| Data-type method / method sugar that binds a definition | Yes |
| Polymorphic hub `add(…)` with known param-1 result type | Yes → specialize then check |
| Polymorphic hub with **unknown** param-1 type | **Arity only** (if shared); **no** specialized formal checks |
| Value held in a variable typed only as `function` | **No** specialized formals (v1) |
| Script function | Existing #28 script path (unchanged) |

### 4.5 Polymorphic algorithm

```text
fn = resolved function_definition for call
if fn is polymorphic hub:
  if source has name<DataType>:
    specialized = get_qualified_function(DataType, name)
  else:
    t1 = afw_value_get_data_type(arg1)   // and/or get_info.evaluated_data_type
         // NOT requiring full evaluate of arg1
    if t1 is NULL:
      // cannot specialize — runtime adaptive path owns formal checking
      optional: shared arity only
      return
    specialized = registry_get_data_type_method(t1, fn->dataTypeMethodNumber)
    if specialized is NULL:
      compile error: data type not supported for this function
  fn = specialized

for each formal i of fn (respecting optional / minArgs / max):
  expect = project(formal)  // leaf or array-of-leaf; polymorphicDataType → t1
  if arg i result type known (or literal inspectable):
    afw_value_type_check_compile_assignable(expect, arg_i, "parameter", xctx)
```

**Supported data type:** prefer **`get_data_type_method` non-NULL** (matches registration). `polymorphicDataTypes` list remains documentation / generate input.

**Union-typed param 1 (script type):** v1 **skip** specialization (treat as unknown) unless a single leaf is known. Avoid “check all arms” complexity in v1.

### 4.6 What is checked per formal

| Formal projection | Check |
|-------------------|--------|
| Leaf data type | Strict assignability of known arg type / value |
| Array + element pointer | Arg is array; element types when known/inspectable |
| Bare array (no element pointer) | Arg is array only |
| object / function | Arg is that leaf only |
| optional / canBeUndefined | Align with existing script nullish rules under **strictNullChecks** when that flag is on; otherwise loose like script without strictNull |
| minArgs / max / required count | **Yes** at compile when callee known (use definition’s required/max integers) |

**Object-literal excess:** only when the *expected* type has a **property shape**. Projected adaptive formals are bare `object` → **no excess** from OT ids. (Script interfaces still use excess as in #28.)

### 4.7 Returns

**Yes for v1** (high value for a complete script type story):

1. Generate/project return the same way as formals (leaf + optional ArrayOf element pointer; polymorphic return → specialized leaf after resolve).
2. When a call node can set **`evaluated_data_type`** / info from projected return, do so at create (extends existing fixed-return behavior).
3. Compile assign of call result into a typed binding uses normal `check_compile_assignable` when the call’s result type is known.

Expression-bodied pure folds remain optimize’s job later; even without fold, known return types help `const x: integer = length(s)`.

### 4.8 Strict vs convert

Under compile type checking: **strict** leaf assignability via existing `#28` helpers.

Runtime without typeCheck: adaptive functions may still convert — unchanged.

Document: enabling typeCheck makes **known** adaptive call arguments strict at compile; it does not change execute convert policy when checking is off.

### 4.9 Errors

- Use **syntax** errors for compile failures (same as `afw_value_type_check_compile_assignable`).
- Message style: `Type error in parameter` / `return` / function id when useful.
- Prefer mentioning **functionId** and, after specialization, enough context to see which method (e.g. data type or specialized id) without dumping internal pointers.

### 4.10 Generate / package coverage

- **All** packages that emit function bindings through afwdev (core + in-tree extensions): same fields, NULL when unused.
- Core and extensions may both reference permanent `afw_data_type_*` pointers (supported platforms).
- Hand-written or incomplete definitions: NULL pointers → outer leaf only / skip element checks.

### 4.11 Interaction with flags

| Flag | Effect on this feature |
|------|-------------------------|
| *(type check off)* | No adaptive formal compile checks |
| `compile:typeCheck` / `typeCheckCompileOnly` | Formal/return compile checks as above |
| `compile:noImplicitAny` | Only affects untyped **script** bindings used as args (unknown type → skip deep formal check for that arg) |
| `compile:strictNullChecks` | Applies to optional/nullish formal matching when checking |
| `compile:strict` | Includes typeCheck → includes this feature |
| Future opt-out flag | Wired only through `AFW_VALUE_TYPE_CHECK_ADAPTIVE_FUNCTION_FORMALS` |

No `#compile` handbook teaching for adaptive specifics beyond existing Types page + one short note (flags only; Jeremy style).

### 4.12 Sequencing

| Phase | Work |
|-------|------|
| **A** | Land / merge #28 script type system if not already |
| **B** | Generate: parameter field + data type kind enum; fill ArrayOf pointers |
| **C** | Call create / known definition: formal + arity compile checks |
| **D** | Polymorphic specialize via known param-1 type + returns projection |
| **E** | Tests + handbook sentence + whats-new |

Optimize issue: **after or parallel**, not a blocker. Document dependency: optimize increases hit rate of “arg type known.”

---

## 5. Worked examples

```adaptive
// typeCheck compile on

length("hi");              // OK — string formal
length(1);                 // error — integer vs string

flag_set(["compile:typeCheck"], true);  // array of string when projected
// wrong element type when known → error

add(1, 2);                 // param1 integer → integer method; OK
add(1, "x");               // error after specialize to integer
add(untyped, 2);           // no specialize — no formal error at compile
add<integer>(1, 2);        // explicit specialize — check integer formals

const n: integer = length("ab");  // OK if return projected integer
const s: string = length("ab");   // error

retrieve-ish(objOptions);  // object formal + OT id → only need object
```

---

## 6. Testing plan

1. Non-polymorphic wrong leaf arg → compile error.  
2. ArrayOf projected (`array`+`string`) wrong element when known.  
3. Object + OT formal: object OK; no OT property excess.  
4. Polymorphic: known param1 → wrong later arg fails; unknown param1 → no formal compile fail.  
5. Explicit `name<type>(…)`.  
6. Return type vs typed `const` / `let`.  
7. Arity too few / too many when max known.  
8. typeCheck off → no compile fail on (1).  
9. Extension function with generated field NULL-safe.  
10. Method-style call when definition is known.

---

## 7. Risks and footguns

- **Surprise strictness:** scripts that relied on convert will fail compile under typeCheck; document and accept (same as script formals).  
- **False confidence:** unknown param1 skips formal checks — do not claim “all adaptive calls are fully checked.”  
- **Generate churn:** large bindings files; one-time regenerate.  
- **Dual sources of truth:** string `dataTypeParameter` vs pointer — generate must set both; checks trust pointer + outer type.  
- **Call early-bind (script):** only **`const` / `function`** bindings with a known script-function `initial_value` early-bind the definition for optimize/formals; **`let` does not** (rebind safety — see `issue-28-type-syntax.md` Call-site formals). Annotation-based function Types still typeCheck call args when the type is known. Adaptive built-ins use definition lookup when the callee is a known Adaptive function value, not only early-bind of a script symbol.

---

## 8. Open only if review disagrees

These are **bets** for your review; change before implement if wrong:

1. Returns checked/projected in v1 (**yes**).  
2. Arity at compile (**yes**).  
3. Method sugar / qualified methods included when definition known (**yes**).  
4. Data type **kind enum** in same effort as parameter pointer (**yes**, small).  
5. Nested ArrayOf (**no** in v1).  
6. Follow-on issue separate from #28 merge (**yes**).

---

## 9. Success criteria (complete Adaptive Script type story)

With #28 + this design implemented:

| Surface | Compile (typeCheck) | Runtime |
|---------|---------------------|---------|
| Script annotations / interfaces / patterns | #28 | #28 when full typeCheck |
| Script function formals/returns | #28 | #28 |
| Adaptive function calls (known callee, known arg types) | **This design** | Existing adaptive function impl |
| Adaptive polymorphic (known param1 type) | Specialize + check | Existing specialize + impl |
| Adaptive polymorphic (unknown param1) | Skip formals | Existing specialize + impl |
| OT / adapter meta | Unchanged | Unchanged |

That is a coherent, opt-in type system for Adaptive Script without marrying script types to adaptive object types.

---

## 10. Implementation sketch (files)

| Area | Likely touch |
|------|----------------|
| Generate | `src/afw_dev/_afwdev/generate/function_bindings.py`, data type bindings / maps |
| Structs | `afw_value.h` (`afw_value_function_parameter_t`), data type struct in generated interface / bindings |
| Check | `afw_value_type_check.c`, `afw_value_internal.h` macros |
| Call | `afw_value_call_built_in_function.c` (create), possibly method call create paths |
| Tests | Adaptive cases in `type_check.as` (under `#compile typeCheck`); flag contract in `type_check_flags.as` |
| Docs | Types handbook note; `whats-new.md` |

Do not hand-edit `generated/`; regenerate via afwdev.

---

## 11. Review checklist for maintainer

- [x] Compile-only adaptive checks + no #28 runtime on adaptive execute  
- [x] ArrayOf → static `const afw_data_type_t *`; other parameter modes NULL  
- [x] OT / FunctionSignature not structural at compile  
- [x] Polymorphic: known param1 type via value interface; else defer  
- [x] Strict assignability under typeCheck compile  
- [x] Returns + arity in v1  
- [x] Naming `afw_value_*` / generate statics  
- [x] Formal/return walks gated on `allow_optimize` (runtime map/reduce create safe)  
- [ ] Compile-time **optimize** — **separate issue**, not #28  

---

*Shipped with **#28** (closed); historical branch name was `issue-#28`.*
