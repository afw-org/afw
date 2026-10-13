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

//? test: spread-for-of
//? description: Spread inside a for-of loop
//? expect: 0
//? source: ...

const l1 = [1, 2, 3];
let l2 = [];

for (const i of l1) {
    l2 = [...l2, i];
}

assert(eq(l2, [1, 2, 3]));
return 0;

//? test: spread-for-of-error
//? description: Spread inside a for-of loop with error
//? expect: error:Spread (...) requires an array or a value that can be iterated, such as a string
//? source: ...

const l1 = [1, 2, 3];
const l2 = [];

// ...i fails when i is integer (list literal lowers to array(...) call)
for (const i of l1) {
    l2 = [...l2, i, ...i];
}

return 0;

//? test: spread-string
//? description: Spreading a string gives its code points, in an array literal and in a call (#583)
//? expect: 0
//? source: ...

assert(eq([..."ab"], ["a", "b"]));
assert(eq([..."x\u20acy"], ["x", "\u20ac", "y"]));
assert(eq([...""], []));
assert(eq([..."a", 1], ["a", 1]));
assert(eq([..."ab"anyURI], ["a", "b"]), "any data type stored as UTF-8");
const u = [..."ab"anyURI];
assert(meta(u[0]).dataType === "string",
    "elements are one-code-point strings");
assert(eq(concat(..."abc"), "abc"), "call-site spread");
assert(is_in<string>("b", [..."abc"]));
return 0;

//? test: spread-not-iterable
//? description: Spreading a value that can't be iterated is an error
//? expect: error:Spread (...) requires an array or a value that can be iterated, such as a string
//? source: ...

return [...5];
