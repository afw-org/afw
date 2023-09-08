#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: greater-than.as
//? customPurpose: Part of test262
//? description: expressions/greater-than
//? sourceType: script
//?
//? test: 11.8.2_A1
//? description: White Space and Line Terminator between relational expression and ">" are allowed
//? expect: undefined
//? source: ...

if (evaluate(script("0\u0009>\u00091")) !== false) {
    throw "0\u0009>\u00091 !== false";
}

if (evaluate(script("0\u000B>\u000B1")) !== false) {
    throw "0\u000B>\u000B1 !== false";
}

if (evaluate(script("0\u000C>\u000C1")) !== false) {
    throw "0\u000C>\u000C1 !== false";
}

if (evaluate(script("0\u0020>\u00201")) !== false) {
    throw "0\u0020>\u00201 !== false";
}

if (evaluate(script("0\u00A0>\u00A01")) !== false) {
    throw "0\u00A0>\u00A01 !== false";
}

if (evaluate(script("0\u000A>\u000A1")) !== false) {
    throw "0\u000A>\u000A1 !== false";
}

if (evaluate(script("0\u000D>\u000D1")) !== false) {
    throw "0\u000D>\u000D1 !== false";
}

if (evaluate(script("0\u2028>\u20281")) !== false) {
    throw "0\u2028>\u20281 !== false";
}

if (evaluate(script("0\u2029>\u20291")) !== false) {
    throw "0\u2029>\u20291 !== false";
}

if (evaluate(script("1\u0009\u000B\u000C\u0020\u00A0\u000A\u000D\u2028\u2029>=\u0009\u000B\u000C\u0020\u00A0\u000A\u000D\u2028\u20291")) !== true) {
    throw "1\u0009\u000B\u000C\u0020\u00A0\u000A\u000D\u2028\u2029>=\u0009\u000B\u000C\u0020\u00A0\u000A\u000D\u2028\u20291 !== true";
}



//? test: 11.8.2_A2.1_T1
//? description: Operator x > y
//? expect: undefined
//? source: ...

if (2 > 1 !== true) {
    throw "2 > 1 !== true";
}

let x: integer = 2;
if (x > 1 !== true) {
    throw "x > 1 !== true";
}

let y: integer = 1;
if (2 > y !== true) {
    throw "2 > y !== true";
}

x = 2;
y = 1;
if (x > 1 !== true) {
    throw "x > 1 !== true";
}



//? test: 11.8.2_A2.1_T2
//? description: x reference error
//? expect: error
//? source: ...

x > 1;

//? test: 11.8.2_A2.1_T3
//? description: y reference error
//? expect: error
//? source: ...

1 > y;

//? test: 11.8.2_A2.4_T1
//? description: first expression is evaluated first, then second expression: checking with "="
//? expect: undefined
//? skip: true
//? source: ...

let x: integer = 0;
if ((x = 1) > x !== false) {
    throw "(x = 1) > x !== false";
}

x = 1;
if (x > (x = 0) !== true) {
    throw "x > (x = 0) !== true";
}



//? test: 11.8.2_A2.4_T2
//? description: first expression is evaluated first, then second expression: checking with assert
//? expect: undefined
//? source: ...

let x1: integer = 0;
let y1: integer = 0;

function x(): any {
    x1 = 1;
    throw "x";
}

function y(): any {
    y1 = 1;
    throw "y";
}

safe_evaluate(
    x() > y(),
    "error"
);

assert(x1 === 1, "x1 !== 1");
assert(y1 === 0, "y1 !== 0");



//? test: 11.8.2_A3.1_T1.1
//? description: compare booleans
//? expect: undefined
//? source: ...

if (true > true !== false) {
    throw "true > true !== false";
}



//? test: 11.8.2_A3.1_T1.2
//? description: compare integers
//? expect: undefined
//? source: ...

if (1 > 1 !== false) {
    throw "1 > 1 !== false";
}



//? test: 11.8.2_A3.1_T2.1
//? description: compare integers and booleans
//? expect: error
//? source: ...

if (true > 1 !== false) {
    throw "true > 1 !== false";
}

if (1 > true !== false) {
    throw "1 > true !== false";
}



//? test: 11.8.2_A3.1_T2.2
//? description: compare strings and integers
//? expect: error
//? source: ...

/* mixing types won't work in AFW */
if ("1" > 1 !== false) {
    throw "'1' > 1 !== false";
}

if (1 > "1" !== false) {
    throw "1 > '1' !== false";
}

if ("x" > 1 !== false) {
    throw "'x' > 1 !== false";
}

if (1 > "x" !== false) {
    throw "1 > 'x' !== false";
}




//? test: 11.8.2_A3.2_T1.1
//? description: compare strings
//? expect: undefined
//? source: ...

