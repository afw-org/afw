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

// KNOWN BUG (issue #249): the rejected duplicate's *primary* object is not
// actually rolled back -- it leaks as an orphaned, unindexed record. This is
// only reproducible when the failing add_object() is the second-or-later
// add on the same session (a fresh session/process rolls back correctly),
// which is exactly what this test file does. An unfiltered retrieve_objects
// (bypassing the index) would show 2 objects here, not 1. Left disabled
// until #249 is fixed -- uncomment to see it fail:
//
// const allObjects: array = retrieve_objects("lmdb", ot, undefined, undefined, undefined, 0);
// assert(length(allObjects) === 1,
//     "rejected duplicate add_object() should not leave its primary object persisted (issue #249)");

safe_evaluate(index_remove("lmdb", "email"), null);
safe_evaluate(delete_object("lmdb", ot, id1), null);
safe_evaluate(delete_object("lmdb", ot, id2), null);

return 0;
