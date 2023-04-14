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
    assert(false, "1 == 1");
}

loc x: integer = 1;
if ((x == 1) !== true) {
    assert(false, "x == 1");
}

loc y: integer = 1;
if ((1 == y) !== true) {
    assert(false, "1 == y");
}

x = 1;
y = 1;
if ((x == y) !== true) {
    assert(false, "x == y");
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
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

loc x: integer = 0;
if (((x = 1) == x) !== true) {
    assert(false, "(x = 1) == x");
}

x = 0;
if ((x == (x = 1)) !== false) {
    assert(false, "x == (x = 1)");
}



//? test: 11.9.1_A2.4_T2
//? description: First expression is evaluated first, and then second expression: checking with assert()
//? expect: null
//? source: ...
#!/usr/bin/env afw

loc x1: integer = 0;
loc y1: integer = 0;

function x(): any {
    x1 = 1;
    assert(false, "x");
}

function y(): any {
    y1 = 1;
    assert(false, "y");
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
    assert(false, "true == true");
}

if ((false == false) !== true) {
    assert(false, "false == false");
}

if ((true == false) !== false) {
    assert(false, "true == false");
}

if ((false == true) !== false) {
    assert(false, "false == true");
}



//? test: 11.9.1_A3.2
//? description: x is primitive boolean, y is primitive number
//? expect: null
//? source: ...
#!/usr/bin/env afw

if ((true == 1) !== true) {
    assert(false, "true == 1");
}

/*
if ((false == "0") !== true) {
    assert(false, "false == \"0\"");
}
*/



//? test: equals-6
//? description: x is primitive integer, y is primitive boolean
//? expect: null
//? source: ...
#!/usr/bin/env afw

/*
if ((0 == false) !== true) {
    assert(false, "true == 1");
}

if (("1" == true) !== true) {
    assert(false, "false == \"0\"");
}
*/



//? test: 11.9.1_A4.1_T1
//? description: If x or y is NaN, return false
//? expect: null
//? source: ...
#!/usr/bin/env afw

if ((true == NaN) !== false) {
    assert(false, "true == NaN");
}

if ((-1 == NaN) !== false) {
    assert(false, "-1 === NaN");
}

if ((NaN == NaN) !== false) {
    assert(false, "NaN == NaN");
}

if ((+Infinity == NaN) !== false) {
    assert(false, "+Infinity == NaN");
}

if ((-Infinity == NaN) !== false) {
    assert(false, "-Infinity == NaN");
}

if (("string" == NaN) !== false) {
    assert(false, "\"string\" == NaN");
}



//? test: 11.9.1_A4.2
//? description: If x is +0(-0) and y is -0(+0), return true
//? expect: null
//? source: ...
#!/usr/bin/env afw

if ((+0 == -0) !== true) {
    assert(false, "+0 == -0");
}

if ((-0 == +0) !== true) {
    assert(false, "-0 == +0");
}



//? test: 11.9.1_A4.3
//? description: Return true, if x is the same number value as y; otherwise, return false
//? expect: null
//? source: ...
#!/usr/bin/env afw

if ((+Infinity == +Infinity) !== true) {
    assert(false, "+Infinity == +Infinity");
}

if ((-Infinity == -Infinity) !== true) {
    assert(false, "-Infinity == -Infinity");
}

const negativeInfinity: double = -Infinity;
if ((+Infinity == -negativeInfinity) !== true) {
    assert(false, "+Infinity == -Infinity");
}

if ((1.0 == 1) !== true) {
    assert(false, "1.0 == 1");
}




//? test: 11.9.1_A5.1
//? description: Return true, if x and y are exactly the same sequence of characters; otherwise, return false
//? expect: null
//? source: ...
#!/usr/bin/env afw

if (("" == "") !== true) {
    assert(false, "\"\" == \"\"");
}

if ((" " == " ") !== true) {
    assert(false, "\" \" == \" \"");
}

if (("string" == "string") !== true) {
    assert(false, "\"string\" == \"string\"");
}

if ((" string" == "string ") !== false) {
    assert(false, "\" string\" == \"string \"");
}

if (("1.0" == "1") !== false) {
    assert(false, "\"1.0\" == \"1\"");
}

if (("0xff" == "255") !== false) {
    assert(false, "\"0xff\" == \"255\"");
}



//? test: 11.9.1_A5.2
//? description: x is primitive number, y is primitive string
//? expect: null
//? source: ...
#!/usr/bin/env afw

if ((1 == "1" ) !== true) {
    assert(false, "1 == \"1\"");
}

if ((1.100 == "+1.10") !== true) {
    assert(false, "1.100 == \"+1.10\"");
}

/*
if ((1 == "true") !== false) {
    assert(false, "1 == \"true\"");
}

if ((255 == "0xff") !== true) {
    assert(false, "255 == \"0xff\"");
}

if ((0 == "") !== true) {
    assert(false, "0 == \"\"");
}

*/




//? test: 11.9.1_A5.3
//? description: x is primitive string, y is primitive number
//? expect: null
//? source: ...
#!/usr/bin/env afw

if (("-1" == -1) !== true) {
    assert(false, "\"-1\" == -1");
}

/*
if (("-1.100" == -1.10) !== true) {
    assert(false, "\"-1.100\" == -1.10");
}
*/

if (("false" == 0) !== false) {
    assert(false, "\"false\" == 0");
}

/*
if (("5e-324" == 5e-234) !== true) {
    assert(false, "\"5e-324\" == 5e-234");
}
*/



//? test: 11.9.1_A6.1
//? description: combinations of undefined and null
//? expect: null
//? source: ...
#!/usr/bin/env afw

if ((undefined == undefined) !== true) {
    assert(false, "undefined == undefined");
}

if ((undefined == null) !== true) {
    assert(false, "undefined == null");
}

if ((null == null) !== true) {
    assert(false, "null == null");
}



//? test: 11.9.1_A6.2_T1
//? description: x is undefined or null, y is not
//? expect: null
//? source: ...
#!/usr/bin/env afw

if ((undefined == true) !== false) {
    assert(false, "undefined == true");
}

if ((undefined == 0) !== false) {
    assert(false, "undefined == 0");
}

if ((undefined == "undefined") !== false) {
    assert(false, "undefined == \"undefined\"");
}

if ((undefined == {}) !== false) {
    assert(false, "undefined == {}");
}

if ((null == false) !== false) {
    assert(false, "null == false");
}

if ((null == 0) !== false) {
    assert(false, "null == 0");
}

if ((null == "null") !== false) {
    assert(false, "null == \"null\"");
}

if ((null == {}) !== false) {
    assert(false, "null == {}");
}



//? test: 11.9.1_A6.2_T2
//? description: y is null or undefined, x is not
//? expect: null
//? source: ...
#!/usr/bin/env afw

if ((false == undefined) !== false) {
    assert(false, "false == undefined");
}

if ((NaN == undefined) !== false) {
    assert(false, "NaN == undefined");
}

if (("undefined" == undefined) !== false) {
    assert(false, "\"undefined\" == undefined");
}

if (({} == undefined) !== false) {
    assert(false, "{} == undefined");
}

if ((false == null) !== false) {
    assert(false, "false == null");
}

if ((0 == null) !== false) {
    assert(false, "0 == null");
}

if (("null" == null) !== false) {
    assert(false, "\"null\" == null");
}

if (({} == null) !== false) {
    assert(false, "{} == null");
}

