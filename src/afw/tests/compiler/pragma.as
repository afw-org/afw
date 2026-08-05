#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: pragma.as
//? customPurpose: Part of compiler category tests
//? description: PragmaStatement / PragmaValue structural compiled-form round-trips
//? sourceType: script
//?
//? test: pragma-unknown-value
//? description: Unknown #name in expression position
//? expect: error
//? source: ...

return #nosuch;

//? test: pragma-unknown-value-call-like
//? description: Unknown #name() in expression position
//? expect: error
//? source: ...

return #nosuch(1);

//? test: pragma-unknown-statement
//? description: Unknown #name as statement
//? expect: error
//? source: ...

#nosuchpragma;
return 0;

//? test: pragma-closure-binding-not-recompilable-value
//? description: #closure_binding known but runtime-only (not "unknown pragma")
//? expect: error
//? source: ...

/* Message includes: runtime-only (closed-over scope) and cannot be recompiled */
return #closure_binding(#script_function(a,#block(return(a))));

//? test: pragma-closure-binding-not-recompilable-statement
//? description: #closure_binding as statement also rejected clearly
//? expect: error
//? source: ...

#closure_binding(#script_function(a,#block(return(a))));
return 0;

//? test: pragma-function-thunk-not-recompilable-value
//? description: #function_thunk known but C-side only (not recompilable)
//? expect: error
//? source: ...

/* Message includes: C-side only and cannot be recompiled */
return #function_thunk("detail");

//? test: pragma-function-thunk-not-recompilable-statement
//? description: #function_thunk as statement also rejected clearly
//? expect: error
//? source: ...

#function_thunk("detail");
return 0;

//? test: pragma-block-literal
//? description: #block(add(1,2)) compiles and evaluates to 3
//? skip: false
//? expect: 0
//? source: ...

assert(evaluate(compile<script>(script("#block(add(1,2))"))) == 3);
return 0;

//? test: pragma-block-empty
//? description: Empty #block() compiles
//? skip: false
//? expect: 0
//? source: ...

const r = evaluate(compile<script>(script("#block()")));
assert(!is_defined(r));
return 0;

//? test: pragma-block-roundtrip-expr
//? description: decompile/compile round-trip for expression script 1+2
//? skip: false
//? expect: 0
//? source: ...

const d = decompile(compile<script>(script("1+2")));
assert(d == "#block(add(1,2))");
assert(evaluate(compile<script>(script(d))) == 3);
/* Outer script block unwraps sole #block so decompile is stable. */
assert(decompile(compile<script>(script(d))) == d);
return 0;

//? test: pragma-block-roundtrip-return
//? description: decompile/compile round-trip for return 1+2
//? skip: false
//? expect: 0
//? source: ...

const d = decompile(compile<script>(script("return 1+2;")));
assert(d == "#block(return(add(1,2)))");
assert(evaluate(compile<script>(script(d))) == 3);
assert(decompile(compile<script>(script(d))) == d);
return 0;

//? test: pragma-block-as-value
//? description: #block as expression value (parenthesized / assigned path)
//? skip: false
//? expect: 0
//? source: ...

// Nested use: outer script evaluates an expression that is #block(...)
assert(evaluate(compile<script>(script(
    "const b = #block(add(10, 20));\nreturn evaluate(b);"
))) == 30);
return 0;

//? test: pragma-assignment-target-roundtrip-const
//? description: decompile/compile round-trip for const + return
//? skip: false
//? expect: 0
//? source: ...

const d = decompile(compile<script>(script(
    "const x = 1 + 2;\nreturn x;"
)));
assert(d ==
    "#block(const(#assignment_target(\"const\",x),add(1,2),undefined),return(x))");
assert(evaluate(compile<script>(script(d))) == 3);
assert(decompile(compile<script>(script(d))) == d);
return 0;

//? test: pragma-assignment-target-direct
//? description: #assignment_target as value inside #block
//? skip: false
//? expect: 0
//? source: ...

assert(evaluate(compile<script>(script(
    "#block(const(#assignment_target(\"const\",y),10,undefined),return(y))"
))) == 10);
return 0;

//? test: pragma-assignment-target-string-name
//? description: Variable name may be a string in #assignment_target
//? skip: false
//? expect: 0
//? source: ...

assert(evaluate(compile<script>(script(
    "#block(const(#assignment_target(\"const\",\"z\"),7,undefined),return(z))"
))) == 7);
return 0;

//? test: pragma-assignment-target-list-destructure-roundtrip
//? description: decompile/compile round-trip for list destructure Pattern
//? skip: false
//? expect: 0
//? source: ...

