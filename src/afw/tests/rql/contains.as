#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: contains.as
//? customPurpose: Part of rql tests
//? description: Test rql "contains" operator
//? sourceType: script
//?
//? test: contains_array
//? description: Test rql operator "contains"
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "contains",
        "property": "collectionURIs",
        "value": "/afw/_AdaptiveCollection_/core"
    }
});

// we should have more than one object
assert(length(objects) > 1);

for (const obj of objects) {
    // make sure the mapped property exists
    assert(obj.collectionURIs[0] === "/afw/_AdaptiveCollection_/core");
}

return 0;


//? test: contains_array_no_match
//? description: Test rql operator "contains" with no matches
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "contains",
        "property": "collectionURIs",
        "value": "/x/y/z"
    }
});

// we should have more than one object
assert(length(objects) === 0);

return 0;