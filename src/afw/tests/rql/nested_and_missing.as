#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: nested_and_missing.as
//? customPurpose: Part of rql tests
//? description: Nested and/or, missing properties, out, and boolean values as strings
//? sourceType: script
//?
//? test: nested-and-object
//? description: and(and(a, b), c) tests b (it was and(a, c))
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "and",
        "filters": [
            {
                "op": "and",
                "filters": [
                    { "op": "eq", "property": "allowEntity", "value": true },
                    { "op": "eq", "property": "objectType",
                        "value": "_AdaptiveObjectType_" }
                ]
            },
            { "op": "ne", "property": "objectType", "value": "x" }
        ]
    }
}, undefined, undefined, 0);
assert(length(objects) === 1);
assert(objects[0].objectType === "_AdaptiveObjectType_");
return 0;

//? test: nested-and-rql
//? description: the same in RQL function syntax
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "urlEncodedRQLString":
        "and(and(eq(allowEntity,true),eq(objectType,_AdaptiveObjectType_)),ne(objectType,x))"
}, undefined, undefined, 0);
assert(length(objects) === 1);
assert(objects[0].objectType === "_AdaptiveObjectType_");
return 0;

//? test: nested-or-rql
//? description: or(or(a, b), c) tests b
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "urlEncodedRQLString":
        "or(or(eq(objectType,x),eq(objectType,_AdaptiveObjectType_)),eq(objectType,y))"
}, undefined, undefined, 0);
assert(length(objects) === 1);
assert(objects[0].objectType === "_AdaptiveObjectType_");
return 0;

//? test: missing-property-goes-on
//? description: a relation on a missing property is false; or goes on to the next filter
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "or",
        "filters": [
            { "op": "eq", "property": "descriptionPropertyName",
                "value": "x" },
            { "op": "eq", "property": "objectType",
                "value": "_AdaptiveAdapter_" }
        ]
    }
}, undefined, undefined, 0);
assert(get_object("afw", "_AdaptiveObjectType_", "_AdaptiveAdapter_")
    .descriptionPropertyName === undefined);
assert(length(objects) === 1);
assert(objects[0].objectType === "_AdaptiveAdapter_");
return 0;

//? test: out
//? description: out is not in (it threw)
//? expect: 0
//? source: ...

const all = retrieve_objects("afw", "_AdaptiveObjectType_",
    undefined, undefined, undefined, 0);
const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": { "op": "out", "property": "objectType",
        "value": ["_AdaptiveObjectType_", "x"] }
}, undefined, undefined, 0);
assert(length(objects) === length(all) - 1);
for (const o of objects) {
    assert(o.objectType !== "_AdaptiveObjectType_");
}
return 0;

//? test: boolean-value-as-string
//? description: allowEntity=false means false (a string compared by truthiness was true)
//? expect: 0
//? source: ...

let want = 0;
for (const o of retrieve_objects("afw", "_AdaptiveObjectType_",
    undefined, undefined, undefined, 0))
{
    if (o.allowEntity === false) {
        want = want + 1;
    }
}
assert(want > 0);

const eq = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "urlEncodedRQLString": "allowEntity=false"
}, undefined, undefined, 0);
assert(length(eq) === want);
for (const o of eq) {
    assert(o.allowEntity === false);
}

const inList = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": { "op": "in", "property": "allowEntity", "value": ["false"] }
}, undefined, undefined, 0);
assert(length(inList) === want);
return 0;
