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
    assert(false, "#1 !\\u009true === false");
}

if (evaluate(script("!\u000Btrue")) !== false) {
    assert(false, "#2 !\u000Btrue === false");
}

if (evaluate(script("!\u000Ctrue")) !== false) {
    assert(false, "#3 !\u000Ctrue === false");
}

if (evaluate(script("!\u0020true")) !== false) {
    assert(false, "#4 !\u0020true === false");
}

if (evaluate(script("!\u00A0true")) !== false) {
    assert(false, "#5 !\u00A0true === false");
}

if (evaluate(script("!\u000Atrue")) !== false) {
    assert(false, "#6 !\u000Atrue === false");
}

if (evaluate(script("!\u000Dtrue")) !== false) {
    assert(false, "#7 !\u000Dtrue === false");
}

if (evaluate(script("!\u2028true")) !== false) {
    assert(false, "#8 !\u2028true === false");
}

if (evaluate(script("!\u2029true")) !== false) {
    assert(false, "#9 !\u2029true === false");
}

if (evaluate(script("!\u0009\u000B\u000C\u0020\u00A0\u000A\u000D\u2028\u2029true")) !== false) {
    assert(false, "#10 !\\u009\\u000B\\u000C\\u0020\\u00A0\\u000A\\u000D\\u2028\\u2029true === false");
}



//? test: 11.4.9_A2.1_T1
//? description: Operator !x
//? expect: null
//? source: ...
#!/usr/bin/env afw

if (!true !== false) {
    assert(false, "#1 !true !== false");
}

if (!(!true) !== true) {
    assert(false, "#2 !(!true) !== true");
}

loc x: boolean = true;
if (!x !== false) {
    assert(false, "#3 !x !== false");
}

x = true;
if (!(!x) !== true) {
    assert(false, "#4 !(!x) !== true");
}



//? test: 11.4.9_A3_T1
//? description: x is boolean primitive
//? expect: null
//? source: ...
#!/usr/bin/env afw

if (!false !== true) {
    assert(false, "#1 !false !== true");
}



//? test: 11.4.9_A3_T2
//? description: x is number
//? expect: error
//? source: ...
#!/usr/bin/env afw

if (!0.1 !== false) {
    assert(false, "#1 !0.1 !== false");
}

if (!NaN !== true) {
    assert(false, "#2 !NaN !== true");
}

if (!0 !== true) {
    assert(false, "#3 !0 !== true");
}

if (!Infinity !== false) {
    assert(false, "#4 !Infinity !== false");
}



//? test: 11.4.9_A4_T1
//? description: Result of boolean conversion from number value is false if the argument is +0, -0, or NaN; otherwise, is true
//? expect: error
//? source: ...
#!/usr/bin/env afw

if (!(+0) !== true) {
    assert(false, "#1 !(+0) !== true");
}

if (!(-0) !== true) {
    assert(false, "#2 !(-0) !== true");
}

if (!(NaN) !== true) {
    assert(false, "#3 !(NaN) !== true");
}



//? test: 11.4.9_A4_T4
//? description: Result of boolean conversion from number value is false if the argument is +0, -0, or NaN; otherwise, is true
//? expect: error
//? source: ...
#!/usr/bin/env afw

if (!(+Infinity) !== false) {
    assert(false, "#1 !(+Infinity) !== false");
}

if (!(-Infinity) !== false) {
    assert(false, "#2 !(-Infinity) !== false");
}

if (!(13) !== false) {
    assert(false, "#3 !(13) !== false");
}

if (!(-13) !== false) {
    assert(false, "#4 !(-13) !== false");
}

if (!(1.3) !== false) {
    assert(false, "#5 !(1.3) !== false");
}

if (!(-1.3) !== false) {
    assert(false, "#6 !(-1.3) !== false");
}



//? test: 11.4.9_A5_T2
//? description:  Result of boolean conversion from nonempty string value (length is not zero) is true; from empty String (length is zero) is false
//? expect: error
//? source: ...
#!/usr/bin/env afw

if (!("") !== true) {
    assert(false, "#1 !(\"\") !== true");
}

