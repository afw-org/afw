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
