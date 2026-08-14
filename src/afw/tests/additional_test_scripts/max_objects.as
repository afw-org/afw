#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: max_objects.as
//? customPurpose: Part of core function tests
//? description: issue #49 - retrieve_objects maxObjects bounds materializing array
//? sourceType: script
//?
//? test: default_max_throws_on_large_catalog
//? description: Default maxObjects 100 throws payload_too_large for large OT catalog
//? expect: 0
//? source: ...

let caught = false;
try {
    retrieve_objects("afw", "_AdaptiveObjectType_");
} catch (e) {
    assert(e.id === "payload_too_large");
    caught = true;
}
assert(caught);
return 0;

//?
//? test: maxObjects_zero_unlimited
//? description: maxObjects 0 returns full AdaptiveObjectType catalog
//? expect: 0
//? source: ...

const objects = retrieve_objects("afw", "_AdaptiveObjectType_", undefined, undefined, undefined, 0);
assert(length(objects) > 100);
return 0;

//?
//? test: maxObjects_explicit_small
//? description: maxObjects 2 throws when catalog exceeds 2
//? expect: 0
//? source: ...

let caught = false;
try {
    retrieve_objects("afw", "_AdaptiveObjectType_", undefined, undefined, undefined, 2);
} catch (e) {
    assert(e.id === "payload_too_large");
    caught = true;
}
assert(caught);
return 0;

//?
//? test: with_uri_maxObjects_zero
//? description: retrieve_objects_with_uri honors maxObjects 0
//? expect: 0
//? source: ...

const objects = retrieve_objects_with_uri(
    anyURI("/afw/_AdaptiveObjectType_/"), undefined, undefined, 0);
assert(length(objects) > 100);
return 0;

//?
//? test: with_uri_default_max_throws
//? description: retrieve_objects_with_uri default max throws on large catalog
//? expect: 0
//? source: ...

let caught = false;
try {
    retrieve_objects_with_uri(anyURI("/afw/_AdaptiveObjectType_/"));
} catch (e) {
    assert(e.id === "payload_too_large");
    caught = true;
}
assert(caught);
return 0;
