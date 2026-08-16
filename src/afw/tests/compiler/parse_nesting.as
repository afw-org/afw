#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: parse_nesting.as
//? customPurpose: Part of compiler category tests
//? description: ...
Type and destructure parse nesting: modest depth still compiles;
pathological depth is a syntax error (not a hang or crash). Limit is
AFW_COMPILE_PARSE_NESTING_MAX (256).
//? sourceType: script
//?
//? test: type-paren-modest-depth
//? description: 32 parenthesized integer still compiles
//? expect: 0
//? source: ...

function wrap(open, mid, close, n) {
    let i = 0;
    let left = "";
    let right = "";
    while (i < n) {
        left = left + open;
        right = right + close;
        i = i + 1;
    }
    return left + mid + right;
}

const src = "const x: " + wrap("(", "integer", ")", 32) + " = 1;\nreturn x;";
assert(evaluate(compile<script>(script(src))) === 1);
return 0;

//?
//? test: type-paren-too-deep
//? description: 300 parenthesized integer is a syntax error
//? expect: 0
//? source: ...

function wrap(open, mid, close, n) {
    let i = 0;
    let left = "";
    let right = "";
    while (i < n) {
        left = left + open;
        right = right + close;
        i = i + 1;
    }
    return left + mid + right;
}

let threw = false;
try {
    compile<script>(script(
        "const x: " + wrap("(", "integer", ")", 300) + " = 1;\nreturn 0;"
    ));
} catch (e) {
    threw = true;
}
assert(threw, "expected compile error for deep type nesting");
return 0;

//?
//? test: pattern-list-modest-depth
//? description: 8-deep list destructure still compiles
//? expect: 0
//? source: ...

function wrap(open, mid, close, n) {
    let i = 0;
    let left = "";
    let right = "";
    while (i < n) {
        left = left + open;
        right = right + close;
        i = i + 1;
    }
    return left + mid + right;
}

const src =
    "const " + wrap("[", "x", "]", 8) + " = " +
    wrap("[", "7", "]", 8) + ";\nreturn x;";
assert(evaluate(compile<script>(script(src))) === 7);
return 0;

//?
//? test: pattern-list-too-deep
//? description: 300-deep list destructure is a syntax error
//? expect: 0
//? source: ...

function wrap(open, mid, close, n) {
    let i = 0;
    let left = "";
    let right = "";
    while (i < n) {
        left = left + open;
        right = right + close;
        i = i + 1;
    }
    return left + mid + right;
}

let threw = false;
try {
    compile<script>(script(
        "const " + wrap("[", "x", "]", 300) + " = " +
        wrap("[", "1", "]", 300) + ";\nreturn 0;"
    ));
} catch (e) {
    threw = true;
}
assert(threw, "expected compile error for deep pattern nesting");
return 0;

//?
//? test: pattern-object-too-deep
//? description: 300-deep object destructure is a syntax error
//? expect: 0
//? source: ...

function wrap(open, mid, close, n) {
    let i = 0;
    let left = "";
    let right = "";
    while (i < n) {
        left = left + open;
        right = right + close;
        i = i + 1;
    }
    return left + mid + right;
}

let threw = false;
try {
    compile<script>(script(
        "const " + wrap("{", "x", "}", 300) + " = {};\nreturn 0;"
    ));
} catch (e) {
    threw = true;
}
assert(threw, "expected compile error for deep object pattern nesting");
return 0;
