#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: index_create_test_option.as
//? customPurpose: Part of lmdb tests
//? description: index_create's test parameter checks a definition and counts what it would index without saving anything; index_create always indexes the objects already there (its retroactive parameter is gone).
//? sourceType: script
//?
//? test: index_create_test_saves_nothing
//? description: test true was ignored: the definition was saved and the objects indexed.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const ot: string = "TestIndexCreateTestOption";

add_object("lmdb", ot, { s_to: "a" }, generate_uuid());
add_object("lmdb", ot, { s_to: "b" }, generate_uuid());
add_object("lmdb", ot, { other: "c" }, generate_uuid());

const counted: object = index_create("lmdb", "s_to", undefined, [ot],
    undefined, undefined, true);
assert(counted.num_processed === 3, "num_processed: " + string(counted.num_processed));
assert(counted.num_indexed === 2, "num_indexed: " + string(counted.num_indexed));
assert(is_nullish(index_list("lmdb").s_to), "test saved no definition");

/* Nothing was saved, so the real create is not "already exists". */
const created: object = index_create("lmdb", "s_to", undefined, [ot],
    undefined, undefined, false);
assert(created.num_indexed === 2, "the real create indexes the objects already there");
assert(is_defined(index_list("lmdb").s_to), "the real create saved the definition");

const found: array = retrieve_objects("lmdb", ot,
    { filter: { op: "eq", property: "s_to", value: "a" } });
assert(length(found) === 1, "eq a through the index");

index_remove("lmdb", "s_to");

return 0;


//? test: index_create_eight_arguments
//? description: A call written for the old signature (retroactive before test) fails instead of reading retroactive as test.
//? expect: error
//? source: ...
#!/usr/bin/env afw

index_create("lmdb", "s_ea", undefined, ["TestIndexCreateEight"],
    undefined, undefined, true, false);

return 0;
