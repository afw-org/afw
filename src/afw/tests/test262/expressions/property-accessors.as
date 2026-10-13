#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: property-accessors.as
//? customPurpose: Part of test262
//? description: expressions/property-accessors on literals (#602)
//? sourceType: script
//?
//? test: array-literal-bracket
//? description: MemberExpression [ Expression ] on an array literal
//? expect: success
//? source: ...

if ([1, 2, 3][1] !== 2) {
    assert(false, "#1: [1, 2, 3][1] === 2");
}
if ([[1], [2, 3]][1][0] !== 2) {
    assert(false, "#2: [[1], [2, 3]][1][0] === 2");
}
if ([1][5] !== undefined) {
    assert(false, "#3: [1][5] === undefined");
}

//? test: object-literal-dot-and-bracket
//? description: MemberExpression . IdentifierName and [ Expression ] on an object literal
//? expect: success
//? source: ...

if ({a: 1}.a !== 1) {
    assert(false, "#1: {a: 1}.a === 1");
}
if ({a: 1}["a"] !== 1) {
    assert(false, "#2: {a: 1}[\"a\"] === 1");
}
if ({a: 1}.b !== undefined) {
    assert(false, "#3: {a: 1}.b === undefined");
}

//? test: string-literal-bracket
//? description: MemberExpression [ Expression ] on a string literal
//? expect: success
//? source: ...

if ("abc"[0] !== "a") {
    assert(false, "#1: \"abc\"[0] === \"a\"");
}
if ('abc'[2] !== "c") {
    assert(false, "#2: 'abc'[2] === \"c\"");
}

//? test: white-space-before-accessor
//? description: White space and line terminators are allowed before '[' and '.'
//? expect: success
//? source: ...

if (eval(script("return [1, 2] [1];")) !== 2) {
    assert(false, "#1: [1, 2] [1] === 2");
}
if (eval(script("return {a: 1}\u000A.a;")) !== 1) {
    assert(false, "#2: {a: 1}\\n.a === 1");
}

//? test: numeric-literal-decimal-point
//? description: 1.x is a syntax error because '1.' is a numeric literal; 1..x parses
//? expect: success
//? source: ...

let threw = false;
try {
    compile<script>(script("return 1.x;"));
} catch (e) {
    threw = e.id === "syntax";
}
if (!threw) {
    assert(false, "#1: 1.x is a syntax error");
}
compile<script>(script("return 1..x;"));
compile<script>(script("return (1).x;"));
