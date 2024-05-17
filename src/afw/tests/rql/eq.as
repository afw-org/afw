#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: eq.as
//? customPurpose: Part of rql tests
//? description: Test rql "eq" operator
//? sourceType: script
//?
//? test: eq_string_match
//? description: Test "eq" with string that matches
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "eq",
        "property": "objectType",
        "value": "_AdaptiveObjectType_"
    }
});

// we should have one object
assert(length(objects) === 1);

const obj = objects[0];

// make sure property is correct
assert(obj.objectType === "_AdaptiveObjectType_");

return 0;


//? test: eq_string_no_match
//? description: Test "eq" with string with no matches
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "eq",
        "property": "objectType",
        "value": "xyz"
    }
});

// we should have no objects
assert(length(objects) === 0);

return 0;


//? test: encoded_rql_eq_string
//? description: Test eq with string, using rql.
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "urlEncodedRQLString": "objectType=_AdaptiveObjectType_"
});

// we should have at least one object
assert(length(objects) > 0);

const obj = objects[0];

// make sure the mapped property exists
assert(obj.objectType === "_AdaptiveObjectType_");

return 0;


//? test: eq_integer
//? description: Test eq operator with integer
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveFunction_", {
    "filter": {
        "op": "eq",
        "property": "numberOfRequiredParameters",
        "value": 0
    }
});

// we should have at least one object
assert(length(objects) > 0);

for (const obj of objects) {
    // make sure the mapped property name is 0
    assert(obj.numberOfRequiredParameters === 0);
}

return 0;


//? test: eq_integer_no_match
//? description: Test eq operator with integer and no match
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveFunction_", {
    "filter": {
        "op": "eq",
        "property": "numberOfRequiredParameters",
        "value": 100
    }
});

// we should have no matching objects
assert(length(objects) === 0);

return 0;


//? test: eq_boolean_true
//? description: Test eq operator with boolean
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "eq",
        "property": "allowEntity",
        "value": true
    }
});

// we should have at least one object
assert(length(objects) > 0);

for (const obj of objects) {
    // make sure the mapped property is true
    assert(obj.allowEntity === true);
}

return 0;


//? test: eq_boolean_false
//? description: Test eq operator with boolean
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "eq",
        "property": "allowEntity",
        "value": false
    }
});

// we should have at least one object
assert(length(objects) > 1);

for (const obj of objects) {
    // make sure the mapped property is false
    assert(obj.allowEntity === false);
}

return 0;


//? test: eq_array
//? description: Test eq operator against array
//? expect: 0
//? skip: true
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "eq",
        "property": "collectionURIs",
        "value": "/afw/_AdaptiveCollection_/core"
    }
});

// we should have at least one object
assert(length(objects) > 0);

const obj = objects[0];

// fixme this will cause a segfault
// make sure the mapped property exists
assert(includes(obj.collectionURIs, "/afw/_AdaptiveCollection_/core"));

return 0;


//? test: eq_object_embedded
//? description: Test eq operator against object embedded property
//? expect: 0
//? skip: true
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "eq",
        "property": "propertyTypes.allowQuery",
        "value": true
    }
});

// we should have at least one object
assert(length(objects) > 0);


return 0;


//? test: eq_data_type_mismatch
//? description: Test operator "eq" with a data type mismatch
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "eq",
        "property": "objectType",
        "value": true
    }
});

// we should have one object to start with
assert(length(objects) === 0);

return 0;