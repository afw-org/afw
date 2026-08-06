# Adaptive array semantics (issue #39 and related)

**Status:** product decisions for Adaptive Script `array` (2026-08).  
**Branch work:** `issue-39-list-literal-holes` and follow-ons.

## What an array is

- Ordered **dense** sequence of values (data type **`array`**).
- **Not** an object: no property model, no sparse “missing index,” no ES exotic array.
- XACML **bag-*** builtins may **store** arrays and apply bag **function** semantics; script still sees `array`.

## Literals and content types

| Surface | Elision `['a', ,'b']` |
|---------|------------------------|
| Script / template | Allowed → middle **undefined** (dense) |
| `relaxed_json` | Same as script |
| Strict `json` | **Forbidden** (RFC JSON) |

Holes are **not** a third state: elision inserts **`undefined`**.

Trailing commas before `]` are allowed in non-strict (script / relaxed); not in strict JSON.

## Index get / set

| Op | Behavior |
|----|----------|
| `a[i]` get, in range | Element (may be undefined) |
| `a[i]` get, OOB | **Throw** (`Index out of range for array`) |
| `at(a, i)` OOB | **undefined** |
| `a[i] =` in range | Replace |
| `a[i] =` with `i === length` | **Append** (grow by one) |
| `a[i] =` with `i > length` | **Throw** (no gap fill) |
| Negative index assign | Resolve into existing range only (no grow from end) |

## Create helpers

| API | Meaning |
|-----|---------|
| `[]` / `array(...)` | Empty or from elements |
| **`empty_array(n)`** | Dense length **n**, all **undefined**; `n` in `0 .. 1_000_000` |

No fill argument on `empty_array` (a separate helper can be added later if needed).

## Explicit non-goals

- `for-in` / `in` / `delete` (reserved; arrays are not objects)
- Sparse present-bits / assign gap fill
- TypeScript `Array<T>` type spelling (use **`T[]` only**)
- Size-in-type (`array[5]`)

## Tests

- `src/afw/tests/language/script/array_semantics.as` — elision, assign-append, empty_array, OOB, HOFs/`for-of` with undefined slots

## Related

- Issue **#39** (holes)
- Faces / shared literals: **#17** (closed)
- Types: **#28** (`T[]` only after dropping `Array<T>`)
- `typescript-differences.md` — array notes and non-goals
