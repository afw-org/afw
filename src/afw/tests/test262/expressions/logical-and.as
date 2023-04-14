#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: logical-and.as
//? customPurpose: Part of test262
//? description: Tests logical-and expressions
//? sourceType: script
//?
//? test: 11.11.1_A1
//? description: White Space and Line Terminator between expressions and "&&" 
//? expect: 0
//? source: ...
#!/usr/bin/env afw

if (evaluate(script("true\u0009&&\u0009true")) !== true) {
    assert(false, "#1: (true\\u0009&&\\u0009true) === true");
}


return 0;

//? test: 11.11.1_A2.4_T1
//? description: First expression is evaluated first, and then second expression: checking with "="
//? expect: 0
//? skip: true
//? source: ...
#!/usr/bin/env afw

loc x: boolean = false;
if (((x = true) && x) !== true) {
    assert(false, "#1: ((x = true) && x) === true");
}

loc x: boolean = false;
if ((x && (x = true)) !== false) {
    assert(false, "#2: (x && (x = true)) === false");
}

return 0;

//? test: 11.11.1_A2.4_T2
//? description: First expression is evaluated first, and then second expression: checking with assert()
//? expect: 0
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

safe_evaluate(
    x() && y(),
    "error"
);

assert(x1 === 1, "x1");
assert(y1 === 0, "y1");

return 0;

//? test: 11.11.1_A3_T1
//? description: If x is false, return x
//? expect: 0
//? source: ...
#!/usr/bin/env afw

if ((false && true) !== false) {
    assert(false, "#1: (false && true) === false");
}

if ((false && false) !== false) {
    assert(false, "#2: (false && false) === false");
}

return 0;

//? test: 11.11.1_A3_T2
//? description: If boolean(x) is false, return x
//? expect: 0
//? skip: true
//? source: ...
#!/usr/bin/env afw

// \fixme skip for now
if ((-0 && -1) !== 0) {
    assert(false, "#1: (-0 && -1) === 0");
}

return 0;

//? test: 11.11.1_A3_T3
//? description: If boolean(x) is false, return x
//? expect: 0
//? skip: true
//? source: ...
#!/usr/bin/env afw

// \fixme skip for now
if (("" && "1") !== "") {
    assert(false, "#1: (\"\" && \"1\") === \"\"");
}

return 0;

//? test: 11.11.1_A4_T1
//? description: If boolean(x) is false, return x
//? expect: 0
//? source: ...
#!/usr/bin/env afw

// \fixme skip for now
if ((true && true) !== true) {
    assert(false, "#1: (true && true) === true");
}

if ((true && false) !== false) {
    assert(false, "#2: (true && false) === false");
}

return 0;

//? test: 11.11.1_A4_T2
//? description: If boolean(x) is true, return y
//? expect: 0
//? skip: true
//? source: ...
#!/usr/bin/env afw

// \fixme skip for now
if ((-1 && -0) !== 0) {
    assert(false, "#1: (-1 && -0) === 0");
}

if ((-1 && 0) !== 0) {
    assert(false, "#2: (-1 && 0) === 0");
}

return 0;

//? test: 11.11.1_A4_T3
//? description: If boolean(x) is true, return y
//? expect: 0
//? skip: true
//? source: ...
#!/usr/bin/env afw

// \fixme skip for now
if (("0" && "-1") !== "-1") {
    assert(false, "#1: (\"0\" && \"-1\") === \"-1\"");
}

if (("-1" && "x") !== "x") {
    assert(false, "#2: (\"-1\" && \"x\") === \"x\"");
}

return 0;