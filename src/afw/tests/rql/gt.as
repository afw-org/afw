#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: gt.as
//? customPurpose: Part of rql tests
//? description: Test rql "gt" operator
//? sourceType: script
//?
//? test: gt_string
//? description: Test rql operator "gt" against a string
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "gt",
        "property": "objectType",
        "value": "_AdaptiveA"
    }
});

// we should have more than one object
assert(length(objects) > 0);

// verify they all match
for (const obj of objects) {
    assert(obj.objectType > "_AdaptiveA");
}

return 0;


//? test: gt_string_no_match
//? description: Test rql operator "gt" against a string without a match
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "gt",
        "property": "objectType",
        "value": "_AdaptiveZZ"
    }
});

// we should have more than one object
assert(length(objects) === 0);

return 0;


//? test: gt_integer
//? description: Test rql operator "gt" against an integer
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveFunction_", {
    "filter": {
        "op": "gt",
        "property": "numberOfRequiredParameters",
        "value": 0
    }
});

// we should have more than one object
assert(length(objects) > 0);

for (const obj of objects) {
    assert(obj.numberOfRequiredParameters > 0);
}

return 0;