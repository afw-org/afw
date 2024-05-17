#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: lte.as
//? customPurpose: Part of rql tests
//? description: Test rql "lte" operator
//? sourceType: script
//?
//? test: lte_string_eq
//? description: Test rql operator "lte" against a string where one is equal
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "lte",
        "property": "objectType",
        "value": "_AdaptiveObject_"
    }
});

// we should have more than one object
assert(length(objects) > 0);

// verify they all match
let found = false;
for (const obj of objects) {
    assert(obj.objectType <= "_AdaptiveObject_");

    if (obj.objectType === "_AdaptiveObject_")
        found = true;
}

assert(found);

return 0;


//? test: lte_string_lt
//? description: Test rql operator "lte" against a string where none are eq
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "lte",
        "property": "objectType",
        "value": "_AdaptiveObjectX_"
    }
});

// we should have more than one object
assert(length(objects) > 0);

// verify they all match
let found = false;
for (const obj of objects) {
    assert(obj.objectType <= "_AdaptiveObject_");

    if (obj.objectType === "_AdaptiveObject_")
        found = true;
}

assert(!found);

return 0;


//? test: lte_string_none
//? description: Test rql operator "lte" against a string where none match
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "lte",
        "property": "objectType",
        "value": "_AAdaptiveObject_"
    }
});

// we should have more than one object
assert(length(objects) === 0);

return 0;


//? test: lte_integer
//? description: Test rql operator "lte" against an integer
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveFunction_", {
    "filter": {
        "op": "lte",
        "property": "numberOfRequiredParameters",
        "value": 100
    }
});

// we should have more than one object
assert(length(objects) > 0);

// verify they all match
for (const obj of objects) {
    assert(obj.numberOfRequiredParameters <= 100);
}

return 0;


//? test: lte_integer_eq
//? description: Test rql operator "lte" against an integer with one matching eq
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveFunction_", {
    "filter": {
        "op": "lte",
        "property": "numberOfRequiredParameters",
        "value": 1
    }
});

// we should have more than one object
assert(length(objects) > 0);

let found = false;

// verify they all match
for (const obj of objects) {
    if (obj.numberOfRequiredParameters === 1)
        found = true;
    assert(obj.numberOfRequiredParameters <= 1);
}

assert(found);

return 0;


//? test: lte_integer_no_match
//? description: Test rql operator "lte" against an integer that doesn't match
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveFunction_", {
    "filter": {
        "op": "lte",
        "property": "numberOfRequiredParameters",
        "value": -1
    }
});

// we should have more than one object
assert(length(objects) === 0);

return 0;