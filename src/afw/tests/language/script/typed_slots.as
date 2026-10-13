#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: typed_slots.as
//? customPurpose: Part of language/script tests
//? description: ...
Typed script slots check and never convert (#583), with type checking off
(the default) as well as on: script function parameters, let and const
declarations, assignments, and function returns. A data type, T[] of a data
type, and unions of these are always checked, the same as a built-in
parameter. Object shapes are checked only under #compile typeCheck. With
strictNullChecks off, null and undefined are allowed as before.
//? sourceType: script
//?
//? test: parameter-data-type
//? description: a typed parameter rejects another data type
//? expect: error:Type error in parameter: expected integer but got double; write to_integer(v)
//? source: ...

function n(x: integer) { return x; }
n(1.9);
return 0;

//?
//? test: parameter-string-not-parsed
//? description: a string for a typed parameter is not read as the data type
//? expect: error:Type error in parameter: expected dateTime but got string; write "abc"dateTime or to_dateTime(v)
//? source: ...

function d(a: dateTime) { return a; }
d("abc");
return 0;

//?
//? test: parameter-ok
//? description: values of the declared data type pass unchanged
//? expect: 0
//? source: ...

function n(x: integer): integer { return x; }
function d(a: dateTime) { return a; }
assert(n(12) === 12);
assert(n(to_integer("12")) === 12);
assert(d("2026-10-12T00:00:00Z"dateTime) === "2026-10-12T00:00:00Z"dateTime);
return 0;

//?
//? test: const-data-type
//? description: a typed const rejects another data type
//? expect: error:Type error in assignment: expected dateTime but got string; write "abc"dateTime or to_dateTime(v)
//? source: ...

function text(): any { return "abc"; }
const x: dateTime = text();
return 0;

//?
//? test: let-assign-data-type
//? description: assigning another data type to a typed let is an error
//? expect: error:Type error in assignment: expected integer but got double; write to_integer(v)
//? source: ...

function half(): any { return 1.5; }
let y: integer = 1;
y = half();
return 0;

//?
//? test: return-data-type
//? description: a declared return data type is checked
//? expect: error:Type error in return: expected integer but got string; write to_integer(v)
//? source: ...

function f(): integer { const v: any = "1"; return v; }
f();
return 0;

//?
//? test: array-of-data-type
//? description: T[] is checked against the array's data type
//? expect: error:Type error in parameter: array element 1: expected integer but got string
//? source: ...

function ints(a: integer[]) { return a; }
assert(length(ints([1, 2])) === 2);
assert(length(ints([])) === 0);
ints([1, "2"]);
return 0;

//?
//? test: union-of-data-types
//? description: a union of data types is checked
//? expect: 0
//? source: ...

function f(v: integer | string) { return v; }
assert(f(1) === 1);
assert(f("a") === "a");
let threw = false;
try { f(1.5); } catch (e) { threw = true; }
assert(threw, "double is not integer | string");
return 0;

//?
//? test: nullish-allowed
//? description: with strictNullChecks off, null and undefined pass a typed slot
//? expect: 0
//? source: ...

function n(x?: integer) { return x; }
assert(n() === undefined);
assert(n(null) === null);
let z: integer;
z = null;
return 0;

//?
//? test: shape-not-checked-when-off
//? description: an object shape is checked only under typeCheck
//? expect: 0
//? source: ...

function p(o: {a: integer}) { return o.a; }
assert(p({a: "x"}) === "x");
return 0;

//?
//? test: shape-checked-under-typeCheck
//? description: an object shape is checked under typeCheck
//? expect: error
//? source: ...

#compile typeCheck;
function p(o: {a: integer}) { return o.a; }
const v: any = {a: "x"};
p(v);
return 0;
