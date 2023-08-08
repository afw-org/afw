#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: spread.as
//? customPurpose: Part of core function tests
//? description: Tests for spread operator.
//? sourceType: script
//?
//? test: spread-1
//? description: Spread operator with empty lists.
//? expect: 0
//? source: ...

const l1 = [];
const l2 = [];
const l3 = [...l1, ...l2];

assert(length(l3) === 0);

return 0;

//? test: spread-2
//? description: Spread operator with first array empty.
//? expect: 0
//? source: ...

const l1 = [];
const l2 = [1];
const l3 = [...l1, ...l2];

assert(length(l3) === 1);

return 0;

//? test: spread-3
//? description: Spread operator with second array empty.
//? expect: 0
//? source: ...

const l1 = [1];
const l2 = [];
const l3 = [...l1, ...l2];

assert(length(l3) === 1);

return 0;

//? test: spread-4
//? description: Spread operator with 2 full lists.
//? expect: 0
//? source: ...

const l1 = [1, 2, 3];
const l2 = [4, 5, 6];
const l3 = [...l1, ...l2];

assert(l3[0] === 1);
assert(l3[1] === 2);
assert(l3[2] === 3);
assert(l3[3] === 4);
assert(l3[4] === 5);
assert(l3[5] === 6);
assert(length(l3) === 6);

return 0;

//? test: spread-foreach
//? description: Spread inside a foreach loop
//? expect: 0
//? source: ...

const l1 = [1, 2, 3];
const l2 = [];

// causes infinite loop
foreach const i of l1 {
    l2 = [...l2, i];
}

return 0;

//? test: spread-foreach-error
//? description: Spread inside a foreach loop with error
//? expect: error:Typesafe error: expecting 'array' but encountered 'integer'
//? source: ...

const l1 = [1, 2, 3];
const l2 = [];

// causes infinite loop
foreach const i of l1 {
    l2 = [...l2, i, ...i];
}

return 0;