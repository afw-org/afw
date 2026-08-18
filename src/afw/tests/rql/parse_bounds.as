#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: parse_bounds.as
//? customPurpose: Part of rql tests
//? description: urlEncodedRQLString percent-decode and filter nesting bounds
//? sourceType: script
//?
//? test: encoded-underscore-still-matches
//? description: valid %5f in a filter value still retrieves
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "urlEncodedRQLString": "objectType=_AdaptiveObjectType%5f"
}, undefined, undefined, 0);
assert(length(objects) === 1);
assert(objects[0].objectType === "_AdaptiveObjectType_");
return 0;

//?
//? test: trailing-percent-is-error
//? description: a value ending in a bare % is invalid percent-encoding
//? expect: error
//? source: ...

retrieve_objects("afw", "_AdaptiveObjectType_", {
    "urlEncodedRQLString": "objectType=_AdaptiveObjectType_%"
}, undefined, undefined, 0)

//?
//? test: truncated-hex-is-error
//? description: % followed by one hex digit is invalid
//? expect: error
//? source: ...

retrieve_objects("afw", "_AdaptiveObjectType_", {
    "urlEncodedRQLString": "objectType=foo%2"
}, undefined, undefined, 0)

//?
//? test: non-hex-percent-is-error
//? description: % followed by non-hex is invalid
//? expect: error
//? source: ...

retrieve_objects("afw", "_AdaptiveObjectType_", {
    "urlEncodedRQLString": "objectType=foo%GG"
}, undefined, undefined, 0)

//?
//? test: and-nesting-modest
//? description: 32 nested and() still parses
//? expect: 0
//? source: ...

function nestAnd(n) {
    let i = 0;
    let s = "eq(objectType,_AdaptiveObjectType_)";
    while (i < n) {
        s = "and(" + s + ")";
        i = i + 1;
    }
    return s;
}

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "urlEncodedRQLString": nestAnd(32)
}, undefined, undefined, 0);
assert(length(objects) === 1);
return 0;

//?
//? test: and-nesting-too-deep
//? description: 300 nested and() is a syntax error
//? expect: error
//? source: ...

function nestAnd(n) {
    let i = 0;
    let s = "eq(objectType,_AdaptiveObjectType_)";
    while (i < n) {
        s = "and(" + s + ")";
        i = i + 1;
    }
    return s;
}

retrieve_objects("afw", "_AdaptiveObjectType_", {
    "urlEncodedRQLString": nestAnd(300)
}, undefined, undefined, 0)

//?
//? test: paren-nesting-modest
//? description: 32 parenthesized FIQL still parses
//? expect: 0
//? source: ...

function wrap(open, mid, close, n) {
    let i = 0;
    let left = "";
    let right = "";
    while (i < n) {
        left = left + open;
        right = right + close;
        i = i + 1;
    }
    return left + mid + right;
}

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", {
    "urlEncodedRQLString": wrap("(", "objectType=_AdaptiveObjectType_", ")", 32)
}, undefined, undefined, 0);
assert(length(objects) === 1);
return 0;

//?
//? test: paren-nesting-too-deep
//? description: 300 parenthesized FIQL is a syntax error
//? expect: error
//? source: ...

function wrap(open, mid, close, n) {
    let i = 0;
    let left = "";
    let right = "";
    while (i < n) {
        left = left + open;
        right = right + close;
        i = i + 1;
    }
    return left + mid + right;
}

retrieve_objects("afw", "_AdaptiveObjectType_", {
    "urlEncodedRQLString": wrap("(", "objectType=_AdaptiveObjectType_", ")", 300)
}, undefined, undefined, 0)
