#!/usr/bin/env -S afw --syntax test_script --conf ./afw.conf
//?
//? testScript: requiresExecuteAccess.as
//? customPurpose: Part of authorization tests
//? description: Test requiresExecuteAccess with authorization code
//? sourceType: script
//?
//? test: service_get vs service_stop
//? description: Test service_get
//? expect: undefined
//? source: ...

let caughtServiceGet = false;
let caughtServiceStop = false;

try {
    service_get('adapter-files');
} catch (e) {
    trace(e);
    assert(e.id === "denied");
    caughtServiceGet = true;
}

try {
    service_stop('adapter-files');
} catch (e) {
    trace("caught error for service_stop");
    assert(e.id === "denied");
    caughtServiceStop = true;
}

assert(!caughtServiceGet && caughtServiceStop);


//? test: service_restart with one argument
//? description: Test service_restart one argument
//? expect: undefined
//? source: ...

let caughtServiceRestart = false;

try {
    service_restart('adapter-files');
} catch (e) {
    trace(e);
    caughtServiceRestart = true;
}

assert(!caughtServiceRestart);


//? test: service_restart with two arguments
//? description: Test service_restart with two arguments
//? expect: undefined
//? source: ...

let caughtServiceRestart = false;

try {
    service_restart('adapter-files', 42);
} catch (e) {
    trace(e);
    // we'll get an error for invalid args, but not denied
    assert(e.id !== "denied");
    caughtServiceRestart = true;
}

assert(caughtServiceRestart);