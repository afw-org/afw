# Type-named conversion functions

**Status:** product decisions (2026-08), with #39 array work.  
**Audience:** maintainers / handbook authors.

## What they are

Optional **conversion functions** whose **functionId equals a data type id**. Typical shape:

- pure, one argument `value`
- return that data type (or `cast_error` if conversion is impossible)
- most share `useExecuteFunction: afw_function_execute_convert` → `afw_value_convert` using `returns.data_type`

They are **hand** entries under `generate/objects/_AdaptiveFunctionGenerate_/<type>.json`, not auto-emitted from `_AdaptiveDataTypeGenerate_`. No data-type flag controls them today.

A **type name** for annotations (`: integer`) is independent of whether a convert exists.

## Who has no conversion function

| Types | Why |
|-------|-----|
| `any`, `undefined`, `void`, `unknown`, `unevaluated` | Meta / non-value kinds |
| **`null`**, **`function`** | Data types kept (JSON null; function values / types). Type-named converts **removed**: names clash with literal/syntax; convert was useless or uncallable in script. |

## Special cases (same id, not pure single-arg convert)

| Function | Role |
|----------|------|
| **`string(...)`** | Convert each arg to string; **concatenate** if more than one (undefined → text `undefined`) |
| **`array(...)`** | **Element constructor** + optional `...` spread; **not** convert-to-array. `array([1,2])` is `[[1,2]]`. |
| **`create_array(n)`** | Length-based constructor; all entries **undefined**. Not a conversion function. |
| **`object(x)`** | Convert: JSON/relaxed object **text** → object, or object identity. Not `{…}` constructor. |

## Source-holding types

`json`, `relaxed_json`, `script`, `template`, `regexp`, `xpathExpression` (and similar):

- Type-named convert produces a value that **holds source text** of that kind.
- It does **not** compile or evaluate the source.
- Polymorphic **`compile`** (and related) compile that typed source.
- Useful as Adaptive object **property** types for “this field is script/json/… source.”

Stringy typed values (`anyURI`, `dnsName`, …) remain classic convert-from-string (XACML-shaped).

## Related

- Array product pad: [`array-semantics.md`](array-semantics.md)
- Branch notes: `whats-new.md` (conversion functions section)
