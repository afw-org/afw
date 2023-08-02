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
    throw "#1: (true\\u0009&&\\u0009true) === true";
}


return 0;

//? test: 11.11.1_A2.4_T1
//? description: First expression is evaluated first, and then second expression: checking with "="
//? expect: error:Parse error at offset 87 around line 5 column 9: Expecting ')'
//? source: ...
#!/usr/bin/env afw

let x = false;
// can't do assignment inside an expression
if (((x = true) && x) !== true) {
    throw "#1: ((x = true) && x) === true";
}

let x = false;
if ((x && (x = true)) !== false) {
    throw "#2: (x && (x = true)) === false";
}


//? test: 11.11.1_A2.4_T2
//? description: First expression is evaluated first, and then second expression: checking with assert()
//? expect: 0
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
    throw "#1: (false && true) === false";
}

if ((false && false) !== false) {
    throw "#2: (false && false) === false";
}

return 0;

//? test: 11.11.1_A3_T2
//? description: If boolean(x) is false, return x
//? expect: error:Parameter 1 of function 'and' must evaluate to data type 'boolean' but evaluated to be 'integer'
//? source: ...
#!/usr/bin/env afw

if ((-0 && -1) !== 0) {
    throw "#1: (-0 && -1) === 0";
}

return 0;


//? test: 11.11.1_A3_T3
//? description: If boolean(x) is false, return x
//? expect: error:Parameter 1 of function 'and' must evaluate to data type 'boolean' but evaluated to be 'string'
//? source: ...
#!/usr/bin/env afw

if (("" && "1") !== "") {
    throw "#1: (\"\" && \"1\") === \"\"";
}

return 0;


//? test: 11.11.1_A4_T1
//? description: If boolean(x) is false, return x
//? expect: 0
//? source: ...
#!/usr/bin/env afw

if ((true && true) !== true) {
    throw "#1: (true && true) === true";
}

if ((true && false) !== false) {
    throw "#2: (true && false) === false";
}

return 0;

//? test: 11.11.1_A4_T2
//? description: If boolean(x) is true, return y
//? expect: error:Parameter 1 of function 'and' must evaluate to data type 'boolean' but evaluated to be 'integer'
//? source: ...
#!/usr/bin/env afw

if ((-1 && -0) !== 0) {
    throw "#1: (-1 && -0) === 0";
}

if ((-1 && 0) !== 0) {
    throw "#2: (-1 && 0) === 0";
}

return 0;

//? test: 11.11.1_A4_T3
//? description: If boolean(x) is true, return y
//? expect: error:Parameter 1 of function 'and' must evaluate to data type 'boolean' but evaluated to be 'string'
//? source: ...
#!/usr/bin/env afw

if (("0" && "-1") !== "-1") {
    throw "#1: (\"0\" && \"-1\") === \"-1\"";
}

if (("-1" && "x") !== "x") {
    throw "#2: (\"-1\" && \"x\") === \"x\"";
}

return 0;