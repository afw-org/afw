#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: alias_env.as
//? customPurpose: Part of lmdb tests
//? description: ...
lmdbA2 is a second adapter id on lmdbA's directory, spelled without
the trailing slash. Both ids resolve to one path, so they share one
MDB_env (#387), but each id has its own session and its own
transaction. Nothing here writes through both ids in one process:
that is refused (alias_write_write.as).
//? sourceType: script
//?
//? test: alias_sees_committed_rows
//? description: lmdbA2 reads the rows config.py committed through lmdbA.
//? skip: false
//? expect: 0
//? source: ...

assert(length(retrieve_objects('lmdbA2', 'OrderRow')) === 2,
    "lmdbA2 should see s1 and s2");
assert(get_object('lmdbA2', 'OrderRow', 's1').item === 's1',
    "lmdbA2 get of s1");

return 0;

//?
//? test: alias_write_during_retrieve
//? description: OrderAliasWrite.sideWritten writes through lmdbA2 while the model's read-only retrieve of lmdbA is open.
//? skip: false
//? expect: 0
//? source: ...

const rows: array = retrieve_objects('model', 'OrderAliasWrite');
assert(length(rows) === 2, "expected 2 rows, got " + string(length(rows)));
for (const row: object of rows) {
    assert(row.sideWritten === true, row.item + " sideWritten");
}
assert(length(retrieve_objects('lmdbA2', 'Side')) === 2,
    "lmdbA2 should see its own 2 Side writes");

return 0;

//?
//? test: alias_write_not_visible_to_other_id
//? description: ...
lmdbA does not see what lmdbA2 wrote, although it is the same
database: lmdbA2's write transaction is not committed until the
process ends, and lmdbA reads in its own transaction. Current
behavior; sharing one transaction per environment would change it.
//? skip: false
//? expect: 0
//? source: ...

assert(length(retrieve_objects('lmdbA', 'Side')) === 0,
    "lmdbA sees lmdbA2's uncommitted Side writes");

return 0;
