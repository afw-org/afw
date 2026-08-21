# test262 leftover `FIXME:`

The convert / reclassify sweep on this branch is done. Skip prefixes are
only **`FIXME:`** and **`Never:`** (see [`README.md`](README.md)).
What already changed is in [`changes.md`](changes.md).

**`Never:`** is decided-not. Do not burn it down unless the product flips.

## Remaining `FIXME:` (2)

| Test | Why it stays |
|------|----------------|
| `language/script/string_code_points.as` `Deferred-produce-type-script-call-return` | Runtime `f()` would false-green. Needs compile-time produce-type on script-call IR. |
| `lmdb/adapter/index_current.as` `index_current_object` | LMDB `index_create` hang. **#57**. |

When one of those lands, unskip (or rewrite) the case and drop the row.
Do not use this file as a convert shortlist anymore.
