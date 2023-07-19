#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: addition.as
//? customPurpose: Part of test262
//? description: Tests addition expressions
//? sourceType: script
//?
//? test: 11.6.1_A1
//? description: White Space and Line Terminator between AdditiveExpression and "+" or between "+" and MultiplicativeExpression are allowed
//? expect: null
//? source: ...
#!/usr/bin/env afw

//CHECK#1
if (evaluate(script("1\u0009+\u00091")) !== 2) {
  throw '#1: 1\\u0009+\\u00091 === 2';
}

//CHECK#2
if (evaluate(script("1\u000B+\u000B1")) !== 2) {
  throw '#2: 1\\u000B+\\u000B1 === 2';  
}

//CHECK#3
if (evaluate(script("1\u000C+\u000C1")) !== 2) {
  throw '#3: 1\\u000C+\\u000C1 === 2';
}

//CHECK#4
if (evaluate(script("1\u0020+\u00201")) !== 2) {
  throw '#4: 1\\u0020+\\u00201 === 2';
}

//CHECK#5
if (evaluate(script("1\u00A0+\u00A01")) !== 2) {
  throw '#5: 1\\u00A0+\\u00A01 === 2';
}

//CHECK#6
if (evaluate(script("1\u000A+\u000A1")) !== 2) {
  throw '#6: 1\\u000A+\\u000A1 === 2';  
}

//CHECK#7
if (evaluate(script("1\u000D+\u000D1")) !== 2) {
  throw '#7: 1\\u000D+\\u000D1 === 2';
}

//CHECK#8
if (evaluate(script("1\u2028+\u20281")) !== 2) {
  throw '#8: 1\\u2028+\\u20281 === 2';
}

//CHECK#9
if (evaluate(script("1\u2029+\u20291")) !== 2) {
  throw '#9: 1\\u2029+\\u20291 === 2';
}

//CHECK#10
if (evaluate(script("1\u0009\u000B\u000C\u0020\u00A0\u000A\u000D\u2028\u2029+\u0009\u000B\u000C\u0020\u00A0\u000A\u000D\u2028\u20291")) !== 2) {
  throw '#10: 1\\u0009\\u000B\\u000C\\u0020\\u00A0\\u000A\\u000D\\u2028\\u2029+\\u0009\\u000B\\u000C\\u0020\\u00A0\\u000A\\u000D\\u2028\\u20291 === 2';
}


//? test: 11.6.1_A2.1_T1
//? description: Operator x + y
//? expect: null
//? source: ...
#!/usr/bin/env afw

if (1 + 1 !== 2) {
    throw "1 + 1 !== 2";
}

let x = 1;
if (x + 1 !== 2) {
    throw "x + 1 !== 2";
}

let y = 1;
if (1 + y !== 2) {
    throw "1 + y !== 2";
}

x = 1;
y = 1;
if (x + y !== 2) {
    throw "x + y !== 2";
}


//? test: 11.6.1_A2.1_T2
//? description: If x is not declared, throw error
//? expect: error
//? source: ...
#!/usr/bin/env afw

x + 1;


//? test: 11.6.1_A2.1_T3
//? description: If y is not declared, throw error
//? expect: error
//? source: ...
#!/usr/bin/env afw

1 + y;


//? test: 11.6.1_A2.4_T1
//? description: First expression is evaluated first, and then second expression
//? expect: error:Parse error at offset 38 around line 4 column 8: Expecting ')'
//? source: ...
#!/usr/bin/env afw

let x = 0;
if ((x = 1) + x !== 2) {
    throw "x = 1 + x !== 2";
}

x = 0;
if (x = (x = 1) !== 1) {
    throw "x = (x = 1) !== 1";
}



//? test: 11.6.1_A2.4_T2
//? description: First expression is evaluated first, and then second expression: checking with assert errors
//? expect: null
//? source: ...
#!/usr/bin/env afw

let x_1 = 0;
let y_1 = 0;
function x(): any {
    x_1 = 1;
    throw "x";
}

function y(): any {
    y_1 = 1;
    throw "y";
}

safe_evaluate(
    x() + y(),
    "error"
);

assert(x_1 === 1, "x_1 === 1");
assert(y_1 === 0, "y_1 === 0");



//? test: 11.6.1_A2.4_T3
//? description: First expression is evaluated first, and then second expression: checking with undeclared variables
//? expect: error:Parse error at offset 55 around line 5 column 9: Unknown built-in function x
//? source: ...
#!/usr/bin/env afw

assert(
    safe_evaluate(
        x + (x = 1),
        "error"
    ) === "error", "safe_evaluate(x + (x = 1), \"error\") === \"error\""
);



//? test: 11.6.1_A2.4_T4
//? description: First expression is evaluated first, and then second expression: checking with undeclared variables
//? expect: error:Parse error at offset 57 around line 5 column 11: Unknown built-in function y
//? source: ...
#!/usr/bin/env afw

assert(
    safe_evaluate(
        ((y = 1) + y !== 2),
        "error"
    ) === "error", "safe_evaluate((y = 1) + y !== 2, \"error\") === \"error\""
);



