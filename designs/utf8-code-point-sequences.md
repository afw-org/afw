# UTF-8 values as immutable code-point sequences

**Issue:** [#153](https://github.com/afw-org/afw/issues/153)  
**Assignee:** Mike  
**Status:** design / tracking (2026-08); implement over coming weeks/months.  
**Audience:** maintainers.

## Product statement

> **UTF-8 Adaptive values are immutable sequences of Unicode code points.**  
> - **`length`**, **indexing (`s[i]`)**, **for-of**, and **array-consuming APIs** may treat them as that sequence.  
> - Elements are **one-code-point strings** (same idea as for-of on strings today).  
> - The value’s **data type stays** `string` / `anyURI` / other utf8-backed types—it is **not** retyped to `array`.  
> - **Mutation** of “elements” is not supported (no `s[i] = …`); new text only via string functions.  
> - **`afw_utf8_t` is always valid NFC UTF-8**; never treat “one character” as one octet. Invalid UTF-8 as a normal string value is a serious bug.

Applies to **all data types whose internal representation is `afw_utf8_t`**, not only `string`. Prefer a generate/data-type flag (name TBD: e.g. code-point sequence / utf8-indexable) rather than a hand-maintained type list in C.

## Encoding vs ECMAScript

| | Adaptive | ECMAScript (typical) |
|--|----------|----------------------|
| Storage | **UTF-8** only | Often **UTF-16 code units** |
| Length / indexes | **Code points** in UTF-8 | **Code units** (astral often 2) |
| `\u` in source/JSON | Heritage escapes → still stored as UTF-8 | Native string model |

See also `typescript-differences.md` (motivated differences).

## What already exists (partial)

| Piece | Notes |
|-------|--------|
| `length` / `substring` / `index_of` family | Code-point oriented |
| for-of over strings | Walk code points (branch work on `test262-skipreason-sweep`) |
| empty-separator `split` | Must be code points (octet step is a bug) |
| `s[i]` on strings | **Not** yet |
| HOFs / array formals with string | **Not** yet (need face or shared sequence) |
| Immutable array face over utf8 | **Not** yet |

Specialized hot paths (e.g. **substring**) may stay hand-tuned; they must share the **same definition of index** (code point). Prefer thin shared `afw_utf8_*` helpers over copy-pasted loops (“get it right at most once”).

## Goals

1. **C helpers** — count / byte offset of code-point index *i* / one-CP slice / foreach.  
2. **`s[i]`** (read) on utf8-backed values; out-of-range policy align with array soft get where product agrees.  
3. **Array consumers** — built-ins first: accept utf8-backed values where an array of values is consumed, via:
   - immutable **array face/view** over code points, and/or  
   - shared sequence access in built-in evaluate.  
4. **Script functions** — same coercion at call/bind when formal expects **array** (not silent wrap for every string use).  
5. **Easy opt-in** — metadata + one choke point so support is not N special cases per HOF.  
6. **Docs + tests** — handbook, differences, test262 / language tests.

## Non-goals

- String **is** the `array` type in the type lattice for all purposes.  
- Mutable string buffers / `s[i] = x`.  
- Auto-wrap in non-array contexts (object keys, concat, …).  
- Full ES iterator protocol / generators.

## Suggested implementation order

1. This pad + GitHub **#153**.  
2. C helpers on `afw_utf8_*`.  
3. Immutable array face create-from-utf8 (any utf8-backed value).  
4. `s[i]` read path.  
5. Built-in array formals accept face / wrap at evaluate.  
6. Script function parameter bind: same wrap when formal is array.  
7. Fold for-of string branch onto helpers (optional fast path).  
8. Tests + docs; fix any remaining octet-as-character bugs.

## Coercion policy (draft)

| Do | Don’t |
|----|--------|
| Explicit sequence access + wrap when **array is expected** | Silent wrap on every string use |
| Immutable view / face only | Mutable array editing the string |
| Opt-in formal / evaluate choke point | Hand list of HOFs only, forever |
| One-char **string** elements | Integer code-point elements (unless separate API later) |

## Related

- [#153](https://github.com/afw-org/afw/issues/153) — tracking issue  
- [#39](https://github.com/afw-org/afw/issues/39) — dense arrays (closed)  
- [#22](https://github.com/afw-org/afw/issues/22) — author ES differences  
- `designs/array-semantics.md`, `designs/conversion-functions.md`  
- Branch `test262-skipreason-sweep` — for-of / split / triage (may land separately)
