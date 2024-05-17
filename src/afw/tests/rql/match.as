#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: match.as
//? customPurpose: Part of rql tests
//? description: Test rql "match" operator
//? sourceType: script
//?
//? test: not implemented
//? description: Test "match" operator
//? expect: error:Not implemented
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "match",
        "property": "objectType",
        "value": "xyz"
    }
});
