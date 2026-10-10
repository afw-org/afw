#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: query_string_lists.as
//? customPurpose: Part of rql tests
//? description: in / out lists in query strings, written and parsed
//? sourceType: script
//?
//? test: write-list
//? description: a list is written (a,b) (it was (a),b))
//? expect: "objectType=in=(a,b)&allowEntity=out=(x,-y)"
//? source: ...

convert_AdaptiveQueryCriteria_to_query_string({
    filter: {
        op: "and",
        filters: [
            { op: "in", property: "objectType", value: ["a", "b"] },
            { op: "out", property: "allowEntity", value: ["x", "-y"] }
        ]
    }
}, "afw", "_AdaptiveObjectType_")

//? test: list-item-starting-with-minus
//? description: a list item starting with - (-1, -x) is one value
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "urlEncodedRQLString": "objectType=in=(-x,_AdaptiveObjectType_,-1)"
}, undefined, undefined, 0);
assert(length(objects) === 1);
assert(objects[0].objectType === "_AdaptiveObjectType_");
return 0;

//? test: round-trip
//? description: each style's string retrieves what the object form does
//? expect: 0
//? source: ...

const criteria = {
    filter: {
        op: "or",
        filters: [
            { op: "in", property: "objectType",
                value: ["-x", "_AdaptiveObjectType_", "_AdaptiveAdapter_"] },
            { op: "and", filters: [
                { op: "eq", property: "allowEntity", value: "true" },
                { op: "out", property: "objectType",
                    value: ["_AdaptiveObjectType_", "y"] },
                { op: "eq", property: "objectType", value: "x" }
            ] }
        ]
    }
};
const want = length(retrieve_objects("afw", "_AdaptiveObjectType_",
    criteria, undefined, undefined, 0));
assert(want === 2);
for (const style of [0, 1, 2, 3]) {
    const qs = convert_AdaptiveQueryCriteria_to_query_string(criteria,
        "afw", "_AdaptiveObjectType_", style);
    assert(length(retrieve_objects("afw", "_AdaptiveObjectType_",
        { urlEncodedRQLString: qs }, undefined, undefined, 0)) === want,
        qs);
}
return 0;