if ("1" > "1" !== false) {
    throw "'1' > '1' !== false";
}

if ("x" > "1" !== true) {
    throw "'x' > '1' !== true";
}

if ("1" > "x" !== false) {
    throw "'1' > 'x' !== false";
}



//? test: 11.8.2_A4.1
//? description: If x is NaN, return false
//? expect: undefined
//? source: ...

if ((NaN > 0.0) !== false) {
    throw "NaN > 0.0 !== false";
}

if ((NaN > 1.1) !== false) {
    throw "NaN > 1.1 !== false";
}

if ((NaN > -1.1) !== false) {
    throw "NaN > -1.1 !== false";
}

if ((NaN > NaN) !== false) {
    throw "NaN > NaN !== false";
}

if ((NaN > +Infinity) !== false) {
    throw "NaN > +Infinity !== false";
}

if ((NaN > -Infinity) !== false) {
    throw "NaN > -Infinity !== false";
}



//? test: 11.8.2_A4.10
//? description: If x is a prefix of y, return false
//? expect: undefined
//? source: ...

if (("x" > "x") !== false) {
    throw "'x' > 'x' !== false";
}

if (("" > "x") !== false) {
    throw "'' > 'x' !== false";
}

if (("ab" > "abcd") !== false) {
    throw "'ab' > 'abcd' !== false";
}

if (("abcd" > "abc\u0064") !== false) {
    throw "'abcd' > 'abc\\u0064' !== false";
}

if (("x" > ("x" + "y")) !== false) {
    throw "'x' > ('x' + 'y') !== false";
}

let x: string = "x";
if ((x > (x + "y")) !== false) {
    throw "x > (x + 'y') !== false";
}



//? test: 11.8.2_A4.11
//? description: If y is a prefix of x and x !== y, return true
//? expect: undefined
//? source: ...

if (("x " > "x") !== true) {
    throw "'x ' > 'x' !== true";
}

if (("x" > "" ) !== true) {
    throw "'x' > '' !== true";
}

if (("abcd" > "ab") !== true) {
    throw "'abcd' > 'ab' !== true";
}

if (("abc\u0064" > "abcd") !== false) {
    throw "'abc\\u0064' > 'abcd' !== false";
}

if ((("x" + "y") > "x") !== true) {
    throw "('x' + 'y') > 'x' !== true";
}

let x: string = "x";
if (((x + "y") > x) !== true) {
    throw "('x' + 'y') > x !== true";
}

if (("a\u0000a" > "a\u0000") !== true) {
    throw "'a\\u0000a' > 'a\\u0000' !== true";
}

if ((" x" > "x") !== false) {
    throw "' x' > 'x' !== false";
}



//? test: 11.8.2_A4.12_T1
//? description: If neither x, nor y is a prefix of each other, returned result of strings comparison applies a simple lexicographic ordering to the sequence of code point value values
//? expect: undefined
//? source: ...

if (("xy" > "xx") !== true) {
    throw "'xy' > 'xx' !== true";
}

if (("xx" > "xy") !== false) {
    throw "'xx' > 'xy' !== false";
}

if (("y" > "x") !== true) {
    throw "'y' > 'x' !== true";
}

if (("aba" > "aab") !== true) {
    throw "'aba' > 'aab' !== true";
}

if (("\u0061\u0061\u0061\u0061" > "\u0061\u0061\u0061\u0062") !== false) {
    throw "'\\u0061\\u0061\\u0061\\u0061' > '\\u0061\\u0061\\u0061\\u0062' !== false";
}

if (("a\u0000b" > "a\u0000a") !== true) {
    throw "'a\\u0000b' > 'a\\u0000a' !== true";
}

if (("aa" > "aB") !== true) {
    throw "'aa' > 'aB' !== true";
}



//? test: 11.8.2_A4.12_T2
//? description: If neither x, nor y is a prefix of each other, returned result of strings comparison applies a simple lexicographic ordering to the sequence of code point value values
//? expect: undefined
//? source: ...

if (("x" > "0") !== true) {
    throw "'x' > '0' !== true";
}

if (("0" > "-0") !== true) {
    throw "'0' > '-0' !== true";
}

if (("-" > "+") !== true) {
    throw "'-' > '+' !== true";
}

if (("-1" > "-0") !== true) {
    throw "'-1' > '-0' !== true";
}

if (("-1" > "+1") !== true) {
    throw "'-1' > '+1' !== true";
}

if (("1e-10" > "1") !== true) {
    throw "'1e-10' > '1' !== true";
}



//? test: 11.8.2_A4.2
//? description: If y is NaN, return false (if result in 11.8.5 is undefined, return false)
//? expect: undefined
//? source: ...

