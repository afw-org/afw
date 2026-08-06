#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: conversion_functions.as
//? customPurpose: Part of language/script tests
//? description: ...
Type-named conversion functions and related constructors:
- Classic convert (integer, boolean, …)
- string multi-arg concat
- object from JSON text
- null/function type-named converts removed (types remain)
- array/create_array are constructors (see array_semantics.as)
//? sourceType: script
//?
//? test: integer-convert
//? description: integer(string) converts
//? expect: 0
//? source: ...

assert(integer("42") === 42);
assert(integer(7) === 7);
return 0;

//?
//? test: boolean-convert
//? description: boolean convert
//? expect: 0
//? source: ...

assert(boolean(true) === true);
assert(boolean("true") === true);
return 0;

//?
//? test: string-one-arg-convert
//? description: string(x) is convert-to-string
//? expect: 0
//? source: ...

assert(string(1) === "1");
assert(string(true) === "true");
return 0;

//?
//? test: string-multi-arg-concat
//? description: string(a,b,...) converts each then concatenates
//? expect: 0
//? source: ...

assert(string(1, "a", true) === "1atrue");
assert(string(undefined) === "undefined");
return 0;

//?
//? test: object-from-json-text
//? description: object(string) parses JSON object text
//? expect: 0
//? source: ...

const o = object("{}");
assert(is_object(o));
const o2 = object("{a:1}");
assert(o2.a === 1);
const o3 = object({ b: 2 });
assert(o3.b === 2);
return 0;

//?
//? test: dayTimeDuration-convert-still-works
//? description: dayTimeDuration convert exists (brief typo only was fixed)
//? expect: 0
//? source: ...

const d = dayTimeDuration("P1DT2H");
assert(is_dayTimeDuration(d));
return 0;

//?
//? test: is-null-type-still-works
//? description: null data type and is_null remain without null() convert
//? expect: 0
//? source: ...

assert(is_null(null) === true);
assert(is_null(0) === false);
assert(is_nullish(null) === true);
assert(is_nullish(undefined) === true);
return 0;

//?
//? test: is-function-type-still-works
//? description: function data type and is_function remain without function() convert
//? expect: 0
//? source: ...

const f = function (x) { return x; };
assert(is_function(f) === true);
assert(is_function(1) === false);
return 0;

//?
//? test: json-source-convert
//? description: json() accepts source text (typed source value; not the same as object())
//? expect: 0
//? source: ...

const j = json("{}");
/* json is a source-holding type (no is_json polymorphic); string form is the source */
assert(string(j) === "{}");
assert(is_string(j) === false);
const o = object("{}");
assert(is_object(o) === true);
return 0;
