#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: multiplication.as
//? customPurpose: Part of test262
//? description: expressions/multiplication
//? sourceType: script
//?
//? test: 11.5.1_A1
//? description: White Space and Line Terminator between multiplicative expression and "*"
//? expect: null
//? source: ...
#!/usr/bin/env afw

if (evaluate(script("1\u0009*\u00091")) !== 1) {
    assert(false, "#1: 1\\u0009*\\u00091 === 1");
}

if (evaluate(script("1\u000B*\u000B1")) !== 1) {
    assert(false, "#2: 1\\u000B*\\u000B1 === 1");
}

if (evaluate(script("1\u000C*\u000C1")) !== 1) {
    assert(false, "#3: 1\\u000C*\\u000C1 === 1");
}

if (evaluate(script("1\u0020*\u00201")) !== 1) {
    assert(false, "#4: 1\\u0020*\\u00201 === 1");
}

if (evaluate(script("1\u00A0*\u00A01")) !== 1) {
    assert(false, "#5: 1\\u00A0*\\u00A01 === 1");
}

if (evaluate(script("1\u000A*\u000A1")) !== 1) {
    assert(false, "#6: 1\\u000A*\\u000A1 === 1");
}

if (evaluate(script("1\u000D*\u000D1")) !== 1) {
    assert(false, "#7: 1\\u000D*\\u000D1 === 1");
}

if (evaluate(script("1\u2028*\u20281")) !== 1) {
    assert(false, "#8: 1\\u2028*\\u20281 === 1");
}

if (evaluate(script("1\u2029*\u20291")) !== 1) {
    assert(false, "#9: 1\\u2029*\\u20291 === 1");
}

if (evaluate(script("1\u0009\u000B\u000C\u0020\u00A0\u000A\u000D\u2028\u2029*\u0009\u000B\u000C\u0020\u00A0\u000A\u000D\u2028\u20291")) !== 1) {
    assert(false, "#10: 1\\u0009\\u000B\\u000C\\u0020\\u00A0\\u000A\\u000D\\u2028\\u2029*\\u0009\\u000B\\u000C\\u0020\\u00A0\\u000A\\u000D\\u2028\\u20291 === 1");
}



//? test: 11.5.1_A2.1_T1
//? description: Either Type is not Reference or GetBase is not null
//? expect: null
//? source: ...
#!/usr/bin/env afw

if (1 * 1 !== 1) {
    assert(false, "1 * 1 !== 1");
}

loc x: integer = 1;
if (x * 1 !== 1) {
    assert(false, "x * 1 !== 1");
}

loc y: integer = 1;
if (1 * y !== 1) {
    assert(false, "1 * y !== 1");
}

x = 1;
y = 1;
if (x * y !== 1) {
    assert(false, "x * y !== 1");
}



//? test: 11.5.1_A2.4_T1
//? description: First expression is evaluated first, and then second expression - checking with "="
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

loc x: integer = 0;
if ((x = 1) * x !== 1) {
    assert(false, "x = 1 * x !== 1");
}

loc x: integer = 0;
if (x * (x = 1) !== 0) {
    assert(false, "x * (x = 1) !== 0");
}



//? test: 11.5.1_A2.4_T2
//? description: First expression is evaluated first, and then second expression - checking with assert()
//? expect: null
//? source: ...
#!/usr/bin/env afw

loc x1: integer = 0;
loc y1: integer = 0;

const x: function = function (): any {
    x1 = 1;
    assert(false, "x");
};

const y: function = function (): any {
    y1 = 1;
    assert(false, "y");
};

safe_evaluate(
    x() * y(),
    "error"
);

assert(x1 === 1, "x1 === 1");
assert(y1 === 0, "y1 === 0");



//? test: 11.5.1_A4_T2
//? description: The sign of the result is positive if both operands have the same sign, negative if the operands have different signs
//? expect: null
//? source: ...
#!/usr/bin/env afw

if (1 * 1 !== 1) {
    assert(false, "#1: 1 * 1 !== 1");
}

if (1 * -1 !== -1) {
    assert(false, "#2: 1 * -1 !== -1");
}

if (-1 * 1 !== -1) {
    assert(false, "#3: -1 * 1 !== -1");
}

/*
if (-1 * -1 !== 1) {
    assert(false, "#4: -1 * -1 !== 1");
}
*/

if (0 * 0 !== 0) {
    assert(false, "#5: 0 * 0 !== 0");
}

if (0 * -0 !== -0) {
    assert(false, "#6: 0 * -0 !== 0");
}

if (-0 * 0 !== -0) {
    assert(false, "#7: -0 * 0 !== -0");
}

if (-0 * -0 !== 0) {
    assert(false, "#8: -0 * -0 !== 0");
}



//? test: 11.5.1_A4_T4
//? description: Multiplication of an infinity by an infinity results in an infinity of appropriate sign
//? expect: null
//? source: ...
#!/usr/bin/env afw

if (-Infinity * -Infinity !== +Infinity) {
    assert(false, "#1: -Infinity * -Infinity !== Infinity");
}

if (+Infinity * Infinity !== +Infinity) {
    assert(false, "#2: Infinity * Infinity !== Infinity");
}

if (-Infinity * +Infinity !== -Infinity) {
    assert(false, "#3: -Infinity * Infinity !== -Infinity");
}

if (+Infinity * -Infinity !== -Infinity) {
    assert(false, "#4: Infinity * -Infinity !== -Infinity");
}



//? test: 11.5.1_A4_T5
//? description: Multiplication of an infinity by a finite non-zero value results in a signed infinity
//? expect: null
//? source: ...
#!/usr/bin/env afw

if (-Infinity * -1.0 !== +Infinity) {
    assert(false, "#1: -Infinity * -1.0 !== Infinity");
}

if (-1.0 * -Infinity !== +Infinity) {
    assert(false, "#2: -1.0 * -Infinity !== Infinity");
}

if (+Infinity * -1.0 !== -Infinity) {
    assert(false, "#3: Infinity * -1.0 !== -Infinity");
}

if (-1.0 * +Infinity !== -Infinity) {
    assert(false, "#4: -1.0 * Infinity !== -Infinity");
}



//? test: line-terminator
//? description: Line terminator between the operands of a multiplication operator
//? expect: null
//? source: ...
#!/usr/bin/env afw

loc x: integer = 18

*

2

*

9
;

assert(x === 324, "x === 324");

