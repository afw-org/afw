#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: index_range_numeric.as
//? customPurpose: Part of lmdb tests
//? description: Index-accelerated lt/le/gt/ge range queries on integer/double properties return numerically, not lexicographically, correct results (issue #251).
//? sourceType: script
//?
//? test: index_range_numeric_integer
//? description: An indexed integer property's range queries (lt/le/gt/ge) return numerically correct results.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const ot: string = "TestIndexRangeIntType";

// Values chosen so that a naive lexicographic ("100" < "20" < "9") text
// comparison would get lt/le/gt/ge wrong -- this is the exact failure
// mode from issue #251 (an indexed `gt 9` query returned only `age: 9`).
const idNeg: string = generate_uuid();
const idZero: string = generate_uuid();
const id5: string = generate_uuid();
const id9: string = generate_uuid();
const id10: string = generate_uuid();
const id20: string = generate_uuid();
const id100: string = generate_uuid();

index_create("lmdb", "age", undefined, [ot], undefined, undefined, false, false);

add_object("lmdb", ot, { age: -50 }, idNeg);
add_object("lmdb", ot, { age: 0 }, idZero);
add_object("lmdb", ot, { age: 5 }, id5);
add_object("lmdb", ot, { age: 9 }, id9);
add_object("lmdb", ot, { age: 10 }, id10);
add_object("lmdb", ot, { age: 20 }, id20);
add_object("lmdb", ot, { age: 100 }, id100);

const gt9: array = retrieve_objects("lmdb", ot,
    { "filter": { "op": "gt", "property": "age", "value": 9 } });
assert(length(gt9) === 3, "gt 9 should return the 3 objects with age > 9 (10, 20, 100)");

// The exact issue #251 repro: age === 9 must never satisfy gt 9. Combining
// with an "eq" clause proves it directly, rather than just trusting a count.
const gt9EqualsNine: array = retrieve_objects("lmdb", ot, { "filter": {
    "op": "and",
    "filters": [
        { "op": "gt", "property": "age", "value": 9 },
        { "op": "eq", "property": "age", "value": 9 }
    ]
}});
assert(length(gt9EqualsNine) === 0,
    "age === 9 must not satisfy gt 9 (issue #251: lexicographic \"9\" > \"100\" made this pass)");

const ge9: array = retrieve_objects("lmdb", ot,
    { "filter": { "op": "ge", "property": "age", "value": 9 } });
assert(length(ge9) === 4, "ge 9 should include the boundary (9, 10, 20, 100)");

const lt9: array = retrieve_objects("lmdb", ot,
    { "filter": { "op": "lt", "property": "age", "value": 9 } });
assert(length(lt9) === 3, "lt 9 should return the 3 objects with age < 9 (-50, 0, 5)");

const le9: array = retrieve_objects("lmdb", ot,
    { "filter": { "op": "le", "property": "age", "value": 9 } });
assert(length(le9) === 4, "le 9 should include the boundary (-50, 0, 5, 9)");

// Negative values must sort below zero and below all positive values.
const ltZero: array = retrieve_objects("lmdb", ot,
    { "filter": { "op": "lt", "property": "age", "value": 0 } });
assert(length(ltZero) === 1, "lt 0 should return only the negative object (-50)");
assert(ltZero[0].age === -50, "lt 0 should return the object with age -50");

safe_evaluate(index_remove("lmdb", "age"), null);
safe_evaluate(delete_object("lmdb", ot, idNeg), null);
safe_evaluate(delete_object("lmdb", ot, idZero), null);
safe_evaluate(delete_object("lmdb", ot, id5), null);
safe_evaluate(delete_object("lmdb", ot, id9), null);
safe_evaluate(delete_object("lmdb", ot, id10), null);
safe_evaluate(delete_object("lmdb", ot, id20), null);
safe_evaluate(delete_object("lmdb", ot, id100), null);

return 0;

//? test: index_range_numeric_double
//? description: An indexed double property's range queries (lt/le/gt/ge) return numerically correct results, including negative and fractional values.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const ot: string = "TestIndexRangeDoubleType";

const idA: string = generate_uuid();
const idB: string = generate_uuid();
const idC: string = generate_uuid();
const idD: string = generate_uuid();
const idE: string = generate_uuid();

index_create("lmdb", "score", undefined, [ot], undefined, undefined, false, false);

add_object("lmdb", ot, { score: -3.5 }, idA);
add_object("lmdb", ot, { score: -1.25 }, idB);
add_object("lmdb", ot, { score: 0.0 }, idC);
add_object("lmdb", ot, { score: 2.75 }, idD);
add_object("lmdb", ot, { score: 10.5 }, idE);

const gtZero: array = retrieve_objects("lmdb", ot,
    { "filter": { "op": "gt", "property": "score", "value": 0.0 } });
assert(length(gtZero) === 2, "gt 0.0 should return the 2 positive scores (2.75, 10.5)");

const ltZero: array = retrieve_objects("lmdb", ot,
    { "filter": { "op": "lt", "property": "score", "value": 0.0 } });
assert(length(ltZero) === 2, "lt 0.0 should return the 2 negative scores (-3.5, -1.25)");

// -3.5 is more negative than -1.25, so it must sort first (lower), not by
// naive text/magnitude comparison. Checked by count (not by reading back
// the matched object's `score`): the LMDB adapter has a separate,
// pre-existing bug that corrupts a `double` property's value on readback
// (reproduces via plain get_object(), with no index involved -- unrelated
// to issue #251 and not touched by this fix, since the index-accelerated
// comparisons here run against the live in-memory value, never the
// corrupted decoded one). Worth its own issue, not fixed here.
const ltNeg1_25: array = retrieve_objects("lmdb", ot,
    { "filter": { "op": "lt", "property": "score", "value": -1.25 } });
assert(length(ltNeg1_25) === 1, "lt -1.25 should return only -3.5");

const leNeg1_25: array = retrieve_objects("lmdb", ot,
    { "filter": { "op": "le", "property": "score", "value": -1.25 } });
assert(length(leNeg1_25) === 2, "le -1.25 should include the boundary (-3.5, -1.25)");

const geNeg1_25: array = retrieve_objects("lmdb", ot,
    { "filter": { "op": "ge", "property": "score", "value": -1.25 } });
assert(length(geNeg1_25) === 4, "ge -1.25 should include the boundary (-1.25, 0.0, 2.75, 10.5)");

safe_evaluate(index_remove("lmdb", "score"), null);
safe_evaluate(delete_object("lmdb", ot, idA), null);
safe_evaluate(delete_object("lmdb", ot, idB), null);
safe_evaluate(delete_object("lmdb", ot, idC), null);
safe_evaluate(delete_object("lmdb", ot, idD), null);
safe_evaluate(delete_object("lmdb", ot, idE), null);

return 0;
