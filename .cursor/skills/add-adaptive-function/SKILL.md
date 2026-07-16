---
name: add-adaptive-function
description: >-
  Adds or changes Adaptive functions or data types via generate/ metadata, afwdev
  generate, hand C implementation, and .as tests. Use when creating a new function,
  editing _AdaptiveFunctionGenerate_ or _AdaptiveDataTypeGenerate_ JSON, implementing
  afw_function_execute_*, or adding polymorphic function metadata.
---

# Add Adaptive function or data type

## Checklist

```
Progress:
- [ ] 1. Metadata JSON under generate/objects/
- [ ] 2. Optional validate
- [ ] 3. ./afwdev build --cdev -j  (generate + cmake + install)
- [ ] 4. Implement in hand C (not generated/)
- [ ] 5. Add/update .as test
- [ ] 6. Re-run ./afwdev build --cdev -j if needed; afwdev test -j
```

## 1. Metadata

**Function** — create/edit:

`src/<srcdir>/generate/objects/_AdaptiveFunctionGenerate_/<functionId>.json`

Required fields typically: `functionId`, `functionLabel`, `category`, `parameters`, `returns`. Optional: `brief`, `description`, `pure`, `op`, etc.

Example: `src/afw/generate/objects/_AdaptiveFunctionGenerate_/and.json`.

**Data type** — use `_AdaptiveDataTypeGenerate_/` (not `_AdaptiveDataType_`).

**Polymorphic** — `_AdaptivePolymorphicFunction_/` with `polymorphicExecuteFunction` / per-type `useExecuteFunction` as in existing entries.

## 2. Validate (optional)

```bash
afwdev validate --pattern 'src/afw/generate/objects/_AdaptiveFunctionGenerate_/<functionId>.json'
```

## 3. Generate + build + install

Prefer the C-dev shortcut from package root (generate, cmake, install core/extensions/afwdev; not JS/docs):

```bash
./afwdev build --cdev -j
```

Use `./afwdev` here because the build may refresh the installed `afwdev` command. This **wipes** matching `generated/` during generate. Do not hand-edit generated bindings or `function_closet/`.

Narrow generate only if needed: `afwdev generate --srcdir-pattern afw`.

## 4. Implement

- Core functions: `src/afw/function/afw_function_<category>.c`
- Signature: `afw_function_execute_<label>(afw_function_execute_t *x)`
- Use `AFW_FUNCTION_EVALUATE_*`; context is `x->xctx`
- Core `.c` includes `afw_internal.h`
- Data-type behavior: often `src/afw/data_type/afw_data_type.c` plus generated bindings

If generate created a new category stub only under `generated/function_closet/`, copy the pattern into hand-maintained `function/` and ensure CMake picks it up via generate variables. Re-run `./afwdev build --cdev -j` after implementing.

## 5. Test

Add `.as` under `src/afw/tests/<area>/` with `test_script` / `//?` metadata (see `src/afw/tests/rql/eq.as`).

```bash
afwdev test -j
afwdev test --srcdir-pattern afw --pattern '<area>/.*'
```

Judge success from command output.

## Pitfalls

- Editing `generated/` — lost on next generate
- Implementing only in `function_closet/` for core — build uses `function/`
- Skipping generate after JSON changes — missing declarations / link errors
- Raw `malloc` — use pools / `afw_xctx_*`
