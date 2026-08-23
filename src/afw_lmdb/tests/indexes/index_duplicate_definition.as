#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: index_duplicate_definition.as
//? customPurpose: Part of lmdb tests
//? description: index_create's response when a key is already defined (issue #57).
//? sourceType: script
//?
//? test: index_duplicate_definition
//? description: Creating an index with a key that already exists returns a message in the result object rather than throwing.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const ot: string = "TestIndexDupType";

const first: object = index_create("lmdb", "dupKey", undefined, [ot], undefined, undefined, false, false);
assert(!is_defined(first.message), "first index_create with a new key should not return a message");

// Re-creating with the same key should not throw -- it should return a
// result object carrying a message instead.
const second: object = index_create("lmdb", "dupKey", undefined, [ot], undefined, undefined, false, false);
assert(is_defined(second.message), "re-creating an existing index key should return a message, not throw");

// The original definition must still be intact and listed.
const listed: object = index_list("lmdb");
assert(is_defined(listed.dupKey), "the original index definition should remain after a rejected duplicate create");

safe_evaluate(index_remove("lmdb", "dupKey"), null);

return 0;
