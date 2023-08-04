#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: unary-minus.as
//? customPurpose: Part of test262
//? description: expressions/unary-minus
//? sourceType: script
//?
//? test: 11.4.7_A2.1_T1
//? description: Operator -x
//? expect: null
//? source: ...
#!/usr/bin/env afw

if (-1 !== -1) {
    throw "#1: -1 === -1";
}

if (-(-1) !== 1) {
    throw "#2: -(-1) === 1";
}

let x: integer = -1;
if (-x !== 1) {
    throw "#3: -x === 1";
}

x = -1;
if (-(-x) !== -1) {
    throw "#4: -(-x) === -1";
}



//? test: 11.4.7_A3_T2
//? description: Operator -x returns -integer(x)
//? expect: null
//? source: ...
#!/usr/bin/env afw

if (-(1) !== -1) {
    throw "#1: -(1) === -1";
}



//? test: 11.4.7_A4.2
//? description: Negating +0 produces -0, negating -0 produces +0
//? expect: null
//? source: ...
#!/usr/bin/env afw

let x: integer = 0;
x = -x;
if (x !== -0) {
    throw "#1: x === -0";
}

x = -0;
x = -x;
if (x !== 0) {
    throw "#2: x === 0";
}
