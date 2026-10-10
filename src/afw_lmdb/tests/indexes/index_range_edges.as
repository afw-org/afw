#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: index_range_edges.as
//? customPurpose: Part of lmdb tests
//? description: Index cursors at the ends of the index, on duplicate keys, on keys that are prefixes of others, and planner shapes with terms an index can't answer.
//? sourceType: script
//?
//? test: index_range_edges_integer
//? description: lt/le/gt/ge past either end of the index find nothing; le takes every duplicate of its key.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const ot: string = "TestIndexRangeEdgesInt";
index_create("lmdb", "n_edge", undefined, [ot], undefined, undefined, false, false);
for (const n of [-3, 0, 1, 1, 1, 2, 3]) {
    add_object("lmdb", ot, { n_edge: n }, generate_uuid());
}

function count(op: string, v: integer): integer {
    return length(retrieve_objects("lmdb", ot,
        { filter: { op: op, property: "n_edge", value: v } }));
}

assert(count("lt", -3) === 0, "lt the first key (returned the first key)");
assert(count("le", -5) === 0, "le below every key");
assert(count("gt", 3) === 0, "gt the last key (returned the last key)");
assert(count("ge", 5) === 0, "ge above every key");
assert(count("le", 1) === 5, "le 1 takes all three 1s (took one)");
assert(count("lt", 1) === 2, "lt 1");
assert(count("ge", 1) === 5, "ge 1");
assert(count("gt", 1) === 2, "gt 1");
assert(count("le", 5) === 7, "le above every key");
assert(count("lt", 5) === 7, "lt above every key");
assert(count("eq", 1) === 3, "eq 1");
return 0;

//? test: index_range_edges_string_prefix
//? description: gt/le compare whole keys: a key that only starts with the value is greater ("ab" > "a").
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const ot: string = "TestIndexRangeEdgesStr";
index_create("lmdb", "s_edge", undefined, [ot], undefined, undefined, false, false);
for (const s of ["ab", "abc", "b"]) {
    add_object("lmdb", ot, { s_edge: s }, generate_uuid());
}

function count(op: string, v: string): integer {
    return length(retrieve_objects("lmdb", ot,
        { filter: { op: op, property: "s_edge", value: v } }));
}

assert(count("gt", "a") === 3, "gt a, a not a key (skipped ab)");
assert(count("le", "a") === 0, "le a (took ab as equal)");
assert(count("le", "ab") === 1, "le ab");
assert(count("gt", "ab") === 2, "gt ab");
return 0;

//? test: index_planner_terms_without_cursor
//? description: an or with a term no index answers, inside an and, is tested on the and's other cursors (it crashed); ne/in on an indexed property get no cursor (they threw "Unable to create cursor for this operator").
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const ot: string = "TestIndexPlannerNoCursor";
index_create("lmdb", "n_pl", undefined, [ot], undefined, undefined, false, false);
add_object("lmdb", ot, { n_pl: -1, b_pl: false }, generate_uuid());
add_object("lmdb", ot, { n_pl: -1, b_pl: true }, generate_uuid());
add_object("lmdb", ot, { n_pl: 0, b_pl: false }, generate_uuid());

const crashed = retrieve_objects("lmdb", ot, { filter: { op: "and", filters: [
    { op: "eq", property: "b_pl", value: false },
    { op: "eq", property: "n_pl", value: -1 },
    { op: "or", filters: [
        { op: "eq", property: "b_pl", value: false },
        { op: "eq", property: "n_pl", value: 0 }
    ] }
] } });
assert(length(crashed) === 1, "and(b, n, or(b, n))");

const ne = retrieve_objects("lmdb", ot, { filter: { op: "and", filters: [
    { op: "ne", property: "n_pl", value: 0 },
    { op: "eq", property: "n_pl", value: -1 },
    { op: "in", property: "n_pl", value: [-1, 0] }
] } });
assert(length(ne) === 2, "and(ne, eq, in) on one indexed property");
return 0;

//? test: index_query_string_value_typed
//? description: a query string's values are strings; on a property whose object type says integer, an indexed range or eq seeks the integer (n=gt=9 found nothing, n=lt=10 found everything).
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const ot: string = "TestIndexQueryStringTyped";
add_object("lmdb", "_AdaptiveObjectType_", {
    propertyTypes: { n_qs: { dataType: "integer", allowQuery: true } }
}, ot);
index_create("lmdb", "n_qs", undefined, [ot], undefined, undefined, false, false);
for (const n of [-3, 2, 9, 10, 100]) {
    add_object("lmdb", ot, { n_qs: n }, generate_uuid());
}

function count(qs: string): integer {
    return length(retrieve_objects("lmdb", ot, { urlEncodedRQLString: qs }));
}

assert(count("n_qs=gt=9") === 2, "n_qs=gt=9");
assert(count("n_qs=lt=10") === 3, "n_qs=lt=10");
assert(count("n_qs=-3") === 1, "n_qs=-3");
assert(count("gt(n_qs,9)") === 2, "gt(n_qs,9)");
assert(count("n_qs=ge=x") === 0, "a value that is not an integer");
assert(length(retrieve_objects("lmdb", ot, { filter:
    { op: "le", property: "n_qs", value: "9" } })) === 3, "filter object, string value");
return 0;
