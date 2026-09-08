# afw_hash_table — C map (apr_hash replacement)

**Audience:** maintainers. **Not** handbook.  
**Branch:** `feature/afw-hash-table`. Same campaign as [`afw-vector.md`](afw-vector.md).

## Why

Drop `apr_hash` so leftover `get_apr_pool()` is not required for name→pointer tables. The pain is **APR**, not the idea of a hash table.

## Settled

- **Not a sorted vector / binary search.** Env function registry and compile intern **keep inserting**. Insert-into-sorted-array is the wrong cost.
- **In-memory btree-ish** is a later option for “load then find,” especially short names. Not required to replace `apr_hash`.
- **Hash is the drop-in.** Keys are `key` + `klen` → pointer. Get/set/iterate, grow as you go. Hash is APR’s `hash * 33 + byte`, then `memcmp`.
- Name it **`afw_hash_table`**, parallel to **`afw_vector`**: AFW pool, typed overlay (`AFW_HASH_TABLE_STRUCT` / `afw_void_hash_table_t`), `const … internal` like an interface instance, no `get_reference`.
- Keys are **not copied**. Caller keeps key memory alive (same as APR). Set value **NULL** deletes.
- Iterator is a **stack** `afw_hash_table_index_t` (no pool for `first()`).
- Intern-then-id is a later lever, not a prerequisite.

## Converted on this branch

Env registries, compile intern / script type names, runtime catalog, adapter session and object-type caches, model object types, object-view intern, associative_array wrappers, service-id set, YAML anchors, LDAP schema name tables.

## Not converted

- LMDB `dbi_handles` — leave while **#299**.
- Remaining `apr_array` (env `registry_types` / `data_type_methods`, ldap mods, index).

## Later, not first

- Ordered tree if something needs range listing, not “find this name.”
- Radix/trie for identifier-like keys.
- Adaptive `afw_array` memory store (vector of value pointers) — separate from hash.
