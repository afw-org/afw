#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: in.as
//? customPurpose: Part of rql tests
//? description: Test rql "in" operator
//? sourceType: script
//?
//? test: in_array_required
//? description: Test "in" without using array
//? expect: error:array required for 'in' operator
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "in",
        "property": "objectType",
        "value": "xyz"
    }
});

//? test: in_data_type_mismatch
//? description: Test "in" when nothing matches
//? expect: error:data type mismatch
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "in",
        "property": "objectType",
        "value": [ true ]
    }
});


//? test: in_none
//? description: Test "in" when nothing matches
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "in",
        "property": "objectType",
        "value": ["xyz"]
    }
});

// we should have no objects that match
assert(length(objects) === 0);

return 0;


//? test: in_match_one
//? description: Test "in" when matches one
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "in",
        "property": "objectType",
        "value": ["_AdaptiveAdaptor_"]
    }
});

// we should have one object that match
assert(length(objects) === 1);

return 0;

//? test: in_match_multi
//? description: Test "in" when matches more than one
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveDataType_", {
    "filter": {
        "op": "in",
        "property": "jsonPrimitive",
        "value": ["string"]
    }
});

// we should have more than one object that matches
assert(length(objects) > 1);

return 0;


//? test: in_multi_match_one
//? description: Test "in" for multiple values when matches one
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "in",
        "property": "objectType",
        "value": ["abc","_AdaptiveAdaptor_","xyz"]
    }
});

// we should have one object that match
assert(length(objects) === 1);

return 0;


//? test: in_multi_match_one
//? description: Test "in" for multiple values when matches two
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "in",
        "property": "objectType",
        "value": ["_AdaptiveAdaptorType_","_AdaptiveAdaptor_","xyz"]
    }
});

// we should have one object that match
assert(length(objects) === 2);

return 0;


//? test: in_list_matches_multi
//? description: Test "in" against list, matching multiple objects
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "in",
        "property": "collectionURIs",
        "value": ["/afw/_AdaptiveCollection_/core"]
    }
});

// we should have one object that match
assert(length(objects) > 1);

return 0;