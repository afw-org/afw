#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: conditional.as
//? customPurpose: Part of test262
//? description: Tests ? expression : expression
//? sourceType: script
//?
//? test: 11.12_A2.1_T1
//? description: checking boolean conditions
//? expect: undefined
//? source: ...

assert((true ? false : true) === false, "true ? false : true !== false");
assert((false ? false : true) === true, "false ? false : true !== true");

let x: boolean = true;
let y: boolean = false;
if ((x ? y : true) !== y) {
    throw "x ? y : true !== y";
}

let z: boolean = true;
if ((false ? false : z) !== z) {
    throw "false ? false : z !== z";
}

x = true;
y = false;
z = true;
if ((x ? y : z) !== y) {
    throw "x ? y : z !== y";
}

x = false;
y = false;
z = true;
if ((x ? y : z) !== z) {
    throw "x ? y : z !== z";
}


//? test: 11.12_A2.1_T2
//? description: undeclared reference in first argument throws error
//? expect: error
//? source: ...

if (x ? true : false) {
    throw "x ? true : false";
}


//? test: 11.12_A2.1_T3
//? description: undeclared reference in second argument throws error
//? expect: error
//? source: ...

if (true ? y : false) {
    throw "true ? y : false";
}


//? test: 11.12_A2.1_T4
//? description: undeclared reference in third argument throws error
//? expect: error
//? source: ...

if (false ? true : z) {
    throw "false ? true : z";
}


//? test: 11.12_A3_T1
//? description: If x is false, return z (booleans)
//? expect: undefined
//? source: ...

if ((false ? false : true) !== true) {
    throw "false ? false : true !== true";
}

let z: boolean = true;
if ((false ? true : z) !== z) {
    throw "false ? true : z !== z";
}


//? test: 11.12_A3_T4
//? description: If boolean(x) is false, return z
//? expect: undefined
//? source: ...

if ((false ? true : undefined) !== undefined) {
    throw "false ? true : undefined";
}

if ((false ? true : null) !== null) {
    throw "false ? true : null";
}


//? test: 11.12_A4_T1
//? description: If x is true, return y (boolean)
//? expect: undefined
//? source: ...

if ((true ? false : true) !== false) {
    throw "true ? false : true !== false";
}

let y: boolean = true;
if ((true ? y : false) !== y) {
    throw "true ? y : false !== y";
}


//? test: 11.12_A4_T4
//? description: If x is true, return y (null/undefined)
//? expect: undefined
//? source: ...

if ((true ? undefined : true) !== undefined) {
    throw "true ? undefined : true !== undefined";
}

if ((true ? null : true) !== null) {
    throw "true ? null : true !== null";
}


//? test: coalesce-expr-ternary
//? description: short-circut expression in the conditional expression (? :)
//? expect: undefined
//? source: ...

let x: integer;
x = undefined ?? true ? 0 : 42;
assert(x === 0, "undefined ?? true ? 0 : 42");

x = undefined;
x = null ?? true ? 0 : 42;
assert(x === 0, "null ?? true ? : 42");

x = undefined;
x = undefined ?? false ? 0 : 42;
assert(x === 42, "undefined ?? false ? 0 : 42");

x = undefined;
x = null ?? false ? 0 : 42;
assert(x === 42, "null ?? false ? 0 : 42");

x = undefined;
x = false ?? true ? 0 : 42;
assert(x === 42, "false ?? true ? 0 : 42");

x = undefined;
x = false ?? true ? 0 : 42;
assert(x === 42, "false ?? true ? 0 : 42");

x = undefined;
x = true ?? false ? 0 : 42;
assert(x === 0, "true ?? false ? 0 : 42");

x = undefined;
x = true ?? true ? 0 : 42;
assert(x === 0, "true ?? true ? 0 : 42");