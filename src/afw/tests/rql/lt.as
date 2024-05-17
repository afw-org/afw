#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: lt.as
//? customPurpose: Part of rql tests
//? description: Test rql "lt" operator
//? sourceType: script
//?
//? test: lt_string
//? description: Test rql operator "lt" against a string
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "lt",
        "property": "objectType",
        "value": "_AdaptiveZ"
    }
});

// we should have more than one object
assert(length(objects) > 0);

// verify they all match
for (const obj of objects) {
    assert(obj.objectType < "_AdaptiveZ");
}

return 0;


//? test: lt_string_no_match
//? description: Test rql operator "lt" against a string that doesn't match
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "lt",
        "property": "objectType",
        "value": "_AAdaptive"
    }
});

// we should have no matches
assert(length(objects) == 0);

return 0;


//? test: lt_integer
//? description: Test rql operator "lt" against an integer
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveFunction_", {
    "filter": {
        "op": "lt",
        "property": "numberOfRequiredParameters",
        "value": 100
    }
});

// we should have more than one object
assert(length(objects) > 0);

// verify they all match
for (const obj of objects) {
    assert(obj.numberOfRequiredParameters < 100);
}

return 0;


//? test: lt_integer_no_match
//? description: Test rql operator "lt" against an integer that doesn't match
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveFunction_", {
    "filter": {
        "op": "lt",
        "property": "numberOfRequiredParameters",
        "value": 0
    }
});

// we should have more than one object
assert(length(objects) === 0);

return 0;