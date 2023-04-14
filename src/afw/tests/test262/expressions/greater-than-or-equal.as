#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: greater-than-or-equal.as
//? customPurpose: Part of test262
//? description: expressions/greater-than-or-equal
//? sourceType: script
//?
//? test: 11.8.4_A2.1_T1
//? description: Operator x >= y
//? expect: null
//? source: ...
#!/usr/bin/env afw

if (1 >= 1 !== true) {
    assert(false, "1 >= 1 === true");
}

loc x: integer = 1;
if (x >= 1 !== true) {
    assert(false, "x >= 1 === true");
}

loc y: integer = 1;
if (1 >= y !== true) {
    assert(false, "1 >= y === true");
}

x = 1;
y = 1;
if (x >= y !== true) {
    assert(false, "x >= y === true");
}



//? test: 11.8.4_A2.1_T2
//? description: Undeclared variable
//? expect: error
//? source: ...
#!/usr/bin/env afw

x >= 1;

//? test: 11.8.4_A2.1_T3
//? description: Undeclared variable 2
//? expect: error
//? source: ...
#!/usr/bin/env afw

1 >= y;

//? test: 11.8.4_A2.4_T1
//? description: First expression is evaluated first, and then the second epxression
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

loc x: integer = 1;
if (((x = 0) >= x !== true) {
    assert(false, "x >= x === true");
}

x = 0;
if (x >= (x = 1) !== false) {
    assert(false, "x >= x === false");
}



//? test: 11.8.4_A2.4_T4
//? description: Checking with undeclared variables
//? expect: error
//? source: ...
#!/usr/bin/env afw

if ((y = 1) >= y !== true) {
    assert(false, "y >= y === true");
}

//? test: 11.8.4_A3.1_T1.1
//? description: Compare x >= y (boolean)
//? expect: null
//? source: ...
#!/usr/bin/env afw

if (true >= true !== true) {
    assert(false, "true >= true === true");
}



//? test: 11.8.4_A3.1_T1.2
//? description: Compare x >= y (integer)
//? expect: null
//? source: ...
#!/usr/bin/env afw

if (1 >= 1 !== true) {
    assert(false, "1 >= 1 === true");
}




//? test: 11.8.4_A3.1_T2.2
//? description: Compare x >= y (string, integer)
//? expect: error
//? source: ...
#!/usr/bin/env afw

/* can't mix types in AFW */
if ("1" >= 1 !== true) {
    assert(false, "'1' >= 1 === true");
}

if (1 >= "1" !== true) {
    assert(false, "1 >= '1' === true");
}

if ("x" >= 1 !== false) {
    assert(false, "'x' >= '1' === true");
}

if (1 >= "x" !== false) {
    assert(false, "1 >= 'x' === true");
}



//? test: 11.8.4_A3.1_T2.5
//? description: Compare x >= y (boolean, string)
//? expect: error
//? source: ...
#!/usr/bin/env afw

/* can't mix types in AFW */
if (true >= "1" !== true) {
    assert(false, "true >= '1' === true");
}

if ("1" >= true !== true) {
    assert(false, "'1' >= true === true");
}



//? test: 11.8.4_A3.1_T2.7
//? description: Compare x >= y (null, string)
//? expect: error
//? source: ...
#!/usr/bin/env afw

/* can't mix types in AFW */
if ("1" >= null !== true) {
    assert(false, "'1' >= null === true");
}

if (null >= "1" !== false) {
    assert(false, "null >= '1' === false");
}



//? test: 11.8.4_A3.2_T1.1
//? description: Compare x >= y (string, string)
//? expect: null
//? source: ...
#!/usr/bin/env afw

if ("1" >= "1" !== true) {
    assert(false, "'1' >= '1' === true");
}



//? test: 11.8.4_A4.1
//? description: If x is NaN, return false
//? expect: null
//? source: ...
#!/usr/bin/env afw

if ((NaN >= 0.0) !== false) {
    assert(false, "NaN >= 0.0 === false");
}

if ((NaN >= 1.1) !== false) {
    assert(false, "NaN >= 1.1 === false");
}

if ((NaN >= -1.1) !== false) {
    assert(false, "NaN >= -1.1 === false");
}

if ((NaN >= NaN) !== false) {
    assert(false, "NaN >= NaN === false");
}

if ((NaN >= +Infinity) !== false) {
    assert(false, "NaN >= +Infinity === false");
}

if ((NaN >= -Infinity) !== false) {
    assert(false, "NaN >= -Infinity === false");
}




//? test: 11.8.4_A4.10
//? description: If x is a prefix of y and x !== y, return false
//? expect: null
//? source: ...
#!/usr/bin/env afw

if (("x" >= "x ") !== false) {
    assert(false, "'x' >= 'x ' === false");
}

if (("" >= "x") !== false) {
    assert(false, "'' >= 'x' === false");
}

if (("ab" >= "abcd") !== false) {
    assert(false, "'ab' >= 'abcd' === false");
}

if (("abcd" >= "abc\u0064") !== true) {
    assert(false, "'abcd' >= 'abc\\u0064' === false");
}

if (("x" >= ("x" + "y")) !== false) {
    assert(false, "'x' >= 'x' + 'y' === false");
}

loc x: string = "x";
if ((x >= (x + "y")) !== false) {
    assert(false, "x >= x + 'y' === false");
}



//? test: 11.8.4_A4.11
//? description: If y is a prefix of x, return true
//? expect: null
//? source: ...
#!/usr/bin/env afw

if (("x" >= "x") !== true) {
    assert(false, "'x' >= 'x ' === true");
}

if (("x" >= "") !== true) {
    assert(false, "'x' >= '' === true");
}

if (("abcd" >= "abc") !== true) {
    assert(false, "'abcd' >= 'ab' === true");
}

if (("abc\u0064" >= "abcd") !== true) {
    assert(false, "'abc\\u0064' >= 'abcd' === true");
}

if ((("x" + "y") >= "x") !== true) {
    assert(false, "'x' + 'y' >= 'x' === true");
}

loc x: string = "x";
if (((x + "y") >= x) !== true) {
    assert(false, "x + 'y' >= x === true");
}

if (("a\u0000a" >= "a\u0000") !== true) {
    assert(false, "'a\\u0000a' >= 'a\\u0000' === true");
}

if ((" x" >= "x") !== false) {
    assert(false, "' x' >= 'x' === false");
}



//? test: 11.8.4_A4.12_T1
//? description: If neither x, nor y is a prefix of each other, return result of strings comparison applies a simple lexicographic ordering to the sequences of code point value values
//? expect: null
//? source: ...
#!/usr/bin/env afw

if (("xy" >= "xx") !== true) {
    assert(false, "'xy' >= 'xx' === true");
}

if (("xx" >= "xy") !== false) {
    assert(false, "'xx' >= 'xy' === false");
}

if (("y" >= "x") !== true) {
    assert(false, "'y' >= 'x' === true");
}

if (("aba" >= "aab") !== true) {
    assert(false, "'aba' >= 'aab' === true");
}

if (("\u0061\u0061\u0061\u0061" >= "\u0061\u0061\u0061\u0062") !== false) {
    assert(false, "'\\u0061\\u0061\\u0061\\u0061' >= '\\u0061\\u0061\\u0061\\u0062' === false");
}

if (("a\u0000b" >= "a\u0000a") !== true) {
    assert(false, "'a\\u0000b' >= 'a\\u0000a' === true");
}

if (("aa" >= "aB") !== true) {
    assert(false, "'aa' >= 'aB' === true");
}



//? test: 11.8.4_A4.12_T2
//? description: If neither x, nor y is a prefix of each other, return result of strings comparison applies a simple lexicographic ordering to the sequences of code point value values
//? expect: null
//? source: ...
#!/usr/bin/env afw

if (("x" >= "0") !== true) {
    assert(false, "'x' >= '0' === true");
}

if (("0" >= "-0") !== true) {
    assert(false, "'0' >= '-0' === true");
}

if (("0" >= ".") !== true) {
    assert(false, "'0' >= '.' === true");
}

if (("-" >= "+") !== true) {
    assert(false, "'-' >= '+' === true");
}

if (("-1" >= "-0") !== true) {
    assert(false, "'-1' >= '-0' === true");
}

if (("-1" >= "+1") !== true) {
    assert(false, "'-1' >= '+1' === true");
}

if (("1e-10" >= "1") !== true) {
    assert(false, "'1e-10' >= '1' === true");
}



//? test: 11.8.4_A4.2
//? description: If y is NaN, return false
//? expect: null
//? source: ...
#!/usr/bin/env afw

if ((double(0) >= NaN) !== false) {
    assert(false, "0 >= NaN === false");
}

if ((1.1 >= NaN) !== false) {
    assert(false, "1.1 >= NaN === false");
}

if ((-1.1 >= NaN) !== false) {
    assert(false, "-1.1 >= NaN === false");
}

if ((NaN >= NaN) !== false) {
    assert(false, "NaN >= NaN === false");
}

if ((+Infinity >= NaN) !== false) {
    assert(false, "'+Infinity' >= NaN === false");
}

if ((-Infinity >= NaN) !== false) {
    assert(false, "'-Infinity' >= NaN === false");
}



//? test: 11.8.4_A4.3
//? description: If x and y are the same number value, return true
//? expect: null
//? source: ...
#!/usr/bin/env afw

if ((1 >= 1) !== true) {
    assert(false, "1 >= 1 === true");
}

if ((1.1 >= 1.1) !== true) {
    assert(false, "1.1 >= 1.1 === true");
}

if ((-1.1 >= -1.1) !== true) {
    assert(false, "-1.1 >= -1.1 === true");
}

if ((-Infinity >= -Infinity) !== true) {
    assert(false, "'-Infinity' >= '-Infinity' === true");
}

if ((+Infinity >= +Infinity) !== true) {
    assert(false, "'+Infinity' >= '+Infinity' === true");
}



//? test: 11.8.4_A4.4
//? description: If either x or y is +0 and the other is -0, return true
//? expect: null
//? source: ...
#!/usr/bin/env afw

if ((0 >= 0) !== true) {
    assert(false, "0 >= 0 === true");
}

if ((-0 >= -0) !== true) {
    assert(false, "'-0' >= '-0' === true");
}

if ((+0 >= -1) !== true) {
    assert(false, "'+0' >= '-1' === true");
}

if ((-0 >= +0) !== true) {
    assert(false, "'-0' >= '+0' === true");
}



//? test: 11.8.4_A4.5
//? description: If x is +Infinity, return true
//? expect: null
//? source: ...
#!/usr/bin/env afw

if ((+Infinity >= double(0)) !== true) {
    assert(false, "'+Infinity' >= 0 === true");
}

if ((+Infinity >= 1.1) !== true) {
    assert(false, "'+Infinity' >= 1.1 === true");
}

if ((+Infinity >= -1.1) !== true) {
    assert(false, "'+Infinity' >= -1.1 === true");
}

if ((+Infinity >= -Infinity) !== true) {
    assert(false, "'+Infinity' >= '-Infinity' === true");
}



//? test: 11.8.4_A4.6
//? description: If y is +Infinity and x !== y, return false
//? expect: null
//? source: ...
#!/usr/bin/env afw

if ((double(0) >= +Infinity) !== false) {
    assert(false, "0 >= '+Infinity' === false");
}

if ((1.1 >= +Infinity) !== false) {
    assert(false, "1.1 >= '+Infinity' === false");
}

if ((-1.1 >= +Infinity) !== false) {
    assert(false, "-1.1 >= '+Infinity' === false");
}

if ((-Infinity >= +Infinity) !== false) {
    assert(false, "'-Infinity' >= '+Infinity' === false");
}



//? test: 11.8.4_A4.7
//? description: If x is -Infinity and x !== y, return false
//? expect: null
//? source: ...
#!/usr/bin/env afw

if ((-Infinity >= 0.0) !== false) {
    assert(false, "'-Infinity' >= 0.0 === false");
}

if ((-Infinity >= 1.1) !== false) {
    assert(false, "'-Infinity' >= 1.1 === false");
}

if ((-Infinity >= -1.1) !== false) {
    assert(false, "'-Infinity' >= -1.1 === false");
}

if ((-Infinity >= +Infinity) !== false) {
    assert(false, "'-Infinity' >= '+Infinity' === false");
}



//? test: 11.8.4_A4.8
//? description: If y is -Infinity, return true
//? expect: null
//? source: ...
#!/usr/bin/env afw

if ((0.0 >= -Infinity) !== true) {
    assert(false, "0.0 >= '-Infinity' === true");
}

if ((1.1 >= -Infinity) !== true) {
    assert(false, "1.1 >= '-Infinity' === true");
}

if ((-1.1 >= -Infinity) !== true) {
    assert(false, "-1.1 >= '-Infinity' === true");
}

if ((+Infinity >= -Infinity) !== true) {
    assert(false, "'+Infinity' >= '-Infinity' === true");
}



//? test: 11.8.4_A4.9
//? description: If x is greater or equal than y and these values are both finite non-zero, return true; otherwise, return false
//? expect: null
//? source: ...
#!/usr/bin/env afw

if ((double(1) >= 1.1) !== false) {
    assert(false, "1 >= 1.1 === false");
}

if ((1.1 >= 1.0) !== true) {
    assert(false, "1.1 >= 1.0 === true");
}

if ((-1.0 >= -1.1) !== true) {
    assert(false, "-1.0 >= -1.1 === true");
}

if ((-1.1 >= -1.0) !== false) {
    assert(false, "-1.1 >= -1.0 === false");
}

if ((0.1 >= 0.0) !== true) {
    assert(false, "0.1 >= 0.0 === true");
}

if ((0.0 >= -0.1) !== true) {
    assert(false, "0.0 >= -0.1 === true");
}

