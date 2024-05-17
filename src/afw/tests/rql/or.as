#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: or.as
//? customPurpose: Part of rql tests
//? description: Test rql "or" operator
//? sourceType: script
//?
//? test: or_matches
//? description: Test rql "or" operator
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "or",
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

// should match more than one object
assert(length(objects) > 1);

for (const obj of objects) {
    assert(obj.allowEntity === true || obj.descriptionPropertyName === "description");
}

return 0;


//? test: or_three_matches
//? description: Test rql "or" operator with three filters
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "or",
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
                "property": "objectType",
                "value": "_AdaptiveModelObjectType_"
            }
        ]
    }
});

// should match more than one object
assert(length(objects) > 1);

for (const obj of objects) {
    assert(obj.allowEntity === true || obj.descriptionPropertyName === "description" || obj.objectType === "_AdaptiveModelObjectType_");
}

return 0;


//? test: or_and
//? description: Test rql "or" operator with nested "and" filter
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "or",
        "filters": [
            {
                "op": "eq",
                "property": "allowEntity",
                "value": true
            },
            {
                "op": "and",
                "filters": [
                    {
                        "op": "eq",
                        "property": "descriptionPropertyName",
                        "value": "description"
                    },
                    {
                        "op": "eq",
                        "property": "objectType",
                        "value": "_AdaptiveModelObjectType_"
                    }
                ]
            }
        ]
    }
});

// should match more than one object
assert(length(objects) > 1);

for (const obj of objects) {
    assert(obj.allowEntity === true || (obj.descriptionPropertyName === "description" && obj.objectType === "_AdaptiveModelObjectType_"));
}

return 0;