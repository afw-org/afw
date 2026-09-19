#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: function.as
//? customPurpose: Part of language/script tests
//? description: Function-as-value call args (#89 regression; keep green under #2)
//? sourceType: script
//?
//? test: function_array_as_parameter
//? description: #89 — named function from array element as call argument
//? skip: false
//? expect: "foo"
//? source: ...

function foo() {
    return "foo";
}

function bar(f) {
    return f();
}

let a = [
    foo
];

return bar(a[0]);

//?
//? test: function_array_index_variable
//? description: #89 — a[i] with variable index still yields callable function
//? skip: false
//? expect: "x"
//? source: ...

function foo() {
    return "x";
}

function bar(f) {
    return f();
}

let a = [foo];
let i = 0;
return bar(a[i]);

//?
//? test: function_array_two_slots
//? description: #89 — multiple array slots of named functions
//? skip: false
//? expect: true
//? source: ...

function a1() {
    return 1;
}

function a2() {
    return 2;
}

function call(f) {
    return f();
}

let a = [a1, a2];
assert(call(a[0]) === 1);
assert(call(a[1]) === 2);
return true;

//?
//? test: function_array_assign_then_pass
//? description: #89 — load function from array into local, then pass
//? skip: false
//? expect: "ok"
//? source: ...

function foo() {
    return "ok";
}

function bar(f) {
    return f();
}

let a = [foo];
let g = a[0];
return bar(g);

//?
//? test: function_array_nested
//? description: #89 — function in nested array a[0][0]
//? skip: false
//? expect: "n"
//? source: ...

function foo() {
    return "n";
}

function bar(f) {
    return f();
}

let a = [[foo]];
return bar(a[0][0]);

//?
//? test: function_array_after_push
//? description: #89 — function pushed onto array then passed as argument
//? skip: false
//? expect: 7
//? source: ...

function foo() {
    return 7;
}

function bar(f) {
    return f();
}

let a = [];
push(a, foo);
return bar(a[0]);

//?
//? test: function_object_property_as_parameter
//? description: #89-adjacent — named function from object property as call arg
//? skip: false
//? expect: "foo"
//? source: ...

function foo() {
    return "foo";
}

function bar(f) {
    return f();
}

let o = { f: foo };
return bar(o.f);

//?
//? test: function_return_closure_as_argument
//? description: #89-adjacent — pass returned closure into another function and call
//? skip: false
//? expect: 0
//? source: ...

/*
 * Companion to closures.as before2-pass-closure-as-argument. Named here so
 * #89 call-arg / convert paths stay covered in this file too.
 */
function apply(fn) {
    return fn();
}

function make() {
    let n = 5;
    return function() {
        return n;
    };
}

assert(apply(make()) === 5);
return 0;
