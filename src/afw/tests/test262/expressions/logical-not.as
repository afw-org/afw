#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: logical-not.as
//? customPurpose: Part of test262
//? description: Tests logical-not expressions
//? sourceType: script
//?
//? test: 11.4.9_A1
//? description: White Space and Line Terminator between "!" and unary expression are allowed
//? expect: null
//? source: ...
#!/usr/bin/env afw

if (evaluate(script("!\u0009true")) !== false) {
    throw "#1 !\\u009true === false";
}

if (evaluate(script("!\u000Btrue")) !== false) {
    throw "#2 !\u000Btrue === false";
}

if (evaluate(script("!\u000Ctrue")) !== false) {
    throw "#3 !\u000Ctrue === false";
}

if (evaluate(script("!\u0020true")) !== false) {
    throw "#4 !\u0020true === false";
}

if (evaluate(script("!\u00A0true")) !== false) {
    throw "#5 !\u00A0true === false";
}

if (evaluate(script("!\u000Atrue")) !== false) {
    throw "#6 !\u000Atrue === false";
}

if (evaluate(script("!\u000Dtrue")) !== false) {
    throw "#7 !\u000Dtrue === false";
}

if (evaluate(script("!\u2028true")) !== false) {
    throw "#8 !\u2028true === false";
}

if (evaluate(script("!\u2029true")) !== false) {
    throw "#9 !\u2029true === false";
}

if (evaluate(script("!\u0009\u000B\u000C\u0020\u00A0\u000A\u000D\u2028\u2029true")) !== false) {
    throw "#10 !\\u009\\u000B\\u000C\\u0020\\u00A0\\u000A\\u000D\\u2028\\u2029true === false";
}



//? test: 11.4.9_A2.1_T1
//? description: Operator !x
//? expect: null
//? source: ...
#!/usr/bin/env afw

if (!true !== false) {
    throw "#1 !true !== false";
}

if (!(!true) !== true) {
    throw "#2 !(!true) !== true";
}

loc x: boolean = true;
if (!x !== false) {
    throw "#3 !x !== false";
}

x = true;
if (!(!x) !== true) {
    throw "#4 !(!x) !== true";
}



//? test: 11.4.9_A3_T1
//? description: x is boolean primitive
//? expect: null
//? source: ...
#!/usr/bin/env afw

if (!false !== true) {
    throw "#1 !false !== true";
}



//? test: 11.4.9_A3_T2
//? description: x is number
//? expect: error
//? source: ...
#!/usr/bin/env afw

if (!0.1 !== false) {
    throw "#1 !0.1 !== false";
}

if (!NaN !== true) {
    throw "#2 !NaN !== true";
}

if (!0 !== true) {
    throw "#3 !0 !== true";
}

if (!Infinity !== false) {
    throw "#4 !Infinity !== false";
}



//? test: 11.4.9_A4_T1
//? description: Result of boolean conversion from number value is false if the argument is +0, -0, or NaN; otherwise, is true
//? expect: error
//? source: ...
#!/usr/bin/env afw

if (!(+0) !== true) {
    throw "#1 !(+0) !== true";
}

if (!(-0) !== true) {
    throw "#2 !(-0) !== true";
}

if (!(NaN) !== true) {
    throw "#3 !(NaN) !== true";
}



//? test: 11.4.9_A4_T4
//? description: Result of boolean conversion from number value is false if the argument is +0, -0, or NaN; otherwise, is true
//? expect: error
//? source: ...
#!/usr/bin/env afw

if (!(+Infinity) !== false) {
    throw "#1 !(+Infinity) !== false";
}

if (!(-Infinity) !== false) {
    throw "#2 !(-Infinity) !== false";
}

if (!(13) !== false) {
    throw "#3 !(13) !== false";
}

if (!(-13) !== false) {
    throw "#4 !(-13) !== false";
}

if (!(1.3) !== false) {
    throw "#5 !(1.3) !== false";
}

if (!(-1.3) !== false) {
    throw "#6 !(-1.3) !== false";
}



//? test: 11.4.9_A5_T2
//? description:  Result of boolean conversion from nonempty string value (length is not zero) is true; from empty String (length is zero) is false
//? expect: error
//? source: ...
#!/usr/bin/env afw

if (!("") !== true) {
    throw "#1 !(\"\") !== true";
}

