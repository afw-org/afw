#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: gte.as
//? customPurpose: Part of rql tests
//? description: Test rql "gte" operator
//? sourceType: script
//?
//? test: gte_string
//? description: Test rql operator "gte" against a string
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "gte",
        "property": "objectType",
        "value": "_AdaptiveA"
    }
});

// we should have more than one object
assert(length(objects) > 0);

// verify they all match
for (const obj of objects) {
    assert(obj.objectType >= "_AdaptiveA");
}

return 0;


//? test: gte_string_eq
//? description: Test rql operator "gte" against a string that creates an eq match
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "gte",
        "property": "objectType",
        "value": "_AdaptiveAdaptor_"
    }
});

// we should have more than one object
assert(length(objects) > 0);

const found = false;

// verify they all match
for (const obj of objects) {
    if (obj.objectType === "_AdaptiveAdaptor_")
        found = true;

    assert(obj.objectType >= "_AdaptiveAdaptor_");
}

assert(found);

return 0;


//? test: gte_string_no_match
//? description: Test rql operator "gte" that contains no match
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "gte",
        "property": "objectType",
        "value": "_AdaptiveZZ"
    }
});

// we should have more than one object
assert(length(objects) === 0);

return 0;


//? test: gte_integer
//? description: Test rql operator "gte" against an integer
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveFunction_", {
    "filter": {
        "op": "gte",
        "property": "numberOfRequiredParameters",
        "value": 0
    }
});

// we should have more than one object
assert(length(objects) > 0);

let found = false;

for (const obj of objects) {
    if (obj.numberOfRequiredParameters === 0)
        found = true;
    assert(obj.numberOfRequiredParameters >= 0);
}

assert(found);

return 0;