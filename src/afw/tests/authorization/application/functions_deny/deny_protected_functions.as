#!/usr/bin/env -S afw --syntax test_script --conf ./afw.conf
//?
//? testScript: deny_protected_functions.as
//? customPurpose: Part of authorization tests
//? description: Test deny access to protected functions
//? sourceType: script
//?
//? test: deny_extension_load
//? description: Test deny access to extension_load
//? expect: undefined
//? source: ...

let caught = false;

try {
    extension_load('afw_curl');
} catch (e) {
    trace(e);
    assert(e.id === "denied");
    caught = true;
}

assert(caught);

//?
//? test: deny_extension_load_by_module_path
//? description: Test deny access to extension_load_by_module_path
//? expect: undefined
//? source: ...

let caught = false;

try {
    extension_load_by_module_path('files'); // Should fail before path used.
} catch (e) {
    trace(e);
    assert(e.id === "denied");
    caught = true;
}

assert(caught);

//?
//? test: deny_service_get
//? description: Test deny access to service_get
//? expect: undefined
//? source: ...

let caught = false;

try {
    service_get('files');
} catch (e) {
    trace(e);
    assert(e.id === "denied");
    caught = true;
}

assert(caught);

//?
//? test: deny_service_restart
//? description: Test deny access to service_restart
//? expect: undefined
//? source: ...

let caught = false;

try {
    service_restart('files');
} catch (e) {
    trace(e);
    assert(e.id === "denied");
    caught = true;
}

assert(caught);

//?
//? test: deny_service_stop
//? description: Test deny access to service_stop
//? expect: undefined
//? source: ...

let caught = false;

try {
    service_stop('files');
} catch (e) {
    trace(e);
    assert(e.id === "denied");
    caught = true;
}

assert(caught);

//?
//? test: deny_service_start
//? description: Test deny access to service_start
//? expect: undefined
//? source: ...

let caught = false;

try {
    service_start('files');
} catch (e) {
    trace(e);
    assert(e.id === "denied");
    caught = true;
}

assert(caught);

