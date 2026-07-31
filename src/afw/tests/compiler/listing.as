#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: listing.as
//? customPurpose: Part of compiler category tests
//? description: Human-oriented compile listings (Fiddle) — not decompile/stringify
//? sourceType: script
//?
//? test: listing-object-expression-free-vars
//? description: object_expression listing must not evaluate property values (regression)
//? skip: false
//? expect: 0
//? source: ...

/*
 * Before fix, listing walked object_expression via as_object which evaluated
 * properties and threw on free variables (e.g. x in {a: x+1}).
 */
const L = compile<script>(script(
    "const x = 1;\nconst o = {a: x + 1, b: 2};\nreturn o;"
), 2);

assert(includes(L, "---CompiledValue"));
assert(includes(L, "---Symbols"));
assert(includes(L, "object_expression"));
assert(includes(L, "property a"));
assert(includes(L, "property b"));
/* Property value remains compiled form (add + symbol x), not evaluated result */
assert(includes(L, "function_definition add") || includes(L, "add"));
assert(includes(L, "symbol_reference"));

return 0;

//? test: listing-switch-default-label
//? description: unique default case labeled switch_default (not plain null)
//? skip: false
//? expect: 0
//? source: ...

const L = compile<script>(script(
    "switch (1) { case 1: return 1; default: return 0; }"
), 2);

assert(includes(L, "switch_default"));
assert(includes(L, "function_definition switch") || includes(L, "switch"));

return 0;

//? test: listing-script-function-rest
//? description: rest parameter marked with ... in listing parameters list
//? skip: false
//? expect: 0
//? source: ...

const L = compile<script>(script(
    "function f(a: integer, ...rest) { return a; }\nreturn f(1, 2);"
), 2);

assert(includes(L, "script_function"));
assert(includes(L, "...rest"));
/* Symbol table still lists rest as a normal parameter name */
assert(includes(L, "parameter rest") || includes(L, "parameter"));

return 0;

//? test: listing-symbols-section
//? description: listing always includes ---Symbols block walk for the unit
//? skip: false
//? expect: 0
//? source: ...

const L = compile<script>(script(
    "const x: integer = 1;\nreturn x;"
), 2);

assert(includes(L, "---CompiledValue"));
assert(includes(L, "---Symbols"));
assert(includes(L, "const x: integer") || includes(L, "x: integer"));
assert(includes(L, "block="));

return 0;

//? test: listing-template-parts
//? description: compile<template> listing shows template_definition parts
//? skip: false
//? expect: 0
//? source: ...

const L = compile<template>(template("hi ${1+2} there"), 2);

assert(includes(L, "template_definition"));
assert(includes(L, "string"));
assert(includes(L, "function_definition add") || includes(L, "add"));

return 0;
