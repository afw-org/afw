#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: index_integer_option_noop.as
//? customPurpose: Part of lmdb tests
//? description: The (removed) "integer" index option is a harmless no-op -- range queries on an integer-property index still return numerically correct results via the sortable-text key encoding, not a mismatched MDB_INTEGERKEY database (issue #266).
//? sourceType: script
//?
//? test: index_integer_option_noop_range
//? description: Passing options: ["integer"] no longer sets MDB_INTEGERKEY against a sortable-text key; lt/le/gt/ge range queries remain numerically correct.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const ot: string = "TestIndexIntegerOptionNoopType";

// Same ordering-trap values as index_range_numeric.as (issue #251): a
// lexicographic ("100" < "20" < "9") comparison, or a raw MDB_INTEGERKEY
// mismatch against a text key, would get these wrong.
const idNeg: string = generate_uuid();
const idZero: string = generate_uuid();
const id9: string = generate_uuid();
const id10: string = generate_uuid();
const id100: string = generate_uuid();

index_create("lmdb", "age", undefined, [ot], undefined, ["integer"], false, false);

add_object("lmdb", ot, { age: -50 }, idNeg);
add_object("lmdb", ot, { age: 0 }, idZero);
add_object("lmdb", ot, { age: 9 }, id9);
add_object("lmdb", ot, { age: 10 }, id10);
add_object("lmdb", ot, { age: 100 }, id100);

const gt9: array = retrieve_objects("lmdb", ot,
    { "filter": { "op": "gt", "property": "age", "value": 9 } });
assert(length(gt9) === 2, "gt 9 should return the 2 objects with age > 9 (10, 100)");

const ge9: array = retrieve_objects("lmdb", ot,
    { "filter": { "op": "ge", "property": "age", "value": 9 } });
assert(length(ge9) === 3, "ge 9 should include the boundary (9, 10, 100)");

const lt0: array = retrieve_objects("lmdb", ot,
    { "filter": { "op": "lt", "property": "age", "value": 0 } });
assert(length(lt0) === 1, "lt 0 should return only the negative object (-50)");
assert(lt0[0].age === -50, "lt 0 should return the object with age -50");

const le0: array = retrieve_objects("lmdb", ot,
    { "filter": { "op": "le", "property": "age", "value": 0 } });
assert(length(le0) === 2, "le 0 should include the boundary (-50, 0)");

safe_evaluate(index_remove("lmdb", "age"), null);
safe_evaluate(delete_object("lmdb", ot, idNeg), null);
safe_evaluate(delete_object("lmdb", ot, idZero), null);
safe_evaluate(delete_object("lmdb", ot, id9), null);
safe_evaluate(delete_object("lmdb", ot, id10), null);
safe_evaluate(delete_object("lmdb", ot, id100), null);

return 0;
