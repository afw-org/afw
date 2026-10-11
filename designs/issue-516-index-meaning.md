# #516 — Index queries mean what the index holds

**Issue:** [#516](https://github.com/afw-org/afw/issues/516) (and its case-insensitive comment). **Branch:** `fix/516-index-meaning`. **Status:** implemented on the branch; decisions below agreed with Jeremy 2026-10-10.

## The bug in one line

An index query used two meanings for one filter term: the **cursor** used the index's (value script, filter, lowercased keys), the **re-test**, the **dedup** and the **scan** used the object's property. Whenever they differ, the answer depends on the plan.

| Case | Symptom before |
|---|---|
| Synthetic name (`FullName`, value script) | `and(FullName eq "Ada", …)` and any scan: 0 |
| Index with a `filter` on a real property | Query via the index misses objects the filter left out (a scan finds them) |
| `case-insensitive-string` | `s eq "SMITH"` alone: both; `and` / `or` / scan: case-sensitive |
| Dedup of an `or` on one such name | `or(FullName eq "Ada", FullName ge "A")` returned `a` twice |

## Decisions

1. **The index definition decides what its name means**, on every path: the index query's re-test, the dedup, and the adapter's scan (LMDB `afw_lmdb_adapter_session_dump_objects`, including #515's fallback scans). The other choice, "index is only a speed-up" (reject value scripts, filtered indexes only as candidates), was turned down: it removes case-insensitive matching, which we keep.
2. **Computed name** = a definition with a `value` script (maybe a `filter`). A relation on it tests what the script gives the object (`current::` as when indexing); an object the filter leaves out has **no value** under the name (the relation is false, as for any missing property since #548). After `index_remove`, queries on the name find nothing.
3. **`filter` needs `value`.** A filter only makes sense with a synthetic name: on a real property it would hide objects that have the property. `index_create` refuses a filter without a value script.
4. **A computed name must not be a declared property.** `index_create` refuses a value or filter script on a key that an object type in `objectType` declares (`otherProperties` doesn't count). A schema changed later is caught **at query time**: the per-query walk throws when the query's object type declares a computed name (`entry->pt` is not the object type's `other_properties`). Object types are cached for a request (session cache), so a change shows from the next request.
5. **Object types and computed names:** with an object type, the query parser refuses names it doesn't declare unless `otherProperties` allows query. So a computed name on a typed object needs queryable `otherProperties`. Accepted and documented for now (see Future).
6. **Case-insensitive applies to every relation**, `match` included: the pattern is lowercased like the values except escapes (`\S`, `\d`, `\p{…}` kept), and recompiled once per query. `\p{Lu}` never matches a lowercased value (documented).
7. **Pay per use** (Jeremy's rule: features cost only those who use them). `afw_adapter_impl_index_query_test_create` walks the filter once per query; it returns NULL when no relation names a computed or case-insensitive index, and callers then use `afw_query_criteria_test_object` unchanged. Scripts compile once per query. `inner_join` stays: a cursor on one term is exact under the index's meaning, so a single-term query still skips the re-test.
8. **One filter walker.** `afw_query_criteria_test_object_cb` takes a value hook; `afw_query_criteria_test_object` passes none. A copy of the walker in the index code was considered and dropped: #548 had just changed the walker's semantics (missing property), and a copy would have drifted.
9. **Dedup by key, not by meaning.** `afw_adapter_impl_index_applies` asks "does this later cursor return the object": the object's values through the definition, as index keys (same encoding, lowercasing, empty keys skipped), compared with the key the cursor seeks (`impl_index_entry_seek_key`, shared with `cursor_list`). A semantic test there could skip an object a later cursor never returns.

## Map (code)

| Piece | Where |
|---|---|
| Walker with hook | `afw_query_criteria_test_object_cb` (`query_criteria/afw_query_criteria.c`) |
| What a definition gives an object (writes, re-test, dedup) | `impl_index_evaluate` + `impl_index_compile_scripts` (`adapter/afw_adapter_impl_index.c`) |
| Per-query test, conflict check, trace | `afw_adapter_impl_index_query_test_create` / `_test_object` |
| Case-insensitive copies of entries | `impl_index_entry_to_lower`, `impl_index_pattern_to_lower` |
| `index_create` rules | top of `afw_adapter_impl_index_create` |
| Scan | `afw_lmdb_adapter_session_dump_objects` (one test per object type) |
| Trace | `index query test: <name> through its index definition (computed|case-insensitive)` on `trace:adapterId:<id>` |

Tests: `afw_lmdb/tests/indexes/index_computed_meaning.as`, `index_case_insensitive_meaning.as`, `index_computed_rules.as`, `index_computed_trace.py` (trace, and the later-schema conflict across two `afw` runs). Each meaning test runs a filter as given (index), `and`'d with a term every object passes (index + re-test), and `or`'d with one every object fails (scan).

## Future options (not done)

- **Declare a computed name in the object type.** A property-type flag (e.g. "computed by index") that rule 4 would accept, so typed objects without `otherProperties` can query computed names and the schema documents them.
- **Detect a conflict with the data, not only the schema:** when a computed definition indexes an object that has a real property of that name, trace it (one property lookup per write, computed definitions only).
- **Classify definitions once when they are read**, instead of per query (per query is already cheap: one walk of the filter).
- **Compile index scripts once per definition** for writes too (they still recompile per write; atlas #381 note).
- **#544** (done): empty strings are index keys (`""` is `"\0"`, a text led by `"\0"` gets another; `impl_index_key_from_text`), and the `integer` / `double` options say an index holds numbers, so a string query value is the number's key without an object type (the only way for a computed name). A query value that doesn't convert re-tests its cursor. Mixed integer and double values compare apart, as the walker does (`double("9")` and mixed numbers are #543 questions).
- **Array-valued properties** (found here, held as an issue draft): the walker's `eq` on an array never matches and `lt`… throw (`impl_compare_value` converts to the array type, not the element type), while an index holds one key per element. Backlog entry in `beta-backlog.md` (LMDB index bugs).
