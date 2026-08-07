# UTF-8 values as immutable code-point sequences

**Issue:** [#153](https://github.com/afw-org/afw/issues/153)  
**Assignee:** Mike  
**Status:** design / tracking (2026-08); implement over coming weeks/months.  
**Branch:** `issue-#153-utf8-code-point-sequences`  
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

## Iterator / sequence access (design lock-in, 2026-08)

Brainstorm on this branch: polymorphic walk and index for utf8 (and
later other types) via a light **`afw_iterator`**, not string-only special
cases forever. Specialized array/object `get_next_*` **opaque cursor**
paths stay for hot C; the first-class iterator is the polymorphic front
door.

### Migration: rename step (done on this branch)

The previous first-class interface and cursor type were renamed so the
**new** design can own the name:

| Old (legacy) | Role until replaced |
|--------------|---------------------|
| **`afw_iterator_old`** / **`afw_iterator_old_t`** | Legacy IDL interface + opaque `get_next_*` cursor token |
| **`afw_iterator_old_*` macros** | `release` / `next` on the legacy interface |

**`afw_iterator`** is reserved for the new `defined_instance_storage` design
(`get_next` / `get_by_index` / `get_count`, stack initialize, no instance
release).

### Interface instance model

| Mode | XML | Lifecycle |
|------|-----|-----------|
| **Created** (default, almost all interfaces) | attribute omitted | create → `const T *`; pool/release as today |
| **Defined** | `defined_instance_storage="true"` | Caller **defines** full public `afw_*_t` (stack or embed); **host** initializes it (sets `inf`); **no instance release** |

- Name **`defined_instance_storage`** matches C wording (declare vs **define**).
- **No** generated zeroing `afw_iterator_initialize()` macro: only the host
  knows which `inf` to install (like `get_setter` knowing its inf).
- Host API example: **`afw_array_initialize_iterator(array, &it, xctx)`**.
  Later: object, utf8 value, etc. See `src/afw/doc/developer/interfaces.md`.
- Capability discovery: host method / inf presence, not a docs flag in the
  hot path.

Stack-resident iterator: fixed public layout holds `inf` + cursor words;
caller provides memory; common path needs no alloc/release. Impl may
spill to a pool only when the fixed cursor is too small; that cleanup is
impl-managed, not a required caller `release`.

### Two interfaces

| Interface | Walk shape | Typical host |
|-----------|------------|--------------|
| **`afw_iterator`** | Values only | array, string code points |
| **`afw_iterator_with_key`** | Key + value each step | object properties (later) |

Both are `defined_instance_storage`. Array has **`initialize_iterator`** only
for keyless **`afw_iterator`**.

### Method names

**`afw_iterator` (keyless):**

| Method | Role |
|--------|------|
| **`get_next`** | Next value, or **NULL** when done (like `get_next_value`) |
| **`get_by_index`** | Value at dense index *i*; does not advance `get_next` |
| **`get_count`** | Dense length when indexable |

**`afw_iterator_with_key`:**

| Method | Role |
|--------|------|
| **`get_next`** | Key + value out-params; **true** when done |
| **`get_by_index`** | Key + value out-params at dense index *i*; **true** if no entry; does not advance `get_next` |
| **`get_count`** | Dense length when indexable |

- NULL from keyless `get_next` means **end**, not an undefined element.
- **`s[i]`** / formals use index/count on keyless iterator; for-of may use
  `get_next` or specialized fast paths sharing utf8 helpers.

### Host initialize + use (array)

```c
afw_iterator_t it;
afw_array_initialize_iterator(array, &it, xctx);  /* sets inf + cursor */
while ((v = afw_iterator_get_next(&it, p, xctx)) != NULL) { ... }
v = afw_iterator_get_by_index(&it, i, p, xctx);
n = afw_iterator_get_count(&it, xctx);
```

### Relation to #153 phases

1. Shared **`afw_utf8_*`** helpers (count / offset / one-CP slice).  
2. **`s[i]`** + tests (may call helpers directly before full iterator land).  
3. **`defined_instance_storage` + iterator methods** in IDL/generate when
   ready; string (and array identity) implement them.  
4. Formals / HOF / fold for-of onto the common path.

### `afw_data_type` optional_initialize_iterator (implemented on branch)

- Inf method **`optional_initialize_iterator`** (NULL if type has no sequence).
- Macro **`afw_data_type_initialize_iterator`** → throws if optional is NULL.
- Macro **`afw_data_type_has_iterator`** → optional method non-NULL.
- Wired in **`afw_data_type.c`**: **array** → `afw_array_initialize_iterator`;
  all **`afw_utf8_t`** types → shared code-point iterator yielding **managed
  string** elements; others NULL.

### `iteratorReturnDataType` + value façade (implemented on branch)

- Generate property **`iteratorReturnDataType`** (camelCase; data type id string)
  → C field **`iterator_return_data_type`** (`const afw_data_type_t *` or NULL)
  on the static data type instance.
- **utf8 family** → `string`; **array** → NULL (iterates; element type per
  instance); others NULL.
- Capability (method non-NULL) and fixed step type (pointer) are **separate**.
- Data type macros: `afw_data_type_has_iterator`,
  `afw_data_type_initialize_iterator`,
  `afw_data_type_iterator_return_data_type`.
- Value macros (public API in `afw_value.h`; no open-code inf walks):
  - **`afw_value_has_iterator`** / **`afw_value_initialize_iterator`** —
    evaluated face (`is_evaluated_of_data_type`).
  - **`afw_value_iterator_return_data_type`** — quick produce type
    (`inf->data_type` → field); no `get_data_type()` call.
- Adaptive runtime property uses value accessor **`data_type_id`** (pointer →
  type id string).
- **Deferred (not this branch’s first priority):** produce-type percolation /
  compile-time optimize — see `designs/compile-optimize-notes.md` and
  [comment on #28](https://github.com/afw-org/afw/issues/28#issuecomment-5222169246).
  Optional later: rewire for-of / formals / `s[i]` onto iterators.

## Related

- [#153](https://github.com/afw-org/afw/issues/153) — tracking issue  
- [#39](https://github.com/afw-org/afw/issues/39) — dense arrays (closed)  
- [#22](https://github.com/afw-org/afw/issues/22) — author ES differences  
- `designs/array-semantics.md`, `designs/conversion-functions.md`  
- `src/afw/doc/developer/interfaces.md` — IDL / generate notes (extend when
  `defined_instance_storage` lands)  
- Existing array/object cursor `get_next_*` — keep for efficiency  
- Branch work on for-of string / split code points (landed on `mgg-develop`)  

