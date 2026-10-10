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


//? test: differ
//? description: "differ" is not match, and match(p,x) in function syntax (their expressions were not compiled, so they threw)
//? expect: 0
//? source: ...

const all = retrieve_objects("afw", "_AdaptiveObjectType_",
    undefined, undefined, undefined, 0);
const matched = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "match",
        "property": "objectType",
        "value": "_AdaptiveA.*"
    }
}, undefined, undefined, 0);
const differ = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "differ",
        "property": "objectType",
        "value": "_AdaptiveA.*"
    }
}, undefined, undefined, 0);
assert(length(matched) > 0);
assert(length(differ) === length(all) - length(matched));
for (const o of differ) {
    assert(!starts_with(o.objectType, "_AdaptiveA"));
}

const differ_rql = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "urlEncodedRQLString": "objectType=differ=_AdaptiveA.*"
}, undefined, undefined, 0);
assert(length(differ_rql) === length(differ));

const differ_function = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "urlEncodedRQLString": "differ(objectType,_AdaptiveA.*)"
}, undefined, undefined, 0);
assert(length(differ_function) === length(differ));

const match_function = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "urlEncodedRQLString": "match(objectType,_AdaptiveA.*)"
}, undefined, undefined, 0);
assert(length(match_function) === length(matched));
return 0;


//? test: differ_invalid_regexp
//? description: "differ" with an invalid regular expression
//? expect: error:regexp syntax error
//? source: ...

retrieve_objects("afw", "_AdaptiveObjectType_", {
    "filter": {
        "op": "differ",
        "property": "objectType",
        "value": "(a*b"
    }
}, undefined, undefined, 0);
