#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: literal_evaluation_chain.as
//? customPurpose: Part of language/script tests
//? description: ...
Calls, member accesses, and '->' calls can follow a literal, as in
ECMAScript: [1, 2][0], "abc"[1], {"a": 1}.a (#602). A missing comma between
call arguments is a parse error.
//? sourceType: script
//?
//? test: member-access
//? description: '[ ]', '.', '?.', and '?.[ ]' after array, object, string, and template literals
//? expect: 0
//? source: ...

assert([1, 2][0] === 1);
assert([[1, 2], [3]][0][1] === 2);
assert("abc"[1] === "b");
assert(`ab`[1] === "b");
assert({"a": 1}.a === 1);
assert({a: {b: [7, 8]}}.a.b[1] === 8);
assert({a: 1}["a"] === 1);
assert([1, 2]?.[5] === undefined);
assert({a: null}.a?.b === undefined);
assert(-[3, 4][0] === -3);
assert(![true][0] === false);
assert((true ? [1][0] : 2) === 1);
return 0;

//?
//? test: calls
//? description: Calls and '->' calls after a literal
//? expect: 0
//? source: ...

assert("abc"->length() === 3);
assert([3, 1, 2]->reverse()[0] === 2);
assert({f: function (n) { return n * 2; }}.f(4) === 8);
assert([function () { return 5; }][0]() === 5);
assert(length([1, 2][0]->to_string()) === 1);
return 0;

//?
//? test: number-literals
//? description: After a number, a '.' right after the digits is the decimal point, as in ECMAScript
//? expect: 0
//? source: ...

function message(src: string): string {
    try {
        compile<script>(script(src));
    } catch (e) {
        return e.message;
    }
    return "compiled";
}
assert(includes(message("return 1.x;"), "Parse error"), message("return 1.x;"));
assert(message("return 1..x;") === "compiled");
assert(message("return (1).x;") === "compiled");
assert(message("return 1.5->to_string();") === "compiled");
assert(1.5->to_string() === "1.5E0");
return 0;

//?
//? test: missing-comma
//? description: A missing comma between call arguments is a parse error
//? expect: 0
//? source: ...

function message(src: string): string {
    try {
        compile<script>(script(src));
    } catch (e) {
        return e.message;
    }
    return "compiled";
}
assert(includes(message("return concat(\"a\" \"b\");"), "Expecting ',' or ')'"));
// With a space, [1, 2] [0] is still an index, as in ECMAScript.
assert(message("return length([1, 2] [0]->to_string());") === "compiled");
assert(includes(message("return add(1 2);"), "Expecting ',' or ')'"));
assert(includes(message("return add(1, 2 3);"), "Expecting ',' or ')'"));
assert(includes(message("const f = function (a, b) { return a; }; return f(1 ...[2]);"),
    "Expecting ',' or ')'"));
assert(message("return add(1, 2,);") === "compiled");
assert(message("return add(1, ...[2]);") === "compiled");
assert(length([1, 2][0]->to_string()) === 1);
return 0;

//?
//? test: decompile-round-trip
//? description: A literal with a chain decompiles to text that recompiles and evaluates the same
//? expect: true
//? source: ...

function round_trip(src: string): boolean {
    const d1 = decompile(compile<script>(script(src)));
    const d2 = decompile(compile<script>(script(d1)));
    assert(d1 === d2, "fixed point: " + d1 + " vs " + d2);
    return stringify(evaluate(compile<script>(script(src)))) ===
        stringify(evaluate(compile<script>(script(d1))));
}
assert(round_trip("return [1, 2][0];"));
assert(round_trip("return \"abc\"[1];"));
assert(round_trip("return {\"a\": 1}.a;"));
assert(round_trip("return [[1, 2], [3]][0][1];"));
assert(round_trip("return \"abc\"->length();"));
return true;

//?
//? test: messages
//? description: Errors from member access name the key and the data types
//? expect: 0
//? source: ...

function message(f): string {
    try {
        f();
    } catch (e) {
        return e.message;
    }
    return "no error";
}
assert(message(function () { return 1..x; }) ===
    'Can not get "x" of double: expecting an object, array, or string');
assert(message(function () { return [1, 2, 3].length; }) ===
    'Index into an array must be an integer, not string "length"');
assert(message(function () { return "abc"["x"]; }) ===
    'Index into string must be an integer, not string "x"');
assert(message(function () { return null.x; }) ===
    'Can not get "x" of null: expecting an object, array, or string');
assert(message(function () { return true[0]; }) ===
    'Can not get 0 of boolean: expecting an object, array, or string');
assert(message(function () { return meta(1..x); }) ===
    'Can not get the meta of "x" of double: expecting an object or array');
return 0;
