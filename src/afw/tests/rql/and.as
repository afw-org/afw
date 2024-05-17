#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: and.as
//? customPurpose: Part of rql tests
//? description: Test rql "and" operator
//? sourceType: script
//?
//? test: and_matches
//? description: Test rql "and" operator
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "and",
        "filters": [
            {
                "op": "eq",
                "property": "allowEntity",
                "value": true
            },
            {
                "op": "eq",
                "property": "descriptionPropertyName",
                "value": "description"
            }
        ]
    }
});

// should match one object
assert(length(objects) === 1);

for (const obj of objects) {
    assert(obj.allowEntity === true);
    assert(obj.descriptionPropertyName === "description");
}

return 0;


//? test: and_three_matches
//? description: Test rql "and" operator with three filters
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "and",
        "filters": [
            {
                "op": "eq",
                "property": "allowEntity",
                "value": true
            },
            {
                "op": "eq",
                "property": "descriptionPropertyName",
                "value": "description"
            },
            {
                "op": "eq",
                "property": "objectIdPropertyName",
                "value": "objectType"
            }
        ]
    }
});

// should match one object
assert(length(objects) === 1);

for (const obj of objects) {
    assert(obj.allowEntity === true);
    assert(obj.descriptionPropertyName === "description");
    assert(obj.objectIdPropertyName === "objectType");
}

return 0;


//? test: and_or
//? description: Test rql "and" operator with nested or filter
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "and",
        "filters": [
            {
                "op": "eq",
                "property": "allowEntity",
                "value": true
            },
            {
                "op": "or",
                "filters": [
                    {
                        "op": "eq",
                        "property": "descriptionPropertyName",
                        "value": "description"
                    },
                    {
                        "op": "eq",
                        "property": "objectIdPropertyName",
                        "value": "objectType"
                    }
                ]
            }
        ]
    }
});

// should match one object
assert(length(objects) === 1);

for (const obj of objects) {
    assert(obj.allowEntity === true);
    assert(obj.descriptionPropertyName === "description" || obj.objectIdPropertyName === "objectType");
}

return 0;