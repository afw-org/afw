# afw_hash_table — C key → pointer map

**Audience:** maintainers. **Not** handbook.  
Landed on **`develop`** ([PR #301](https://github.com/afw-org/afw/pull/301)). Same campaign as [`afw-vector.md`](afw-vector.md).

## Why

Name→pointer tables allocated from an AFW pool.

## Settled

- **Not a sorted vector / binary search.** Env function registry and compile intern **keep inserting**. Insert-into-sorted-array is the wrong cost.
- **In-memory btree-ish** is a later option for “load then find,” especially short names. Not required for a hash table.
- **Hash is the drop-in.** Keys are `key` + `klen` → pointer. Get/set/iterate, grow as you go. Hash is `hash * 33 + byte`, then `memcmp`.
- Name it **`afw_hash_table`**, parallel to **`afw_vector`**: AFW pool, typed overlay (`AFW_HASH_TABLE_STRUCT` / `afw_void_hash_table_t`), `const … internal` like an interface instance, no `get_reference`.
- Keys are **not copied**. Caller keeps key memory alive. Set value **NULL** deletes.
- Iterator is a **stack** `afw_hash_table_index_t` (no pool for `first()`). Prefetches next so deleting the current entry is safe.
- Intern-then-id is a later lever, not a prerequisite.

## Converted

Env registries, compile intern / script type names, runtime catalog, adapter session and object-type caches, model object types, object-view intern, associative_array wrappers, service-id set, YAML anchors, LDAP schema name tables, LMDB `dbi_handles` (keys cloned into adapter `p` — same as `apr_hash` store-as-is; clone is the on-the-fly name lifetime, not a hash-API difference).

## Later, not first

- Ordered tree if something needs range listing, not “find this name.”
- Radix/trie for identifier-like keys.
- Adaptive `afw_array` memory store (vector of value pointers) — separate from hash.
