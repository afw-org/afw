#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: alias_read_during_retrieve.as
//? customPurpose: Part of lmdb tests
//? description: Read through lmdbA2 while a read-only retrieve of lmdbA, the same MDB_env, is open.
//? sourceType: script
//?
//? test: alias_read_during_retrieve
//? description: ...
FIXME: pins a bug (beta-backlog.md, LMDB shared env). The model's
retrieve holds a read transaction on lmdbA's MDB_env. lmdbA2 is
another session on that MDB_env, so the reentrant reuse in
AFW_LMDB_BEGIN_TRANSACTION (per session) does not apply, and
mdb_txn_begin() fails on this thread's reader slot with
MDB_BAD_RSLOT. aliasCount gets the error instead of 2. When fixed,
assert aliasCount === 2 for both rows instead.
//? skip: false
//? expect: 0
//? source: ...

const rows: array = retrieve_objects('model', 'OrderAliasRead');
assert(length(rows) === 2, "expected 2 rows, got " + string(length(rows)));

for (const row: object of rows) {
    assert(row.aliasCount === undefined,
        "aliasCount worked (" + string(row.aliasCount) + "): bug fixed? see FIXME");
    const errors: array = meta(row).propertyTypes.aliasCount.errors;
    assert(includes<string>(errors[0], 'Unable to begin transaction'),
        "unexpected error: " + string(errors));
}

return 0;