const d = decompile(compile<script>(script(
    "const [a, b] = [1, 2];\nreturn a;"
)));
assert(d ==
    "#block(const(#assignment_target(\"const\",[a,b]),[1,2],undefined),return(a))");
assert(evaluate(compile<script>(script(d))) == 1);
assert(decompile(compile<script>(script(d))) == d);
return 0;

//? test: pragma-assignment-target-object-destructure-roundtrip
//? description: ...
decompile/compile round-trip for object destructure Pattern. Expect
wrap_literal_object on the object literal RHS (issue #17); evaluate and
re-decompile must match.
//? skip: false
//? expect: 0
//? source: ...

const d = decompile(compile<script>(script(
    "const {a, b} = {a: 1, b: 2};\nreturn b;"
)));
assert(d ==
    "#block(const(#assignment_target(\"const\",{a,b}),wrap_literal_object({\"a\":1,\"b\":2}),undefined),return(b))");
assert(evaluate(compile<script>(script(d))) == 2);
assert(decompile(compile<script>(script(d))) == d);
return 0;

//? test: pragma-assignment-target-list-hole-rest-default
//? description: list Pattern with hole, default, and rest
//? skip: false
//? expect: 0
//? source: ...

const d = decompile(compile<script>(script(
    "const [a, , c = 9, ...r] = [1, 2, 3, 4, 5];\nreturn [a, c, r];"
)));
assert(d ==
    "#block(const(#assignment_target(\"const\",[a,,c=9,...r]),[1,2,3,4,5],undefined),return(array(a,c,r)))");
assert(evaluate(compile<script>(script(d))) == [1, 3, [4, 5]]);
assert(decompile(compile<script>(script(d))) == d);
return 0;

//? test: pragma-assignment-target-object-rename-default-rest
//? description: ...
object Pattern with rename, default, and rest; decompile shows
wrap_literal_object on the RHS object literal (issue #17); round-trip evaluate.
//? skip: false
//? expect: 0
//? source: ...

const d = decompile(compile<script>(script(
    "const {a: x, b = 3, ...r} = {a: 1, c: 4};\nreturn [x, b, r];"
)));
assert(d ==
    "#block(const(#assignment_target(\"const\",{a:x,b=3,...r}),wrap_literal_object({\"a\":1,\"c\":4}),undefined),return(array(x,b,r)))");
assert(evaluate(compile<script>(script(d))) == [1, 3, {"c": 4}]);
assert(decompile(compile<script>(script(d))) == d);
return 0;

//? test: pragma-assignment-target-nested-list
//? description: nested list Pattern round-trip
//? skip: false
//? expect: 0
//? source: ...

const d = decompile(compile<script>(script(
    "const [a, [b, c]] = [1, [2, 3]];\nreturn c;"
)));
assert(d ==
    "#block(const(#assignment_target(\"const\",[a,[b,c]]),[1,[2,3]],undefined),return(c))");
assert(evaluate(compile<script>(script(d))) == 3);
assert(decompile(compile<script>(script(d))) == d);
return 0;

//? test: pragma-assignment-target-list-direct
//? description: list Pattern written directly in compiled form
//? skip: false
//? expect: 0
//? source: ...

assert(evaluate(compile<script>(script(
    "#block(const(#assignment_target(\"const\",[p,q]),[10,20],undefined),return(add(p,q)))"
))) == 30);
return 0;

//? test: pragma-assignment-target-typed-roundtrip
//? description: typed const decompile/compile round-trip (x: integer)
//? skip: false
//? expect: 0
//? source: ...

const d = decompile(compile<script>(script(
    "const x: integer = 7;\nreturn x;"
)));
assert(d ==
    "#block(const(#assignment_target(\"const\",x:integer),7,undefined),return(x))");
assert(evaluate(compile<script>(script(d))) == 7);
assert(decompile(compile<script>(script(d))) == d);
return 0;

//? test: pragma-script-function-typed-roundtrip
//? description: typed params and return type on #script_function
//? skip: false
//? expect: 0
//? source: ...

const d = decompile(compile<script>(script(
    "const f = function (a: integer): integer { return a + 1; };\nreturn f(10);"
)));
assert(d ==
    "#block(const(#assignment_target(\"const\",f),#script_function(a:integer,#block(return(add(a,1))),integer),undefined),return(f(10)))");
assert(evaluate(compile<script>(script(d))) == 11);
assert(decompile(compile<script>(script(d))) == d);
return 0;

//? test: pragma-typed-array-of-roundtrip
//? description: integer[] type annotation round-trip (TS-like; issue #28)
//? skip: false
//? expect: 0
//? source: ...

const d = decompile(compile<script>(script(
    "const x: integer[] = [1, 2];\nreturn x;"
)));
assert(d ==
    "#block(const(#assignment_target(\"const\",x:integer[]),[1,2],undefined),return(x))");
assert(evaluate(compile<script>(script(d))) == [1, 2]);
assert(decompile(compile<script>(script(d))) == d);
return 0;

//? test: pragma-list-expression-roundtrip
//? description: decompile/compile round-trip for array spread (... surface)
//? skip: false
//? expect: 0
//? source: ...

const d = decompile(compile<script>(script(
    "const a = [1, 2];\nreturn [...a, 3];"
)));
assert(d ==
    "#block(const(#assignment_target(\"const\",a),[1,2],undefined),return(array(...a,3)))");
assert(evaluate(compile<script>(script(d))) == [1, 2, 3]);
assert(decompile(compile<script>(script(d))) == d);
return 0;

//? test: pragma-list-expression-direct
//? description: #list_expression / ... as spread entry inside array constructor
//? skip: false
//? expect: 0
//? source: ...

assert(evaluate(compile<script>(script(
    "#block(const(#assignment_target(\"const\",a),[10,20],undefined),return(array(...a,30)))"
))) == [10, 20, 30]);
/* pragma form still recompiles */
assert(evaluate(compile<script>(script(
    "#block(const(#assignment_target(\"const\",a),[10,20],undefined),return(array(#list_expression(a),30)))"
))) == [10, 20, 30]);
return 0;

//? test: pragma-script-function-roundtrip
//? description: decompile/compile round-trip for #script_function
//? skip: false
//? expect: 0
//? source: ...

const d = decompile(compile<script>(script(
    'const f = function (a) { return a + 1; };\nreturn f(10);'
)));
assert(d ==
    "#block(const(#assignment_target(\"const\",f),#script_function(a,#block(return(add(a,1)))),undefined),return(f(10)))");
assert(evaluate(compile<script>(script(d))) == 11);
assert(decompile(compile<script>(script(d))) == d);
return 0;

//? test: pragma-script-function-direct
//? description: #script_function with param name as identifier
//? skip: false
//? expect: 0
//? source: ...

assert(evaluate(compile<script>(script(
    "#block(const(#assignment_target(\"const\",f),#script_function(a,#block(return(multiply(a,2)))),undefined),return(f(7)))"
))) == 14);
return 0;

//? test: pragma-script-function-multi-param
//? description: #script_function with two params
//? skip: false
//? expect: 0
//? source: ...

assert(evaluate(compile<script>(script(
    "#block(const(#assignment_target(\"const\",f),#script_function(x,y,#block(return(add(x,y)))),undefined),return(f(3,4)))"
))) == 7);
return 0;

//? test: pragma-script-function-multi-roundtrip
//? description: decompile/compile round-trip for multi-param script function
//? skip: false
//? expect: 0
//? source: ...

const d = decompile(compile<script>(script(
    "const f = function (x, y) { return x + y; };\nreturn f(3, 4);"
)));
assert(d ==
    "#block(const(#assignment_target(\"const\",f),#script_function(x,y,#block(return(add(x,y)))),undefined),return(f(3,4)))");
assert(evaluate(compile<script>(script(d))) == 7);
assert(decompile(compile<script>(script(d))) == d);
return 0;

//? test: pragma-script-function-zero-param
//? description: #script_function with no params
//? skip: false
//? expect: 0
//? source: ...

assert(evaluate(compile<script>(script(
    "#block(const(#assignment_target(\"const\",f),#script_function(#block(return(42))),undefined),return(f()))"
))) == 42);
return 0;

//? test: pragma-template-definition-roundtrip
//? description: decompile/compile round-trip for template #template_definition
//? skip: false
//? expect: 0
//? source: ...

const d = decompile(compile<template>(template("hello ${1+2}")));
assert(d ==
    "#block(#template_definition(\"hello \",#block(add(1,2))))");
assert(evaluate(compile<script>(script(d))) == "hello 3");
assert(decompile(compile<script>(script(d))) == d);
return 0;

//? test: pragma-template-definition-direct
//? description: #template_definition evaluates by concatenating parts
//? skip: false
//? expect: 0
//? source: ...

assert(evaluate(compile<script>(script(
    "return #template_definition(\"x=\",#block(add(2,3)));"
))) == "x=5");
return 0;
