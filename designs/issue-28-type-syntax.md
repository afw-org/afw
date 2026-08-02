# Issue #28 — TS-like type syntax and opt-in checking

**Branch:** `issue-#28`  
**Status:** Implemented for merge — syntax, IR, decompile, opt-in checking, handbook, tests.

## Decisions

- **Hard cut** of old Adaptive Type spelling (`(array of T)`, `(object "OT")`, `meta {…}`).
- Reshape **`afw_value_type_t`** (drop param union + `value_meta_object`); TS-like graph in `afw_value_internal.h`.
- Leaves = permanent **`afw_data_type_*`** pointers (`any` / `void` / … by address).
- Missing annotation → **`any`** (error when `noImplicitAny` and checking active).
- Script-local **`type` / `interface` (+ multi `extends`)**; **not** adaptive object types / OT catalogs.
- Arrays: **`T[]`** and **`Array<T>`** (same); tuples **`[T,U]`**; unions **`|`**; intersections **`&`**.
- Function types: **`(a: T) => R`** (parse/store; assignability is “is a function” only for now).
- **Checking default off**; flags + `#typecheck` pragma to enforce.
- Structural typing for objects: required props + property types + `extends`; **extra properties allowed**.

## Key files

| Area | Path |
|------|------|
| Type IR | `src/afw/value/afw_value_internal.h` (`afw_value_type_t`) |
| Parse Type | `src/afw/compile/afw_compile_parse_expression.c` |
| type/interface statements | `src/afw/compile/afw_compile_parse_script.c` |
| `#typecheck` pragma | `src/afw/compile/afw_compile_parse_pragma.c` |
| Assignability | `src/afw/value/afw_value_type_check.c` |
| Decompile | `src/afw/value/afw_value_decompile.c` |
| Flags | `src/afw/flag/afw_flag.c`, `generate/strings/strings.txt` |
| Handbook | `src/afw/doc/reference/language/types.xml` |
| Tests | `src/afw/tests/compiler/type_syntax.as`, `type_check.as` |
| User note | `whats-new.md` (Adaptive Script types) |

## Type-check flags (default off)

| Flag | Effect |
|------|--------|
| *(neither mode)* | **off** — parse/store types only |
| `compile:typeCheckCompileOnly` | compile-time only (**wins** if both mode flags set) |
| `compile:typeCheck` | compile + runtime |
| `compile:noImplicitAny` | require annotations when checking is active |
| `compile:strictNullChecks` | stricter null/undefined assignability |
| `compile:strict` | includes typeCheck + noImplicitAny + strictNullChecks |

Helpers: `afw_value_type_check_*` / `afw_value_type_is_assignable` in `afw_value.h`.

**Where checks run**

- **Runtime** (mode `on`): assignment and script function parameters.
- **Compile** (mode `on` or `compileOnly`): const/let/assign when RHS has a known data type (including inspectable object/array literals).

**What is checked**

- Leaf data types; unions / intersections.
- Object / interface shapes: required properties, property value types, `extends` bases (when value is inspectable).
- Array element types; tuple length + per-position types (when value is inspectable).
- **Function types:** script functions/closures — param types (contravariant) and return type (covariant); built-ins without a signature only need data type `function`.
- **Patterns:** list/object destructure element annotations and symbol types on Pattern leaves (runtime + compile when RHS inspectable).
- Error text: composites report missing property, element index, tuple length, or decompiled expected type (not only `composite`).

**Pragma:** `#typecheck` mode (`off` / `on` / `compileOnly`) plus options `noImplicitAny`, `strictNullChecks`, `strict` (statement position; commas optional). `#typecheck off;` clears mode and related policy flags for that unit.

## Out of scope / residual (not this issue’s merge bar)

- Advanced TS surface (generics, `keyof`, conditionals) unless forced later.
- Compile-time **optimize** using known types (mentioned on the GitHub issue as a possible side-effect, not a requirement).
- Adaptive OT ↔ script type import (intentionally separate).

**Excess properties (compile-only):** Object literals assigned to an object/interface type may not include keys outside that shape (and `extends`). Nested object literals are checked the same way. Spreads / computed keys skip the check. **Runtime** assignability stays open (adaptive objects may have extra props). Untyped RHS (e.g. bare variable) is not excess-checked at compile.

## Verify

```bash
./afwdev build --cdev
afwdev test -j --srcdir-pattern afw --test-pattern 'type_'
# optional: ./afwdev build --cdev --scan ; afwdev build --docs
```
