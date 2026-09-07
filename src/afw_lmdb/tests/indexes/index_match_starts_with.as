#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: index_match_starts_with.as
//? customPurpose: Part of lmdb tests
//? description: Index-accelerated "starts with" queries via the match operator's literal-prefix shape (`<literal>.*`).
//? sourceType: script
//?
//? test: index_match_starts_with_correctness
//? description: An indexed property's literal-prefix match query returns exactly the objects whose value starts with the literal.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const ot: string = "TestIndexMatchStartsWithType";

const idApple: string = generate_uuid();
const idApplication: string = generate_uuid();
const idApply: string = generate_uuid();
const idApp: string = generate_uuid();
const idBanana: string = generate_uuid();

index_create("lmdb", "name", undefined, [ot], undefined, undefined, false, false);

add_object("lmdb", ot, { name: "apple" }, idApple);
add_object("lmdb", ot, { name: "application" }, idApplication);
add_object("lmdb", ot, { name: "apply" }, idApply);
add_object("lmdb", ot, { name: "app" }, idApp);
add_object("lmdb", ot, { name: "banana" }, idBanana);

const appl: array = retrieve_objects("lmdb", ot,
    { "filter": { "op": "match", "property": "name", "value": "appl.*" } });
assert(length(appl) === 3, "appl.* should match apple, application, apply");

const app: array = retrieve_objects("lmdb", ot,
    { "filter": { "op": "match", "property": "name", "value": "app.*" } });
assert(length(app) === 4, "app.* should match apple, application, apply, app (the literal itself)");

const none: array = retrieve_objects("lmdb", ot,
    { "filter": { "op": "match", "property": "name", "value": "xyz.*" } });
assert(length(none) === 0, "xyz.* should match nothing");

// Combine with another indexed clause to prove the index-accelerated
// conjunction path (afw_adapter_impl_index_query's cursor merge/dedup),
// not just the single-cursor case above.
index_create("lmdb", "kind", undefined, [ot], undefined, undefined, false, false);
add_object("lmdb", ot, { name: "appstore", kind: "tool" }, generate_uuid());
add_object("lmdb", ot, { name: "appstore", kind: "toy" }, generate_uuid());

const appAndTool: array = retrieve_objects("lmdb", ot, { "filter": {
    "op": "and",
    "filters": [
        { "op": "match", "property": "name", "value": "app.*" },
        { "op": "eq", "property": "kind", "value": "tool" }
    ]
}});
assert(length(appAndTool) === 1, "app.* AND kind=tool should return exactly the one appstore/tool object");

safe_evaluate(index_remove("lmdb", "name"), null);
safe_evaluate(index_remove("lmdb", "kind"), null);

return 0;


//? test: index_match_unsupported_pattern_on_indexed_property_throws
//? description: A non-literal-prefix match pattern on an indexed property, combined with another sargable clause, still throws rather than silently mishandling the index (it is not translatable to a b-tree range scan).
//? expect: error:Unable to create cursor for this operator
//? source: ...
#!/usr/bin/env afw

const ot: string = "TestIndexMatchUnsupportedType";

index_create("lmdb", "status", undefined, [ot], undefined, undefined, false, false);
index_create("lmdb", "name", undefined, [ot], undefined, undefined, false, false);

add_object("lmdb", ot, { name: "apple", status: "active" }, generate_uuid());

// ".*apple" is not the "<literal>.*" shape (the wildcard isn't at the
// end), so it cannot be reduced to an index range scan. Anding it with a
// sargable clause on another indexed property makes the overall filter
// sargable, forcing the planner to attempt an index cursor for this leaf
// too -- which must throw, not silently full-scan or return wrong results.
const objects: array = retrieve_objects("lmdb", ot, { "filter": {
    "op": "and",
    "filters": [
        { "op": "eq", "property": "status", "value": "active" },
        { "op": "match", "property": "name", "value": ".*apple" }
    ]
}});

return 0;
