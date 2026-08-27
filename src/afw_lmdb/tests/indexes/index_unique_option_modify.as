#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: index_unique_option_modify.as
//? customPurpose: Part of lmdb tests
//? description: The "unique" index option also protects modify_object (issue #249).
//? sourceType: script
//?
//? test: index_unique_option_modify
//? description: A unique-index violation during modify_object's reindex must not leave the primary object partially updated, and must leave the old index entry intact.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const ot: string = "TestIndexUniqueModifyType";
const id1: string = generate_uuid();
const id2: string = generate_uuid();

index_create("lmdb", "email", undefined, [ot], undefined, ["unique"], false, false);

add_object("lmdb", ot, { email: "a@example.com" }, id1);
add_object("lmdb", ot, { email: "b@example.com" }, id2);

// Regression for issue #249 (modify side): reindexing id2 to a value
// already used by id1 must fail atomically -- id2's primary object write
// and its reindex share one nested LMDB transaction, so a rejected unique
// value rolls back the whole modify, not just the failed index put.
const dup: string = safe_evaluate(
    modify_object("lmdb", ot, id2, [["set_property", "email", "a@example.com"]]),
    "error");
assert(dup === "error", "modifying an object to a duplicate value on a unique index should fail");

// id2's primary object must be unchanged -- not partially applied.
const unchanged: object = get_object("lmdb", ot, id2);
assert(unchanged.email === "b@example.com",
    "id2 should still have its original email after the rejected modify (issue #249)");

// No third object should have appeared, and both original objects remain.
const allObjects: array = retrieve_objects("lmdb", ot, undefined, undefined, undefined, 0);
assert(length(allObjects) === 2,
    "a rejected modify_object() should not change how many objects exist (issue #249)");

// id2's old indexed value must still be intact (reindex must not have
// removed the old entry before the new one was rejected).
const foundByOldValue: array = retrieve_objects("lmdb", ot,
    { "filter": { "op": "eq", "property": "email", "value": "b@example.com" } });
assert(length(foundByOldValue) === 1,
    "id2 should still be reachable by its original indexed value (issue #249)");

const foundByRejectedValue: array = retrieve_objects("lmdb", ot,
    { "filter": { "op": "eq", "property": "email", "value": "a@example.com" } });
assert(length(foundByRejectedValue) === 1,
    "only id1 should be reachable by the contested indexed value");

safe_evaluate(index_remove("lmdb", "email"), null);
safe_evaluate(delete_object("lmdb", ot, id1), null);
safe_evaluate(delete_object("lmdb", ot, id2), null);

return 0;
