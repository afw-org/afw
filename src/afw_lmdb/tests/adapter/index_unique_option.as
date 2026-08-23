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

// The rejected duplicate must not have been left half-indexed or persisted.
const found: array = retrieve_objects("lmdb", ot,
    { "filter": { "op": "eq", "property": "email", "value": "a@example.com" } });
assert(length(found) === 1, "only the original object should exist after the duplicate add was rejected");

safe_evaluate(index_remove("lmdb", "email"), null);
safe_evaluate(delete_object("lmdb", ot, id1), null);

return 0;
