#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: assignment.as
//? customPurpose: Part of language/script tests
//? description: ...
Chained assignment as a statement (issue #62). x = y = 1 sets both.
Assignment is not an expression: (x = 1) > x and let x = y = 1 stay
illegal. let/const RHS remains Expression.
//? sourceType: script
//?
//? test: chain-two
//? description: x = y = 1 sets both
//? expect: 0
//? source: ...

let x;
let y;
x = y = 1;
assert(x === 1);
assert(y === 1);
return 0;

//?
//? test: chain-three
//? description: x = y = z = 2
//? expect: 0
//? source: ...

let x;
let y;
let z;
x = y = z = 2;
assert(x === 2);
assert(y === 2);
assert(z === 2);
return 0;

//?
//? test: chain-compound
//? description: x = y += 1
//? expect: 0
//? source: ...

let x;
let y = 3;
x = y += 1;
assert(y === 4);
assert(x === 4);
return 0;

//?
//? test: chain-plus-equal-then-assign
//? description: x += y = 1
//? expect: 0
//? source: ...

let x = 10;
let y;
x += y = 1;
assert(y === 1);
assert(x === 11);
return 0;

//?
//? test: chain-property
//? description: object properties in a chain
//? expect: 0
//? source: ...

let o = { a: 0, b: 0 };
o.a = o.b = 5;
assert(o.a === 5);
assert(o.b === 5);
return 0;

//?
//? test: chain-index
//? description: array indexes in a chain
//? expect: 0
//? source: ...

let a = [0, 0];
a[0] = a[1] = 8;
assert(a[0] === 8);
assert(a[1] === 8);
return 0;

//?
//? test: chain-rhs-expression
//? description: x = 1 + 2 still works
//? expect: 0
//? source: ...

let x;
x = 1 + 2;
assert(x === 3);
return 0;

//?
//? test: chain-rhs-array
//? description: x = [1, 2] still works
//? expect: 0
//? source: ...

let x;
x = [1, 2];
assert(x[0] === 1);
assert(x[1] === 2);
return 0;

//?
//? test: chain-rhs-object
//? description: x = { a: 1 } still works
//? expect: 0
//? source: ...

let x;
x = { a: 1 };
assert(x.a === 1);
return 0;

//?
//? test: chain-in-for-init
//? description: for (x = y = 1; …)
//? expect: 0
//? source: ...

let x;
let y;
let s = 0;
for (x = y = 1; x < 3; x = x + 1) {
    s = s + y;
}
assert(y === 1);
assert(s === 2);
return 0;

//?
//? test: reject-assign-in-if
//? description: if (x = 1) is not an expression
//? expect: error
//? source: ...

let x;
if (x = 1) {
}

//?
//? test: reject-assign-in-compare
//? description: (x = 1) > 0 stays illegal
//? expect: error
//? source: ...

let x;
let b = (x = 1) > 0;

//?
//? test: reject-paren-inner
//? description: x = (y = 1) is assignment as expression
//? expect: error
//? source: ...

let x;
let y;
x = (y = 1);

//?
//? test: reject-let-rhs-chain
//? description: let x = y = 1 is still Expression on the right
//? expect: error
//? source: ...

let y;
let x = y = 1;

//?
//? test: reject-const-reassign-in-chain
//? description: const in the chain still errors
//? expect: error
//? source: ...

let x;
const y = 1;
x = y = 2;

//?
//? test: decompile-evaluates
//? description: decompile of a chain still evaluates
//? expect: 0
//? source: ...

const src = "let x; let y; x = y = 4; return x + y;";
const d1 = decompile(compile<script>(script(src)));
assert(evaluate(compile<script>(script(d1))) === 8);
return 0;
