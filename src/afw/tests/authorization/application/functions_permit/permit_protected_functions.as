#!/usr/bin/env -S afw --syntax test_script --conf ./afw.conf
//?
//? testScript: permit_protected_functions.as
//? customPurpose: Part of authorization tests
//? description: Test permit access to protected functions
//? sourceType: script
//?
//? test: permit_extension_load
//? description: Test permit access to extension_load
//? expect: true
//? source: ...

let caught = false;

try {
    extension_load('afw_curl');
} catch (e) {
    trace(e);
    assert(e.id === "denied");
    caught = true;
}

assert(!caught);

//?
//? test: permit_extension_load_by_module_path
//? description: Test permit access to extension_load_by_module_path
//? expect: undefined
//? source: ...

let caught = false;

try {
    extension_load_by_module_path('files'); // Should fail before path used.
} catch (e) {
    trace(e);
    // this may throw a general exception, because the path
    // can't be resolved, but should not fail due to "denied"
    if (e.id === "denied")
        caught = true;
}

assert(!caught);

//?
//? test: permit_service_get
//? description: Test permit access to service_get
//? expect: undefined
//? source: ...

let caught = false;

try {
    let svc = service_get('adapter-files');
    assert(svc.serviceId === "adapter-files");
} catch (e) {
    trace(e);
    assert(e.id === "denied");
    caught = true;
}

assert(!caught);

//?
//? test: permit_service_restart
//? description: Test permit access to service_restart
//? expect: undefined
//? source: ...

let caught = false;

try {
    let svc = service_restart('adapter-files');
    assert(svc.serviceId === "adapter-files");
} catch (e) {
    trace(e);
    assert(e.id === "denied");
    caught = true;
}

assert(!caught);

//?
//? test: permit_service_stop
//? description: Test permit access to service_stop
//? expect: undefined
//? source: ...

let caught = false;

try {
    let svc = service_stop('adapter-files');
    assert(svc.serviceId === "adapter-files");
} catch (e) {
    trace(e);
    assert(e.id === "denied");
    caught = true;
}

assert(!caught);

//?
//? test: permit_service_start
//? description: Test permit access to service_start
//? expect: undefined
//? source: ...

let caught = false;

try {
    let svc = service_start('adapter-files');
    assert(svc.serviceId === "adapter-files");
} catch (e) {
    trace(e);
    assert(e.id === "denied");
    caught = true;
}

assert(!caught);

