# Issue #28 — Type syntax and opt-in checking

**Branch:** `issue-#28` (merged line) + follow-on `issue-#28-pragma-cleanup` (PR **#145**)  
**GitHub #28:** **Still open** (2026-08-04 process-close pass left it open for residuals/packaging). Core product bar is **shipped** on `mgg-develop` — syntax, type graph, decompile, opt-in checking (flags + `#compile` Pattern B), structural rules, excess on object literals, Adaptive formals (companion pad), handbook Types, tests.

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

## Out of scope / residual (not this issue’s merge bar)

- Advanced TS surface (generics, `keyof`, conditionals) unless forced later.
- Compile-time **optimize** using known types (separate issue).
- Adaptive OT ↔ script type import (intentionally separate).
- Convert-aware Adaptive formal checks; runtime #28 layer on Adaptive execute.
- Call-site formals from a **variable’s** function-type annotation only (named / early-bound callees today).

## Verify

```bash
./afwdev build --cdev
afwdev test -j --srcdir-pattern afw --test-pattern 'type_'
# pre-PR: ./afwdev build --fulldev ; afwdev test -j --env-mode valgrind
```
