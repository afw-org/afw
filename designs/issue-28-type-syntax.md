# Issue #28 — TS-like type syntax (phase 1: parse/store)

**Branch:** `issue-#28`  
**Status:** Phase 1 in progress — syntax + IR + decompile; **no type checking yet**.

## Decisions (session)

- **Hard cut** of old Adaptive Type spelling (`(array of T)`, `(object "OT")`, `meta {…}`).
- Reshape **`afw_value_type_t`** (drop param union + `value_meta_object`); TS-like graph in `afw_value_internal.h`.
- Leaves = permanent **`afw_data_type_*`** pointers (`any` / `void` / … by address).
- Missing annotation → **`any`**.
- Script-local **`type` / `interface` (+ multi `extends`)**; **not** adaptive OTs.
- Arrays: **`T[]`** and **`Array<T>`** (same); tuples **`[T,U]`**; unions **`|`**; intersections **`&`**.
- Function types: **`(a: T) => R`**.
- Non-checking mode; flags/pragmas for enforce later.
- Parse tests + decompile round-trip.

## Key files

| Area | Path |
|------|------|
| Type IR | `src/afw/value/afw_value_internal.h` (`afw_value_type_t`) |
| Parse Type | `src/afw/compile/afw_compile_parse_expression.c` |
| type/interface statements | `src/afw/compile/afw_compile_parse_script.c` |
| Decompile | `src/afw/value/afw_value_decompile.c` |
| Tests | `src/afw/tests/compiler/type_syntax.as` |

## Type-check flags (default off)

| Flag | Effect |
|------|--------|
| *(neither)* | **off** |
| `compile:typeCheckCompileOnly` | compile-time only (wins if both) |
| `compile:typeCheck` | compile + runtime |
| `compile:noImplicitAny` | require annotations when checking |
| `compile:strictNullChecks` | stricter null/undefined |
| `compile:strict` | includes typeCheck + noImplicitAny + strictNullChecks |

Helpers: `afw_value_type_check.c` / APIs in `afw_value.h`.  
Runtime: assign + script params (leaves, union/intersection, array/tuple elements, object/interface properties + extends, function shape).  
Compile: const/let/assign when RHS type known (including inspectable object/array literals); `noImplicitAny` on missing annotations.

**Pragma:** `#typecheck` mode (`off` / `on` / `compileOnly`) plus options `noImplicitAny`, `strictNullChecks`, `strict` (statement position; sets xctx flags; commas optional).

**Handbook:** `src/afw/doc/reference/language/types.xml`.

## Later

- Richer function-type checks (param/return structural).
- More TS surface (generics, etc.) if needed.
- Object/array meta access polish under #2 (separate).
