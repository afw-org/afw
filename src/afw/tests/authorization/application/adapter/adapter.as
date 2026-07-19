#!/usr/bin/env -S afw --syntax test_script --conf ./afw.conf
//?
//? testScript: adapter.as
//? customPurpose: Part of authorization tests
//? description: issue #90 - query deny for get_object and retrieve_objects (unskipped)
//? sourceType: script
//?
//? test: get_object_query_denied
//? description: get_object denied at query for specific resource
//? expect: 0
//? source: ...

let caught = false;
try {
    get_object("afw", "_AdaptiveObject_", "get-object-query");
} catch (e) {
    assert(e.id === "denied");
    caught = true;
}
assert(caught);
return 0;

//?
//? test: retrieve_objects_permit_runtime
//? description: retrieve of runtime adapters is permitted by default policy branch
//? expect: 0
//? source: ...

/*
 * Policy only denies a specific get resource; general retrieve on afw should
 * succeed (permit path) when not matching the deny case.
 */
let list = retrieve_objects("afw", "_AdaptiveAdapter_");
assert(length(list) >= 1);
return 0;
