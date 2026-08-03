# Compile / call contextual audit

**Status:** Done for core libafw (2026-08, `issue-#28-pragma-cleanup`).  
**Related:** multi-unit policy (`type_check_multi_unit.as`), `AFW_FUNCTION_CONTEXTUAL`, Pattern B `#compile`.

## Rules

| Contextual | Use when |
|------------|----------|
| **Parser / create-time** `contextual` | Compile creates call/built_in nodes for this unit |
| **Definition** `script->contextual` | Script formal/return **runtime** checks (prefer over call-site) |
| **Call-site** `args.contextual` / `AFW_FUNCTION_CONTEXTUAL` | Nested `call_create` from a built-in execute so nested values share the outer call’s unit link |
| **NULL** | No compiled unit: type-check macros use **process flags only**. Prefer only at host/C edges with no call value |

## Core libafw (`src/afw`)

### `afw_value_call_create` first argument

| Site | First arg | Classification |
|------|-----------|----------------|
| `compile/afw_compile_parse_expression.c` | `contextual` | Compile unit |
| `function/afw_function_higher_order_array.c` | `AFW_FUNCTION_CONTEXTUAL` | Nested functor/compare calls |
| `function/afw_function_compiler.c` | `AFW_FUNCTION_CONTEXTUAL` | stringify replacer, etc. |
| `function/afw_function_compiler_internal.c` | `AFW_FUNCTION_CONTEXTUAL` | switch predicate |
| `function/afw_function_adapter.c` | `ctx->contextual` (from macro at setup) | retrieve callbacks |
| `value/afw_value_call.c` | (API implementation) | — |

**No `afw_value_call_create(NULL, …)` in core libafw.**

### Built-in / script create helpers

| API | First arg pattern |
|-----|-------------------|
| `afw_value_call_built_in_function_create` | `contextual` from compile / call path (no NULL first arg found) |
| `afw_value_call_script_function_create` | call-site `contextual`; formals/returns use definition when present |
| `afw_value_call_built_in_function` (immediate) | e.g. action path with request contextual |

### Type-check helpers

- `AFW_VALUE_TYPE_CHECK_*(contextual, xctx)` — NULL contextual ⇒ process flags (`afw_value.h`).
- Runtime assignability: `call_script_function` (formals/returns), `compiler_internal` assign/Pattern — always pass a real contextual when available; gates inside `type_check_assignable`.
- Compile assignability: parse + known Adaptive call sites with parser/call contextual.

## Extensions residual

| Site | First arg | Classification |
|------|-----------|----------------|
| `src/afw_curl/afw_curl_internal.c` (×3) | **NULL** | Libcurl C callbacks invoke script functors after `http_*` execute returns into curl; `script_cb` does not yet store call-site contextual. **Intentional residual:** process flags for nested typeCheck until contextual is plumbed from `afw_curl_function_execute_http_*` through internals into `afw_curl_internal_script_cb_t`. |

## Regression coverage

- Nested creates with unit link: `function_contextual.as`
- Multi-unit policy / definition formals: `type_check_multi_unit.as`

## When adding new code

1. Prefer `AFW_FUNCTION_CONTEXTUAL` (or explicit call/parser contextual) for any nested `call_create` from execute.
2. Do not use NULL unless there is no compiled-value unit to attach.
3. Script formals/returns: definition contextual first.
4. Leave built-in `afw_function_evaluate_parameter` free of typeCheck branching.
