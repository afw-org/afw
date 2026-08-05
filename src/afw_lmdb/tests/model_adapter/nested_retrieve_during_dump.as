#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: nested_retrieve_during_dump.as
//? customPurpose: Part of lmdb tests
//? description: Regression test for MDB_BAD_RSLOT from a nested retrieve_objects() during a dump.
//? sourceType: script
//?
//? test: nested_retrieve_during_dump
//? description: A model onGetProperty triggers retrieve_objects() on the same lmdb adapter session while an outer dump is still open.
//? skip: false
//? expect: 0
//? source: ...

// Two TestObjectType1 rows are seeded by config.py in a separate afw
// process, so this process's lmdb adapter session has never begun a
// transaction of its own when the retrieve below starts. For each row,
// the model layer evaluates NestedCount's onGetProperty while the outer
// dump's read transaction is still open; that property issues a second
// retrieve_objects() against the same lmdb adapter session, which used
// to abort with MDB_BAD_RSLOT (-30783) before the transaction reuse fix
// in afw_lmdb_internal.h.
const objects: array = retrieve_objects('model', 'MyObjectType1');

assert(length(objects) == 2, "expected 2 objects, got " + string(length(objects)));

for (const obj: object of objects) {
    assert(obj.NestedCount == 2, "NestedCount was " + string(obj.NestedCount));
}

return 0;
