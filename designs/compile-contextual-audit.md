# Compile / call contextual audit

**Status:** Done for core libafw + curl extension (2026-08, `issue-#28-pragma-cleanup`).  
**Related:** multi-unit policy (`type_check_multi_unit.as`), `afw_function_execute_contextual(x)`, Pattern B `#compile`.

## Rules

| Contextual | Use when |
|------------|----------|
| **Parser / create-time** `contextual` | Compile creates call/built_in nodes for this unit |
| **Definition** `script->contextual` | Script formal/return **runtime** checks (prefer over call-site) |
| **Call-site** `args.contextual` / `afw_function_execute_contextual(x)` | Nested `call_create` from a built-in execute so nested values share the outer call’s unit link. |
| **NULL** | No compiled unit: type-check macros use **process flags only**. Prefer only at host/C edges with no call value |

## Core libafw (`src/afw`)

### `afw_value_call_create` first argument

| Site | First arg | Classification |
|------|-----------|----------------|
| `compile/afw_compile_parse_expression.c` | `contextual` | Compile unit |
| `function/afw_function_higher_order_array.c` | `afw_function_execute_contextual(x)` | Nested functor/compare calls |
| `function/afw_function_compiler.c` | `afw_function_execute_contextual(x)` | stringify replacer, etc. |
| `function/afw_function_compiler_internal.c` | `afw_function_execute_contextual(x)` | switch predicate |
| `function/afw_function_adapter.c` | `ctx->contextual` (from `afw_function_execute_contextual(x)` at setup) | retrieve callbacks |
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

## Extensions

| Site | First arg | Classification |
|------|-----------|----------------|
| `src/afw_curl/afw_curl_internal.c` (header/writer/reader `call_create`) | **`script_cb->contextual`** | Plumbed from `afw_function_execute_contextual(x)` at `http_*` / `smtp_send` execute through internal helpers into `afw_curl_internal_script_cb_t`. NULL only when the outer call has no unit link (same as other execute paths). |

## Regression coverage

- Nested creates with unit link: `function_contextual.as`
- Multi-unit policy / definition formals: `type_check_multi_unit.as`

## NULL first-arg hygiene (grep audit)

**As of 2026-08 (pragma-cleanup):** no residual literal `NULL` first argument on:

- `afw_value_call_create`
- `afw_value_call_built_in_function_create`
- `afw_value_call_script_function_create`

under `src/**/*.c` (excluding false positives in comments / docs). Curl header/writer/reader use `script_cb->contextual` from `afw_function_execute_contextual(x)`.

Cheap re-check from package root:

```bash
# Same-line NULL (quick)
grep -Rn 'afw_value_call\(.*\|_built_in_function\|_script_function\)_create' src --include='*.c' \
  | grep -i NULL || true

# First-arg NULL (handles multi-line)
python3 - <<'PY'
import re, os
from pathlib import Path
creators = re.compile(
    r'afw_value_call(?:_built_in_function|_script_function)?_create\s*\(')
for root, _, files in os.walk('src'):
    for f in files:
        if not f.endswith('.c'):
            continue
        p = Path(root) / f
        if 'generated' in p.parts:
            continue
        text = p.read_text(errors='replace')
        for m in creators.finditer(text):
            i, depth, arg0 = m.end(), 1, []
            while i < len(text) and depth:
                c = text[i]
                if c == '(':
                    depth += 1; arg0.append(c)
                elif c == ')':
                    depth -= 1
                    if depth == 0:
                        break
                    arg0.append(c)
                elif c == ',' and depth == 1:
                    break
                else:
                    arg0.append(c)
                i += 1
            first = ''.join(arg0).strip()
            if re.search(r'\bNULL\b', first):
                line = text[:m.start()].count('\n') + 1
                print(f'{p}:{line}: {first[:60]!r}')
print('done')
PY
```

Expect empty output. Any new hit should either pass real contextual or be justified as a true host edge with no unit.

## When adding new code

1. Prefer `afw_function_execute_contextual(x)` (or explicit call/parser contextual) for any nested `call_create` from execute.
2. Do not use NULL unless there is no compiled-value unit to attach.
3. Script formals/returns: definition contextual first.
4. Leave built-in `afw_function_evaluate_parameter` free of typeCheck branching.
