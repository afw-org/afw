#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: equals.as
//? customPurpose: Part of test262
//? description: Tests equals expressions
//? sourceType: script
//?
//? test: 11.9.1_A2.1_T1
//? description: Operator x == y
//? expect: null
//? source: ...
#!/usr/bin/env afw

if ((1 == 1) !== true) {
    throw "1 == 1";
}

let x: integer = 1;
if ((x == 1) !== true) {
    throw "x == 1";
}

let y: integer = 1;
if ((1 == y) !== true) {
    throw "1 == y";
}

x = 1;
y = 1;
if ((x == y) !== true) {
    throw "x == y";
}



//? test: 11.9.1_A2.1_T2
//? description: Reference error
//? expect: error
//? source: ...
#!/usr/bin/env afw

return x == 1;

//? test: 11.9.1_A2.1_T3
//? description: Reference error
//? expect: error
//? source: ...
#!/usr/bin/env afw

return 1 == y;

//? test: 11.9.1_A2.1_T3
//? description: Reference error
//? expect: error:Parse error at offset 90 around line 5 column 9: Expecting ')'
//? source: ...
#!/usr/bin/env afw

let x: integer = 0;
// can't use assignment inside expression
if (((x = 1) == x) !== true) {
    throw "(x = 1) == x";
}

x = 0;
if ((x == (x = 1)) !== false) {
    throw "x == (x = 1)";
}



//? test: 11.9.1_A2.4_T2
//? description: First expression is evaluated first, and then second expression: checking with assert()
//? expect: null
//? source: ...
#!/usr/bin/env afw

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

assert(
    safe_evaluate(
        x() == y(),
        "error"
    ) === "error", "safe_evaluate(x() == y(), \"error\")"
);




//? test: 11.9.1_A3.1
//? description: Return true, if x and y are both true or both false; otherwise, return false
//? expect: null
//? source: ...
#!/usr/bin/env afw

if ((true == true) !== true) {
    throw "true == true";
}

if ((false == false) !== true) {
    throw "false == false";
}

if ((true == false) !== false) {
    throw "true == false";
}

if ((false == true) !== false) {
    throw "false == true";
}



//? test: 11.9.1_A3.2
//? description: x is primitive boolean, y is primitive number
//? expect: null
//? source: ...
#!/usr/bin/env afw

if ((true == 1) !== true) {
    throw "true == 1";
}

/*
if ((false == "0") !== true) {
    throw "false == \"0\"";
}
*/



//? test: equals-6
//? description: x is primitive integer, y is primitive boolean
//? expect: null
//? source: ...
#!/usr/bin/env afw

/*
if ((0 == false) !== true) {
    throw "true == 1";
}

if (("1" == true) !== true) {
    throw "false == \"0\"";
}
*/



//? test: 11.9.1_A4.1_T1
//? description: If x or y is NaN, return false
//? expect: null
//? source: ...
#!/usr/bin/env afw

if ((true == NaN) !== false) {
    throw "true == NaN";
}

if ((-1 == NaN) !== false) {
    throw "-1 === NaN";
}

if ((NaN == NaN) !== false) {
    throw "NaN == NaN";
}

if ((+Infinity == NaN) !== false) {
    throw "+Infinity == NaN";
}

if ((-Infinity == NaN) !== false) {
    throw "-Infinity == NaN";
}

if (("string" == NaN) !== false) {
    throw "\"string\" == NaN";
}



//? test: 11.9.1_A4.2
//? description: If x is +0(-0) and y is -0(+0), return true
//? expect: null
//? source: ...
#!/usr/bin/env afw

if ((+0 == -0) !== true) {
    throw "+0 == -0";
}

if ((-0 == +0) !== true) {
    throw "-0 == +0";
}



//? test: 11.9.1_A4.3
//? description: Return true, if x is the same number value as y; otherwise, return false
//? expect: null
//? source: ...
#!/usr/bin/env afw

if ((+Infinity == +Infinity) !== true) {
    throw "+Infinity == +Infinity";
}

