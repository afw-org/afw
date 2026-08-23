#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: index_case_insensitive_option.as
//? customPurpose: Part of lmdb tests
//? description: The "case-insensitive-string" index option (issue #57).
//? sourceType: script
//?
//? test: index_case_insensitive_option
//? description: A case-insensitive index matches queries regardless of the stored or queried value's case.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const ot: string = "TestIndexCiType";
const id: string = generate_uuid();

index_create("lmdb", "surname", undefined, [ot], undefined, ["case-insensitive-string"], false, false);

add_object("lmdb", ot, { surname: "Smith" }, id);

const upper: array = retrieve_objects("lmdb", ot,
    { "filter": { "op": "eq", "property": "surname", "value": "SMITH" } });
assert(length(upper) === 1, "case-insensitive index should match an uppercase query value");

const lower: array = retrieve_objects("lmdb", ot,
    { "filter": { "op": "eq", "property": "surname", "value": "smith" } });
assert(length(lower) === 1, "case-insensitive index should match a lowercase query value");

const mixed: array = retrieve_objects("lmdb", ot,
    { "filter": { "op": "eq", "property": "surname", "value": "sMiTh" } });
assert(length(mixed) === 1, "case-insensitive index should match a mixed-case query value");

safe_evaluate(index_remove("lmdb", "surname"), null);
safe_evaluate(delete_object("lmdb", ot, id), null);

return 0;
