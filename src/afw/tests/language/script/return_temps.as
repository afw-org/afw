#!/usr/bin/env -S afw --syntax test_script
//? testScript: return_temps.as
//? customPurpose: Part of language/script tests
//? description: ...
Multiple call arguments that are themselves calls. Each argument is a
return temp that must stay alive until the enclosing call pops, then be
released. Covers two and three parked temps, mixed call/literal, nested
calls, returned closures as arguments, and throw rewind after the first
argument is already a return temp.
//? sourceType: script
//?
//? test: two-call-args-script
//? description: Script function with two arguments that are calls
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let n = 0;
function a() {
    n = n + 1;
    return 1;
}
function b() {
    n = n + 1;
    return 2;
}
function add2(x, y) {
    return x + y;
}

assert(add2(a(), b()) === 3);
assert(n === 2);
assert(add2(a(), b()) === 3);
assert(n === 4);
return 0;
//?
//? test: three-call-args-script
//? description: Three call arguments parked at once
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let n = 0;
function a() {
    n = n + 1;
    return 10;
}
function b() {
    n = n + 1;
    return 20;
}
function c() {
    n = n + 1;
    return 30;
}
function add3(x, y, z) {
    return x + y + z;
}

assert(add3(a(), b(), c()) === 60);
assert(n === 3);
return 0;
//?
//? test: two-call-args-operator
//? description: Built-in + / === with both sides calls
//? expect: 0
//? source: ...
#!/usr/bin/env afw

function a() {
    return 1;
}
function b() {
    return 2;
}

assert(a() + b() === 3);
assert(a() === a());
assert(a() !== b());
return 0;
//?
//? test: mixed-call-and-literal
//? description: Call arguments mixed with literals
//? expect: 0
//? source: ...
#!/usr/bin/env afw

function a() {
    return 4;
}
function add3(x, y, z) {
    return x + y + z;
}

assert(add3(a(), 1, a()) === 9);
assert(add3(0, a(), 2) === 6);
return 0;
//?
//? test: nested-call-args
//? description: Argument is a call whose own arguments are calls
//? expect: 0
//? source: ...
#!/usr/bin/env afw

function a() {
    return 1;
}
function b() {
    return 2;
}
function c() {
    return 3;
}
function add2(x, y) {
    return x + y;
}

assert(add2(add2(a(), b()), c()) === 6);
return 0;
//?
//? test: two-closure-args
//? description: Two arguments are calls that return closures
//? expect: 0
//? source: ...
#!/usr/bin/env afw

function make(n) {
    return function () {
        return n;
    };
}
function pair(f, g) {
    return f() + g();
}

assert(pair(make(1), make(2)) === 3);
let p = pair(make(10), make(20));
assert(p === 30);
return 0;
//?
//? test: throw-after-first-call-arg
//? description: ...
First argument is a call (return temp parked). Second throws. Rewind
must release the parked temp; a later two-arg call still works.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let n = 0;
function a() {
    n = n + 1;
    return 1;
}
function boom() {
    throw "mid-arg";
}
function c() {
    n = n + 1;
    return 3;
}
function add3(x, y, z) {
    return x + y + z;
}

try {
    add3(a(), boom(), c());
    assert(false, "should have thrown");
} catch (e) {
    assert(e.message === "mid-arg");
}
assert(n === 1, "c() must not run after throw");
assert(add3(a(), a(), a()) === 3);
assert(n === 4);
return 0;
//?
//? test: call-arg-then-use-again
//? description: Same callee used as two arguments; each return is its own temp
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let n = 0;
function tick() {
    n = n + 1;
    return n;
}
function add2(x, y) {
    return x + y;
}

assert(add2(tick(), tick()) === 3);
assert(n === 2);
return 0;