if ((double(0) > NaN) !== false) {
    throw "0 > NaN !== false";
}

if ((1.1 > NaN) !== false) {
    throw "1.1 > NaN !== false";
}

if ((-1.1 > NaN) !== false) {
    throw "'-1.1' > NaN !== false";
}

if ((NaN > NaN) !== false) {
    throw "NaN > NaN !== false";
}

if ((+Infinity > NaN) !== false) {
    throw "'+Infinity' > NaN !== false";
}

if ((-Infinity > NaN) !== false) {
    throw "'-Infinity' > NaN !== false";
}



//? test: 11.8.2_A4.3
//? description: info: If x and y are the same number value, return false
//? expect: undefined
//? source: ...

if ((1 > 1) !== false) {
    throw "1 > 1 !== false";
}

if ((1.1 > 1.1) !== false) {
    throw "1.1 > 1.1 !== false";
}

if ((-1.1 > -1.1) !== false) {
    throw "'-1.1' > -1.1 !== false";
}

if ((-Infinity > -Infinity) !== false) {
    throw "'-Infinity' > -Infinity !== false";
}

if ((+Infinity > +Infinity) !== false) {
    throw "'+Infinity' > +Infinity !== false";
}



//? test: 11.8.2_A4.4
//? description: If either variable x or y is +0 and the other is -0, return false
//? expect: undefined
//? source: ...

if ((0 > 0) !== false) {
    throw "0 > 0 !== false";
}

if ((-0 > 0) !== false) {
    throw "'-0' > 0 !== false";
}

if ((+0 > -0) !== false) {
    throw "'+0' > '-0' !== false";
}

if ((-0 > +0) !== false) {
    throw "'-0' > '+0' !== false";
}




//? test: 11.8.2_A4.5
//? description: If x is +Infinity and x !== y, return true
//? expect: undefined
//? source: ...

if ((+Infinity > double(0)) !== true) {
    throw "'+Infinity' > 0 !== true";
}

if ((+Infinity > 1.1) !== true) {
    throw "'+Infinity' > 1.1 !== true";
}

if ((+Infinity > -1.1) !== true) {
    throw "'+Infinity' > -1.1 !== true";
}

if ((+Infinity > -Infinity) !== true) {
    throw "'+Infinity' > -Infinity !== true";
}



//? test: 11.8.2_A4.6
//? description: If y is +Infinity, return false
//? expect: undefined
//? source: ...

if ((double(0) > +Infinity) !== false) {
    throw "0 > '+Infinity' !== false";
}

if ((1.1 > +Infinity) !== false) {
    throw "1.1 > '+Infinity' !== false";
}

if ((-1.1 > +Infinity) !== false) {
    throw "'-1.1' > '+Infinity' !== false";
}

if ((-Infinity > +Infinity) !== false) {
    throw "'-Infinity' > '+Infinity' !== false";
}



//? test: 11.8.2_A4.7
//? description: If x is -Infinity, return false
//? expect: undefined
//? source: ...

if ((-Infinity > 0.0) !== false) {
    throw "'-Infinity' > 0.0 !== false";
}

if ((-Infinity > 1.1) !== false) {
    throw "'-Infinity' > 1.1 !== false";
}

if ((-Infinity > -1.1) !== false) {
    throw "'-Infinity' > -1.1 !== false";
}

if ((-Infinity > +Infinity) !== false) {
    throw "'-Infinity' > '+Infinity' !== false";
}



//? test: 11.8.2_A4.8
//? description: If y is -Infinity and x !== y, return true
//? expect: undefined
//? source: ...

if ((0.0 > -Infinity) !== true) {
    throw "0.0 > '-Infinity' !== true";
}

if ((1.1 > -Infinity) !== true) {
    throw "1.1 > '-Infinity' !== true";
}

if ((-1.1 > -Infinity) !== true) {
    throw "'-1.1' > '-Infinity' !== true";
}

if ((+Infinity > -Infinity) !== true) {
    throw "'+Infinity' > '-Infinity' !== true";
}



//? test: 11.8.2_A4.9
//? description: If is x greater than y and these values are both finite non-zero, return true; otherwise, return false
//? expect: undefined
//? source: ...

if ((double(1) > 1.1) !== false) {
    throw "1 > 1.1 !== false";
}

if ((1.1 > double(1)) !== true) {
    throw "1.1 > 1 !== true";
}

if ((double(-1) > -1.1) !== true) {
    throw "'-1' > '-1.1' !== true";
}

if ((-1.1 > double(-1)) !== false) {
    throw "'-1.1' > '-1' !== false";
}

if ((0.1 > double(0)) !== true) {
    throw "0.1 > 0 !== true";
}

if ((double(0) > -0.1) !== true) {
    throw "0 > '-0.1' !== true";
}

