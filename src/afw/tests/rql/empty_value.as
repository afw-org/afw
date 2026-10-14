#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: empty_value.as
//? customPurpose: Part of rql tests
//? description: A relation on the empty string, as the query string writer writes it, reads back (#543)
//? sourceType: script
//?
//? test: fiql-empty-value
//? description: p= is eq on ""
//? expect: ({"filter": {"op": "eq", "property": "p", "value": ""}})
//? source: ...

convert_query_string_to_AdaptiveQueryCriteria("p=")

//? test: fiql-empty-value-operation
//? description: p=gt= is gt on ""
//? expect: ({"filter": {"op": "gt", "property": "p", "value": ""}})
//? source: ...

convert_query_string_to_AdaptiveQueryCriteria("p=gt=")

//? test: rql-empty-value
//? description: eq(p,) is eq on ""
//? expect: ({"filter": {"op": "eq", "property": "p", "value": ""}})
//? source: ...

convert_query_string_to_AdaptiveQueryCriteria("eq(p,)")

//? test: round-trip
//? description: empty values next to each separator round-trip
//? expect: 0
//? source: ...

for (const q of ["p=&q=x", "(p=|q=)", "(p=gt=|q=)&r=x", "p!=", "p!=&q=x"]) {
    const out = convert_AdaptiveQueryCriteria_to_query_string(
        convert_query_string_to_AdaptiveQueryCriteria(q));
    assert(out === q, q + " wrote " + out);
}
return 0;

//? test: retrieve-empty-value
//? description: a retrieve with an empty value runs (it was a syntax error)
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "urlEncodedRQLString": "objectType="
}, undefined, undefined, 0);
assert(length(objects) === 0);
return 0;
