#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: throw_scope_cleanups.as
//? customPurpose: Part of language/script tests
//? description: ...
A scope that ends while a throw is processed runs its registered releases
at the ENDTRY that handles the error (#644). Each test stores a closure in
a container held by a release registered on a try block or loop body
scope, then throws through that scope. Without the fix about 550 bytes
per call stay in use; with it memory stays flat.
//? sourceType: script
//?
//? test: push-closure-in-try
//? description: push a closure in a try block, throw: memory stays flat
//? expect: 0
//? source: ...

function f() {
    let fns = [];
    try {
        push(fns, function () { return fns; });
        throw "x";
    } catch (e) {
        return 1;
    }
}
let start = 0;
for (let i = 0; i < 2010; i = i + 1) {
    if (i == 10) { start = pool_bytes_in_use(); }
    f();
}
let used = pool_bytes_in_use() - start;
assert(used < 200000, "closure in array after throw is released: " + string(used));
return 0;

//? test: push-closure-throw-in-inner-block
//? description: push a closure in a try block, throw from an inner block: memory stays flat
//? expect: 0
//? source: ...

function f() {
    let fns = [];
    try {
        push(fns, function () { return 1; });
        { throw "x"; }
    } catch (e) {
    }
    return 1;
}
let start = 0;
for (let i = 0; i < 2010; i = i + 1) {
    if (i == 10) { start = pool_bytes_in_use(); }
    f();
}
let used = pool_bytes_in_use() - start;
assert(used < 200000, "closure in array after throw is released: " + string(used));
return 0;

//? test: object-closure-in-try
//? description: set an object property to a closure in a try block, throw: memory stays flat
//? expect: 0
//? source: ...

function f() {
    let o = {};
    try {
        o.f = function () { return o; };
        throw "x";
    } catch (e) {
        return 1;
    }
}
let start = 0;
for (let i = 0; i < 2010; i = i + 1) {
    if (i == 10) { start = pool_bytes_in_use(); }
    f();
}
let used = pool_bytes_in_use() - start;
assert(used < 200000, "closure in object after throw is released: " + string(used));
return 0;

//? test: push-closure-in-loop-body
//? description: push a closure in a loop body, throw from the loop: memory stays flat, catch sees the array
//? expect: 0
//? source: ...

function f() {
    let fns = [];
    try {
        for (let i = 0; i < 3; i = i + 1) {
            push(fns, function () { return i; });
            if (i == 1) { throw "z"; }
        }
    } catch (e) {
        return length(fns);
    }
}
let start = 0;
for (let i = 0; i < 2010; i = i + 1) {
    if (i == 10) { start = pool_bytes_in_use(); }
    assert(f() == 2, "catch sees both closures");
}
let used = pool_bytes_in_use() - start;
assert(used < 200000, "closures in array after throw are released: " + string(used));
return 0;

//? test: push-closure-rethrow
//? description: push a closure in a try block, rethrow from its catch: memory stays flat
//? expect: 0
//? source: ...

function f() {
    let fns = [];
    try {
        try {
            push(fns, function () { return fns; });
            throw "x";
        } catch (e) {
            throw "y";
        }
    } catch (e) {
        assert(e.message === "y", "outer catch sees the rethrow");
    }
    return 1;
}
let start = 0;
for (let i = 0; i < 2010; i = i + 1) {
    if (i == 10) { start = pool_bytes_in_use(); }
    f();
}
let used = pool_bytes_in_use() - start;
assert(used < 200000, "closure in array after rethrow is released: " + string(used));
return 0;
