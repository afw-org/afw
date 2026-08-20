# Issue #2 — property names as values

**Audience:** maintainers / assistants. **Not** handbook.  
**GitHub:** umbrella [#2](https://github.com/afw-org/afw/issues/2) (memory). Another feature branch in that campaign — not the first, not the last.  
**Parent pad:** [`memory-management.md`](memory-management.md) (do not dump this novel there).  
**Related:** [#38](https://github.com/afw-org/afw/issues/38) construct `[expr]:` (closed; still converts names to utf8); [`c-naming-and-payloads.md`](c-naming-and-payloads.md) (utf8 has no pool).

**Status:** discussed 2026-08-20. **No code until execute is requested.** One feature branch off `develop` for this C/script/JSON cut plus the leftover autoconvert cleanup listed below. Name should blame #2 (example: `issue-2-property-name-values`). Local commits; push only when we would keep the branch.

#2 is a **retrofit**: memory was built for **request** lifetime (bulk free when the request-session pool dies). Adaptive Script added **scope** lifetimes (nested, assign, escape, closures) inside and across those requests. Faces, catalog, permanent `afw_v_*`, dual `->value`, pool-release-returns-NULL already landed on other feature branches. This branch is names-as-values only; more #2 branches will follow.

## Why (values first)

Object property names are `const afw_utf8_t *` today. Payloads have no pool; the setter comment is “name must live as long as the object.” Generated strings already emit **both** `afw_s_*` (utf8 view) and `afw_v_*` (`&afw_self_v_*.pub`) — that bag exists so names can be values.

This slice: **`afw_object` / `afw_object_setter` (and helpers that are really a property name) take `const afw_value_t *`.** Implementations compare with **`afw_value_equal`** (already type + internal for evaluated values; pointer identity first). Store-as-is lifetime, same contract as utf8; extra `inf` pointer only. **`clone_or_reference` of names is later #2**, not this branch.

Call sites that already pass `afw_s_foo` become `afw_v_foo`. Dynamic names: `afw_value_create_unmanaged_string` in the object (or compile) pool — copies the `afw_utf8_t` struct, not the bytes (same as utf8 today).

## Layering (settled)

| Layer | Property names |
|--------|----------------|
| Object / setter implementations | Names are values. Exact type+internal. **No extra ifs** (not “is it a string?”, not JSON, not script). |
| Adaptive Script | **String only** for now. Throw if the name is not a string value. No `as_utf8` autoconvert. |
| JSON / YAML / UBJSON | **String only.** Read: JSON keys → string values. Write: string value → JSON string key; otherwise throw. Do **not** `stringify` a name as a key (that is value text, not a key). |
| Object-type `propertyTypes` | Still string-keyed metadata. Lookup only when the runtime name is a string (`.internal`). JSON **values** still use the existing normalize / `propertyTypes[name].dataType` dance (admin app too). Do not start that dance for **keys**. |

**`reference_by_key` is the future door** for non-string object keys (`obj[integer]`, `obj[memory]`, …). Do not invent that now.

### Later (weeks, not this branch) — JSON lost XML metadata

XML was AFW’s first content type and could carry the extra metadata. JSON became the accepted wire; that extra was stripped. To get type/meta to the admin app we grew **object options** plus **`afw_object_view`** packaging (including reserved `"_meta_"` — [#138](https://github.com/afw-org/afw/issues/138)). Without that metadata, a JSON string is just a string: you cannot know the Adaptive data type of a **value** (date vs anyURI vs string) or, later, of a **name**.

A later encoding will likely put type (and maybe more meta) **in the JSON string** so the app can harvest it more easily than today’s view/options dance. A **primitive** form for names (candidate only): if the key contains `:` or `>` (or similar), what is before is a data type id and what is after is `to_string` of the value. That also covers stores whose keys are not Unicode — old national encodings, SQL column names as **`hexBinary`** rather than utf8, and so on.

**Do not design or implement that encoding here.** This branch: throw if the name is not a string value. The edge helper is the place that encoding would grow; object implementations still will not care. When that work starts, it can fill script/JSON/YAML/UBJSON (everywhere this branch is string-only) in one pass.

Indexing already split:

| Aggregate | Key | Policy |
|-----------|-----|--------|
| Array | integer (negatives from the end) | already throw if not integer |
| String / utf8 iterator | integer code-point index | already throw if not integer |
| Object | property name | leftover `as_utf8` — **this branch: string only, throw** |

`obj[1]` on an **object** becomes a type error instead of looking up `"1"`. Same class of tightening as built-in parameter types. `arr[1]` unchanged.

## Equality

Use existing **`afw_value_equal`**. It is exact for evaluated values: same `is_evaluated_of_data_type` (the data type, not the lifetime inf) then `compare_internal`. Permanent / unmanaged / managed string infs share `afw_data_type_string`, so JSON-parsed `"foo"` matches `afw_v_foo`. Script `eqx` / `===` is the language twin. Do not invent a new C exact-equal for names. Do not use loose `eq` / `==` for lookup.

Unevaluated names (`call`, `block`, …): throw at the **script/JSON helper**, not inside every object impl.

NULL name **pointer** is invalid. `null` / `undefined` as a name: script/JSON throw. `set_property(..., NULL)` remains **delete** (value, not name).

## Not in this slice

- `objectId`, `objectTypeId`, path, adapter ids — stay utf8.
- `get_property_extended` dotted paths / `_meta_.path` — utf8 path syntax.
- Literal `_meta_:` peel stays the **string** `"_meta_"` at parse (#38). Computed name that is the string `_meta_` is a normal property.
- Intern of hot names, `afw_value_hash`, hash-table objects.
- Name `clone_or_reference` / optional_release (later #2).
- Sibling package conversion (prompt after this tree is sure).
- `AFW_VALUE_STRING_LITERAL` as a **stored** compound literal (dangling if inf is permanent). Prefer generated `afw_v_*` or `static const afw_value_string_t`.
- JSON/XML metadata encoding, object_view/options replacement, typed keys in script, hexBinary/non-Unicode names — **later** (see above). Not #138’s `"_meta_"` collision either, though they share the “JSON stripped XML meta” history.

## Leftover autoconvert (cleanup **on this branch**)

Old expression language (and early Adaptive Script) autoconverted. Built-in parameter checks later required exact types. These object-name edges still convert — missed in that campaign. Fix them with the helper, same branch as the C type change.

| Site | Today | This branch |
|------|--------|-------------|
| `afw_value_reference_by_key.c` object path | not string → `afw_value_as_utf8` (`obj[1]` → `"1"`) | string or throw |
| `afw_value_object_construct.c` `impl_name_from_value` | same (`{ [1]: x }`) | string or throw |
| Assign through `reference_by_key` | likely same convert — verify while there | string or throw |

Already strict (leave as-is except pass `const afw_value_t *` into object APIs):

- `property_get` / `property_exists` / `property_delete` — `name: string` via `AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER`
- `property_delete_by_reference` — throws if key is not string
- Array / string iterator indexes — integer required

## Stumbled-on, **not** this branch

Recorded so they are not only chat. Do not mix into the XML/`s`→`v` sweep.

| Finding | Notes |
|---------|--------|
| **`eq` / `==`** | Metadata says convert arg2 to arg1’s type. C `afw_function_execute_eq` **throws** if data types differ. Same typecheck-campaign leftover; separate fix. |
| **`properties_ht`** | Memory object comment / union for a property hash table; lookup is still a linear list + `afw_utf8_equal`. Residual; not required to land names-as-values. |

When pairing on #2, keep adding rows here (or a later leftovers pad) instead of growing `memory-management.md`.

## Suggested execute order (when asked)

1. Edge helper: string property name or throw (compiler, JSON/YAML/UBJSON write).
2. Interface XML + generate (`afw_object` / setter, `const_objects` name field, `afw_runtime_property_t`).
3. Memory object + helpers (store `const afw_value_t *`, `afw_value_equal`).
4. Other object impls until the tree compiles.
5. Mechanical `afw_s_` → `afw_v_` call-site sweep; wrap remaining dynamic utf8 as unmanaged string values.
6. Compiler construct + `reference_by_key` object path (kill `as_utf8`).
7. Content types: string values in/out; throw otherwise.
8. Tests: generated-name match; two unmanaged strings with same text; `obj[1]` on object throws; `arr[1]` still works; JSON string keys; #38 tests updated for throw-not-convert.
9. `./afwdev build --cdev` while iterating; fulldev + valgrind before considering push.

Do not hand-edit `generated/`.
