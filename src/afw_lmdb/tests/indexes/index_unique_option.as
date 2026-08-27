#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: index_unique_option.as
//? customPurpose: Part of lmdb tests
//? description: The "unique" index option (issue #57).
//? sourceType: script
//?
//? test: index_unique_option
//? description: A unique index rejects a second object with a duplicate indexed value.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const ot: string = "TestIndexUniqueType";
const id1: string = generate_uuid();
const id2: string = generate_uuid();

index_create("lmdb", "email", undefined, [ot], undefined, ["unique"], false, false);

add_object("lmdb", ot, { email: "a@example.com" }, id1);

const dup: string = safe_evaluate(add_object("lmdb", ot, { email: "a@example.com" }, id2), "error");
assert(dup === "error", "adding a second object with a duplicate value on a unique index should fail");

// The rejected duplicate must not show up via the index -- this passes, but
// only proves the index itself never got the duplicate's entry, since this
// query is index-accelerated. It does NOT prove the duplicate's primary
// object wasn't persisted (see below).
const found: array = retrieve_objects("lmdb", ot,
    { "filter": { "op": "eq", "property": "email", "value": "a@example.com" } });
assert(length(found) === 1, "only the original object should be reachable via the indexed value after the duplicate add was rejected");

// Regression for issue #249: a rejected duplicate's *primary* object used
// to leak as an orphaned, unindexed record -- reproducible only when the
// failing add_object() was the second-or-later add on the same session,
// because the primary write and the index write shared the session's
// implicit per-request transaction with no way to roll back just this
// call. Fixed by giving add/modify/replace/delete_object their own nested
// LMDB transaction (AFW_LMDB_BEGIN_ATOMIC_TRANSACTION) so a failure here
// only discards this call's own writes.
const allObjects: array = retrieve_objects("lmdb", ot, undefined, undefined, undefined, 0);
assert(length(allObjects) === 1,
    "rejected duplicate add_object() should not leave its primary object persisted (issue #249)");

safe_evaluate(index_remove("lmdb", "email"), null);
safe_evaluate(delete_object("lmdb", ot, id1), null);
safe_evaluate(delete_object("lmdb", ot, id2), null);

return 0;
