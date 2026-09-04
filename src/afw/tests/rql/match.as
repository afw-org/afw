#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: match.as
//? customPurpose: Part of rql tests
//? description: Test rql "match" operator
//? sourceType: script
//?
//? test: match_none
//? description: Test "match" operator when nothing matches
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "match",
        "property": "objectType",
        "value": "xyz"
    }
}, undefined, undefined, 0);

// we should have no objects that match
assert(length(objects) === 0);

return 0;


//? test: match_one
//? description: Test "match" operator when exactly one object matches
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "match",
        "property": "objectType",
        "value": "_AdaptiveAdapter_"
    }
}, undefined, undefined, 0);

// we should have one object that matches
assert(length(objects) === 1);

return 0;


//? test: match_multi
//? description: Test "match" operator when more than one object matches
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "match",
        "property": "objectType",
        "value": "_AdaptiveA.*"
    }
}, undefined, undefined, 0);

// we should have more than one object that matches
assert(length(objects) > 1);

for (const obj of objects) {
    // make sure every mapped property actually matches the pattern
    assert(regexp_match<string>(obj.objectType, "_AdaptiveA.*"));
}

return 0;


//? test: match_invalid_regexp
//? description: Test "match" operator with an invalid regular expression
//? expect: error:regexp syntax error
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "match",
        "property": "objectType",
        "value": "(a*b"
    }
}, undefined, undefined, 0);
