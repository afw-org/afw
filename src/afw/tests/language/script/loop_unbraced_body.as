#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: loop_unbraced_body.as
//? customPurpose: Part of language/script tests
//? description: ...
Unbraced while / do / for / for-of bodies compile as a `{ }` so
each trip has a frame.
//? sourceType: script
//?
//? test: while-unbraced-assign
//? description: unbraced while body assignment
//? expect: 0
//? source: ...

let i = 0;
while (i < 3)
    i = i + 1;
assert(i === 3);
return 0;

//?
//? test: do-unbraced-assign
//? description: unbraced do-while body assignment
//? expect: 0
//? source: ...

let i = 0;
do
    i = i + 1;
while (i < 3);
assert(i === 3);
return 0;

//?
//? test: for-unbraced-assign
//? description: unbraced classic for body assignment
//? expect: 0
//? source: ...

let s = 0;
for (let i = 0; i < 3; i = i + 1)
    s = s + i;
assert(s === 3);
return 0;

//?
//? test: for-of-unbraced-assign
//? description: unbraced for-of body assignment
//? expect: 0
//? source: ...

let s = 0;
for (let v of [1, 2, 3])
    s = s + v;
assert(s === 6);
return 0;

//?
//? test: for-of-unbraced-let-same-name
//? description: ...
unbraced `let x` body of `for (let x of [])` is the same block
as the for-of binding (`'x' already defined`), not a nested `{ }`.
//? expect: error
//? source: ...

for (let x of []) let x = 1;

//?
//? test: while-unbraced-last
//? description: unbraced while last assignment is the script last
//? expect: 0
//? source: ...

const r = evaluate(compile<script>(script(
    "let i = 0; while (i < 3) i = i + 1;")));
assert(r === 3);
return 0;

//?
//? test: unbraced-nested-loops-with-heads
//? description: blocks in an unbraced body (a loop head, an if or while block) run in the body block
//? expect: 0
//? source: ...

let s = "";
for (let i = 0; i < 2; i = i + 1) for (const y of ["p", "q"]) s = s + y;
let t = "";
for (const a of ["x", "y"]) for (let j = 0; j < 2; j = j + 1) t = t + a + string(j);
let n = 0;
let k = 0;
while (k < 2) for (let j = 0; j < 3; j = j + 1) { n = n + 1; k = k + 1; }
let u = 0;
for (let i = 0; i < 3; i = i + 1) if (i > 0) { u = u + i; }
let w = 0;
for (const x of [1, 2]) while (w < x) { w = w + 1; }
assert(s === "pqpq");
assert(t === "x0x1y0y1");
assert(n === 3);
assert(u === 3);
assert(w === 2);
return 0;

//?
//? test: unbraced-body-closure
//? description: a function made in an unbraced body sees that trip's names
//? expect: 0
//? source: ...

let fs = [];
for (let i = 0; i < 3; i = i + 1) push(fs, function () { return i; });
for (const x of [7, 8]) push(fs, function () { return x; });
assert(stringify(map(function (f) { return f(); }, fs)) === "[0,1,2,7,8]");
return 0;

//?
//? test: unbraced-declarations-in-enclosing-block
//? description: a let, const, or function unbraced body declares in the enclosing block
//? expect: 0
//? source: ...

do let y = 3; while (false);
do const z = 4; while (false);
do function f() { return 7; } while (false);
assert(y + z + f() === 14);
return 0;
