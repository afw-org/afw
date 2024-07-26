#!/usr/bin/env -S afw --syntax test_script --conf ./afw.conf
//?
//? testScript: adapter.as
//? customPurpose: Part of authorization tests
//? description: Test adapter function decisions using application authorizationControl
//? sourceType: script
//?
//? test: get_object
//? description: Test get_object
//? expect: undefined
//? skip: true
//? source: ...

let caught = false;

try {
    get_object("afw", "_AdaptiveObject_", "get-object-query");
} catch (e) {
    trace(e);
    assert(e.id === "denied");
    caught = true;
}

assert(caught);

//? test: retrieve_objects
//? description: Test retrieve_objects
//? expect: undefined
//? skip: true
//? source: ...

let caught = false;
flag_set('response:error');

try {
    retrieve_objects("afw", "_AdaptiveAdapter_");
} catch (e) {
    assert(e.id === "denied");
    caught = true;
}

assert(caught);