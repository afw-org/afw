#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: filter_incomplete.as
//? customPurpose: Part of rql tests
//? description: ...
A filter object without a value or property is an error. Testing an
object against a filter with no value dereferenced NULL.
//? sourceType: script
//?
//? test: filter-no-value
//? description: ne with a property and no value throws
//? expect: error:Filter operator 'ne' requires a value
//? source: ...

retrieve_objects("afw", "_AdaptiveFunction_", {
    "filter": {
        "op": "ne",
        "property": "numberOfRequiredParameters"
    }
})

//?
//? test: filter-no-property
//? description: eq with a value and no property throws
//? expect: error:Filter operator 'eq' requires a property
//? source: ...

retrieve_objects("afw", "_AdaptiveFunction_", {
    "filter": {
        "op": "eq",
        "value": 1
    }
})

//?
//? test: nested-filter-no-value
//? description: an incomplete filter inside and throws
//? expect: error:Filter operator 'lt' requires a value
//? source: ...

retrieve_objects("afw", "_AdaptiveFunction_", {
    "filter": {
        "op": "and",
        "filters": [
            { "op": "eq", "property": "category", "value": "string" },
            { "op": "lt", "property": "numberOfRequiredParameters" }
        ]
    }
})
