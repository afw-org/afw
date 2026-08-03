# Decompile vs compiler-private accept inventory

**Status:** Active checklist (issue #28 pragma cleanup / #18 decompile fidelity).  
**Code:** accept — `src/afw/compile/afw_compile_parse_compiler_private.c`; emit — value `impl_afw_value_decompile` + `afw_value_decompile_call_args`.  
**Tests:** `src/afw/tests/compiler/decompile_accept/`; broad round-trips — `decompile_fidelity.as`, samples — `decompile.as` / `pragma.as`.

Author-facing `#compile` is **not** in this inventory (policy pragma).  
Compiler-private forms are **toolchain only** (decompile → recompile).

## Dispatch positions

| Position | Entry | Allowed names |
|----------|--------|----------------|
| Statement | `afw_compile_parse_CompilerPrivateStatement` | `block`; known rejects `closure_binding`, `function_thunk` |
| Value / expression | `afw_compile_parse_CompilerPrivateValue` | `block`, `assignment_target`, `list_expression`, `script_function`, `template_definition`, `switch_default`, `statements`; same known rejects |

Lex token: `pound_identifier` (`#Name`). Unknown names → parse error (statement vs value wording differs).

## Inventory table

| `#Name` | Emit (decompile) | Accept | Round-trip | Notes |
|---------|------------------|--------|------------|--------|
| `#block` | Yes (`afw_value_block`) | Stmt + value | Yes | Root of almost every script decompile |
| `#assignment_target` | Yes (`afw_value_assignment_target`, try/catch paths) | Value only | Yes | Kind string + Pattern (`const`/`let`/…) |
| `#script_function` | Yes (`afw_value_script_function`) | Value | Yes | Params surface-like: name/`…`/Pattern/`?`/`=`/`: Type`; body; optional return Type |
| `#template_definition` | Yes (`afw_value_template_definition`) | Value | Yes | Parts: strings + nested expressions/`#block` |
| `#list_expression` | Partial | Value | Accept yes | **Call-site spreads** decompile as surface `...expr` via `afw_value_decompile_call_args` (not `#list_expression`). Accept still parses `#list_expression(expr)` (list_expression node). |
| `#statements` | Yes (for / switch clause lists) | Value | Yes | Unevaluated statement list for control-flow builtins |
| `#switch_default` | Yes (`afw_value_decompile` permanent null marker) | Value | Yes | Optional `()` |
| `#closure_binding` | Yes (live closure) | Known **reject** | No | Runtime enclosing scope not reconstructible from text |
| `#function_thunk` | Yes (C thunk label) | Known **reject** | No | C-side only (e.g. model hooks) |
| `#compile` | Never (policy pragma) | Pragma path only | n/a | Not compiler-private |
| other `#Name` | — | Unknown error | No | |

## Emit helpers

- Synthetic name: `afw_value_decompile_write_synthetic_function_name` → `#` + `implementation_id`.
- Call spreads: `afw_value_decompile_call_args` prefers `...` over wrapping `#list_expression`.
- Patterns in `#script_function` params: surface Pattern, not nested `#assignment_target` (see `afw_value_decompile_assignment_pattern`).

## Gaps / decisions (current)

| Item | Decision |
|------|----------|
| `#list_expression` rarely appears in decompile text | Intentional surface `...`; keep accept for recompile / explicit forms |
| Statement position only `#block` (+ rejects) | Intentional; other forms are values nested under `#block` / calls |
| `#closure_binding` / `#function_thunk` | Documented non-round-trip; specific error text |
| EBNF harvest | Statement EBNF lists only `CompilerPrivateBlock`; value EBNF lists all accept productions |

## Test map

| Suite | Role |
|-------|------|
| `decompile_accept/accept_forms.as` | Direct compile of each accept form; evaluate where meaningful |
| `decompile_accept/reject_forms.as` | Known rejects + unknown + value-only form in statement position |
| `decompile_accept/emit_and_roundtrip.as` | Source → decompile contains form → d1 == d2 / evaluate |
| `decompile_fidelity.as` | Broad construct matrix (d1 == d2) |
| `decompile.as` / `pragma.as` | Sample string expects / #block evaluate |

When adding a new decompile `#implementation_id`, update this table, accept dispatch, EBNF, and at least one accept or round-trip case.

## `compiler_internal` naming

| Layer | Name |
|-------|------|
| Adaptive function **category** | **`compiler_internal`** (was `compiler_script`) |
| Execute source file | `afw_function_compiler_internal.c` |
| Script formal evaluate | `afw_function_script_evaluate_parameter_with_type` (not in `afw_function.c`) |

**Do not** use category name `private` — reserve for a possible future class/member visibility story.

Function **ids** (`const`, `assign`, …) stay stable for **decompile → compile** round-trip. Distinct from **`#…` compiler-private value forms** and author **`#compile`**.

**Built-in formals:** `afw_function.c` / `AFW_FUNCTION_EVALUATE_*` only — no script typeCheck in that hot path.
