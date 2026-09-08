# afw_hash_table — C map (apr_hash replacement)

**Audience:** maintainers. **Not** handbook.  
**Start:** discuss on a branch, same campaign as [`afw-vector.md`](afw-vector.md). No implementation in this pad.

## Why

Drop `apr_hash` so leftover `get_apr_pool()` is not required for name→pointer tables. The pain is **APR**, not the idea of a hash table.

## Settled in discussion (reduce-apr)

- **Not a sorted vector / binary search.** Env function registry and compile intern **keep inserting** (extensions/commands load; literals intern). Insert-into-sorted-array is the wrong cost.
- **In-memory btree-ish** (rotate on insert, log n compares, ordered walk) is a fair option for “load then find,” especially short names. Not required to replace `apr_hash`.
- **Hash is the drop-in.** Keys are almost always `key->s` + `key->len` → pointer. Get/set/iterate, grow as you go. Generating the hash walks **every key byte** (APR `hash * 33 + *p++`) then `memcmp`s — that cost is real, but one AFW table still beats two containers.
- Name it **`afw_hash_table`**, parallel to **`afw_vector`**: AFW pool, typed overlay if we want no cast at the call, `const … internal` like an interface instance, no `get_reference`.
- Intern-then-id is a later lever, not a prerequisite.

## Where `apr_hash_make` is today

| Area | Use |
|------|-----|
| Env **registry** (`type->ht`, `registry_names_ht`, data-type method numbers) | Register at load; **lookup on every call** |
| Compile **string intern** / `script_type_names` | Many insert+get per compile (`parser->apr_p`) |
| Runtime catalog (type → id → object) | Admin/retrieve |
| Adapter session / object-type cache | Per request |
| Model object types, LDAP schema names | Load + get |
| YAML anchors, object-view intern, `associative_array` | Local / generic wrapper |
| LMDB `dbi_handles` | Leave while **#299** |

## Later, not first

- Ordered tree if something needs range listing, not “find this name.”
- Radix/trie for identifier-like keys.
- Adaptive `afw_array` memory store (vector of value pointers) — separate from hash.
