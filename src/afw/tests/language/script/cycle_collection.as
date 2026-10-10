#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: cycle_collection.as
//? customPurpose: Part of language/script tests
//? description: ...
Reference cycles made by script are collected (#458). Each test calls a
function that makes a cycle many times; pool bytes stay flat. A scope
lists the value releases registered on its pool as references it holds:
push() holds its array for the scope's lifetime, so an array in a frame
that holds a closure over that frame is a cycle through that hold.
//? sourceType: script
//?
//? test: object-references-itself
//? description: o.self = o is collected
//? expect: 0
//? source: ...

function mk() { let o = {}; o.self = o; return 0; }
let start = 0;
for (let i = 0; i < 2000; i = i + 1) {
    if (i == 10) { start = pool_bytes_in_use(); }
    mk();
}
assert(pool_bytes_in_use() - start < 200000, "self cycle collected");
return 0;

//?
//? test: closure-pushed-into-frame-array
//? description: a closure over a frame pushed into an array in that frame
//? expect: 0
//? source: ...

function mk() {
    let fns = [];
    push(fns, function () { return 1; });
    return 0;
}
let start = 0;
for (let i = 0; i < 2000; i = i + 1) {
    if (i == 10) { start = pool_bytes_in_use(); }
    mk();
}
assert(pool_bytes_in_use() - start < 200000, "push hold cycle collected");
return 0;

//?
//? test: closure-assigned-to-frame-array-element
//? description: a closure over a frame stored in an element of an array in that frame
//? expect: 0
//? source: ...

function mk() {
    let fns = [];
    fns[0] = function () { return 1; };
    return 0;
}
let start = 0;
for (let i = 0; i < 2000; i = i + 1) {
    if (i == 10) { start = pool_bytes_in_use(); }
    mk();
}
assert(pool_bytes_in_use() - start < 200000, "element cycle collected");
return 0;

//?
//? test: closures-pushed-per-loop-trip
//? description: per-trip closures pushed into an array in the function frame
//? expect: 0
//? source: ...

function mk() {
    let fns = [];
    for (let j = 0; j < 3; j = j + 1) {
        push(fns, function () { return j; });
    }
    return fns[2]();
}
let start = 0;
for (let i = 0; i < 1000; i = i + 1) {
    if (i == 10) { start = pool_bytes_in_use(); }
    assert(mk() === 2);
}
assert(pool_bytes_in_use() - start < 200000, "per-trip cycles collected");
return 0;

//?
//? test: array-pushed-into-itself
//? description: push(a, a) is collected
//? expect: 0
//? source: ...

function mk() { let a = []; push(a, a); return 0; }
let start = 0;
for (let i = 0; i < 2000; i = i + 1) {
    if (i == 10) { start = pool_bytes_in_use(); }
    mk();
}
assert(pool_bytes_in_use() - start < 200000, "array self cycle collected");
return 0;

//?
//? test: compiled-script-evaluated-repeatedly
//? description: a closure cycle inside a script evaluated many times
//? expect: 0
//? source: ...

const c = compile<script>(script(
    "let fns = []; push(fns, function () { return 1; }); return fns[0]();"));
let start = 0;
for (let i = 0; i < 2000; i = i + 1) {
    if (i == 10) { start = pool_bytes_in_use(); }
    assert(evaluate(c) === 1);
}
assert(pool_bytes_in_use() - start < 200000, "evaluated cycles collected");
return 0;
