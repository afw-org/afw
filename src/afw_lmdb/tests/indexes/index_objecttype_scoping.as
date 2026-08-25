#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: index_objecttype_scoping.as
//? customPurpose: Part of lmdb tests
//? description: An index definition's objectType list scopes both retroactive build and query use (issue #57).
//? sourceType: script
//?
//? test: index_objecttype_scoping
//? description: index_create's objectType restricts retroactive indexing to matching object types only.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const otA: string = "TestIndexScopeTypeA";
const otB: string = "TestIndexScopeTypeB";

const idA1: string = generate_uuid();
const idA2: string = generate_uuid();
const idB1: string = generate_uuid();

// Seed both object types with an object sharing the same property value
// before the index is created, so the retroactive scan has to choose.
add_object("lmdb", otA, { surname: "Smith" }, idA1);
add_object("lmdb", otA, { surname: "Jones" }, idA2);
add_object("lmdb", otB, { surname: "Smith" }, idB1);

// Scope the index to otA only.
const created: object = index_create("lmdb", "surname", undefined, [otA], undefined, undefined, true, false);
assert(created.num_indexed === 2,
    "retroactive index_create scoped to otA should only index otA's 2 objects, not otB's");
assert(created.num_processed === 2,
    "retroactive scan scoped to otA should only fetch otA's 2 objects -- num_processed === 3 would mean otB was scanned too (issue #252 item 2)");

// A query against the scoped type still finds its match.
const foundA: array = retrieve_objects("lmdb", otA,
    { "filter": { "op": "eq", "property": "surname", "value": "Smith" } });
assert(length(foundA) === 1, "otA object should still be found by the scoped index");

// A query against the unscoped type is unaffected by the otA-only index
// (it falls back to a normal scan, but must still return correct results).
const foundB: array = retrieve_objects("lmdb", otB,
    { "filter": { "op": "eq", "property": "surname", "value": "Smith" } });
assert(length(foundB) === 1, "otB object should still be found even though it's outside the index's objectType scope");

safe_evaluate(index_remove("lmdb", "surname"), null);
safe_evaluate(delete_object("lmdb", otA, idA1), null);
safe_evaluate(delete_object("lmdb", otA, idA2), null);
safe_evaluate(delete_object("lmdb", otB, idB1), null);

return 0;
