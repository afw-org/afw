# Type-named conversion functions

**Status:** shipped on **`mgg-develop`** with #39 / [PR #152](https://github.com/afw-org/afw/pull/152) (2026-08-06). The generic converts (`integer`, `double`, `dateTime`, …) were removed by [#589](https://github.com/afw-org/afw/issues/589) (2026-10-12): typed literal `"…"T` for text you write, `to_T(v)` to convert, `v as T` to assert.  
**Audience:** maintainers / handbook authors.

## What they are

Optional **conversion functions** whose **functionId equals a data type id**. Typical shape:

- pure, one argument `value`
- return that data type (or `conversion_error` if conversion is impossible)
- `boolean` and the source holders share `useExecuteFunction: afw_function_execute_convert` → `afw_value_convert` using `returns.data_type` (so do the `to_T` polymorphic functions)

They are **hand** entries under `generate/objects/_AdaptiveFunctionGenerate_/<type>.json`, not auto-emitted from `_AdaptiveDataTypeGenerate_`. No data-type flag controls them today.

A **type name** for annotations (`: integer`) is independent of whether a convert exists.

## Who has no conversion function

| Types | Why |
|-------|-----|
| `any`, `undefined`, `void`, `unknown`, `unevaluated` | Meta / non-value kinds |
| **`null`**, **`function`** | Data types kept (JSON null; function values / types). Type-named converts **removed**: names clash with literal/syntax; convert was useless or uncallable in script. |
| `anyURI`, `base64Binary`, `date`, `dateTime`, `dayTimeDuration`, `dnsName`, `double`, `hexBinary`, `ia5String`, `integer`, `ipAddress`, `objectId`, `objectPath`, `password`, `rfc822Name`, `time`, `x500Name`, `yearMonthDuration` | Removed by #589: `T(x)` hid whether `x` was text to read or a value to convert. Typed literal or `to_T(v)`; the compile error for `T(…)` names them. |

## Special cases (same id, not pure single-arg convert)

| Function | Role |
|----------|------|
| **`string(...)`** | Convert each arg to string; **concatenate** if more than one (undefined → text `undefined`) |
| **`array(...)`** | **Element constructor** + optional `...` spread; **not** convert-to-array. `array([1,2])` is `[[1,2]]`. |
| **`create_array(n)`** | Length-based constructor; all entries **undefined**. Not a conversion function. |
| **`object(x)`** | Convert: JSON/relaxed object **text** → object, or object identity. Not `{…}` constructor. |
| **`boolean(v)`** | ECMAScript `Boolean()` truthiness (#545 / #550); boolean **text** is `string_to_boolean(s)` (#588). |

## Source-holding types

`json`, `relaxedJson`, `script`, `template`, `regexp`, `xpathExpression` (and similar):

- Type-named convert produces a value that **holds source text** of that kind.
- It does **not** compile or evaluate the source.
- Polymorphic **`compile`** (and related) compile that typed source.
- Useful as Adaptive object **property** types for “this field is script/json/… source.”

`regexp` and `xpathExpression` also have typed literals, which `decompile` writes; `json`, `relaxedJson`, `script`, and `template` decompile as `json("…")` and so on.

## Related

- Array product pad: [`array-semantics.md`](array-semantics.md)
- Branch notes: `whats-new.md` (conversion functions section)