if ((-Infinity == -Infinity) !== true) {
    throw "-Infinity == -Infinity";
}

const negativeInfinity: double = -Infinity;
if ((+Infinity == -negativeInfinity) !== true) {
    throw "+Infinity == -Infinity";
}

if ((1.0 == 1) !== true) {
    throw "1.0 == 1";
}




//? test: 11.9.1_A5.1
//? description: Return true, if x and y are exactly the same sequence of characters; otherwise, return false
//? expect: null
//? source: ...
#!/usr/bin/env afw

if (("" == "") !== true) {
    throw "\"\" == \"\"";
}

if ((" " == " ") !== true) {
    throw "\" \" == \" \"";
}

if (("string" == "string") !== true) {
    throw "\"string\" == \"string\"";
}

if ((" string" == "string ") !== false) {
    throw "\" string\" == \"string \"";
}

if (("1.0" == "1") !== false) {
    throw "\"1.0\" == \"1\"";
}

if (("0xff" == "255") !== false) {
    throw "\"0xff\" == \"255\"";
}



//? test: 11.9.1_A5.2
//? description: x is primitive number, y is primitive string
//? expect: null
//? source: ...
#!/usr/bin/env afw

if ((1 == "1" ) !== true) {
    throw "1 == \"1\"";
}

if ((1.100 == "+1.10") !== true) {
    throw "1.100 == \"+1.10\"";
}

/*
if ((1 == "true") !== false) {
    throw "1 == \"true\"";
}

if ((255 == "0xff") !== true) {
    throw "255 == \"0xff\"";
}

if ((0 == "") !== true) {
    throw "0 == \"\"";
}

*/




//? test: 11.9.1_A5.3
//? description: x is primitive string, y is primitive number
//? expect: null
//? source: ...
#!/usr/bin/env afw

if (("-1" == -1) !== true) {
    throw "\"-1\" == -1";
}

/*
if (("-1.100" == -1.10) !== true) {
    throw "\"-1.100\" == -1.10";
}
*/

if (("false" == 0) !== false) {
    throw "\"false\" == 0";
}

/*
if (("5e-324" == 5e-234) !== true) {
    throw "\"5e-324\" == 5e-234";
}
*/



//? test: 11.9.1_A6.1
//? description: combinations of undefined and null
//? expect: null
//? source: ...
#!/usr/bin/env afw

if ((undefined == undefined) !== true) {
    throw "undefined == undefined";
}

if ((undefined == null) !== true) {
    throw "undefined == null";
}

if ((null == null) !== true) {
    throw "null == null";
}



//? test: 11.9.1_A6.2_T1
//? description: x is undefined or null, y is not
//? expect: null
//? source: ...
#!/usr/bin/env afw

if ((undefined == true) !== false) {
    throw "undefined == true";
}

if ((undefined == 0) !== false) {
    throw "undefined == 0";
}

if ((undefined == "undefined") !== false) {
    throw "undefined == \"undefined\"";
}

if ((undefined == {}) !== false) {
    throw "undefined == {}";
}

if ((null == false) !== false) {
    throw "null == false";
}

if ((null == 0) !== false) {
    throw "null == 0";
}

if ((null == "null") !== false) {
    throw "null == \"null\"";
}

if ((null == {}) !== false) {
    throw "null == {}";
}



//? test: 11.9.1_A6.2_T2
//? description: y is null or undefined, x is not
//? expect: null
//? source: ...
#!/usr/bin/env afw

if ((false == undefined) !== false) {
    throw "false == undefined";
}

if ((NaN == undefined) !== false) {
    throw "NaN == undefined";
}

if (("undefined" == undefined) !== false) {
    throw "\"undefined\" == undefined";
}

if (({} == undefined) !== false) {
    throw "{} == undefined";
}

if ((false == null) !== false) {
    throw "false == null";
}

if ((0 == null) !== false) {
    throw "0 == null";
}

if (("null" == null) !== false) {
    throw "\"null\" == null";
}

if (({} == null) !== false) {
    throw "{} == null";
}

