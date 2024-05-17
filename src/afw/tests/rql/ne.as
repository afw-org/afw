#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: ne.as
//? customPurpose: Part of rql tests
//? description: Test rql "ne" operator
//? sourceType: script
//?
//? test: ne_string
//? description: Test "ne" with string
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "ne",
        "property": "objectType",
        "value": "_AdaptiveObjectType_"
    }
});

// we should have more than one object
assert(length(objects) > 1);

for (const obj of objects) {
    assert(obj.objectType !== "_AdaptiveObjectType_");
}

return 0;


//? test: ne_integer
//? description: Test "ne" with integer
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveFunction_", {
    "filter": {
        "op": "ne",
        "property": "numberOfRequiredParameters",
        "value": 0
    }
});

// we should have at least one object to start
assert(length(objects) > 0);

for (const obj of objects) {
    assert(obj.numberOfRequiredParameters !== 0);
}

return 0;


//? test: ne_boolean_true
//? description: Test "ne" with boolean true
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "ne",
        "property": "allowEntity",
        "value": true
    }
});

// we should have at least one object
assert(length(objects) > 0);


for (const obj of objects) {
    assert(obj.allowEntity !== true);
}

return 0;


//? test: ne_boolean_false
//? description: Test "ne" with boolean true
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "ne",
        "property": "allowEntity",
        "value": false
    }
});

// we should have at least one object
assert(length(objects) > 0);


for (const obj of objects) {
    assert(obj.allowEntity !== false);
}

return 0;