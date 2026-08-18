#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: includes_empty.as
//? customPurpose: Part of core function tests
//? description: includes() on an empty array with fromIndex does not divide by zero
//? sourceType: script
//?
//? test: empty-no-fromIndex
//? description: includes on empty array without fromIndex is false
//? expect: 0
//? source: ...

assert(includes([], 1) === false);
assert(includes([], "x") === false);
return 0;

//?
//? test: empty-fromIndex-zero
//? description: includes on empty array with fromIndex 0 is false
//? expect: 0
//? source: ...

assert(includes([], 1, 0) === false);
return 0;

//?
//? test: empty-fromIndex-positive
//? description: includes on empty array with positive fromIndex is false
//? expect: 0
//? source: ...

assert(includes([], 1, 1) === false);
assert(includes([], 1, 100) === false);
return 0;

//?
//? test: empty-fromIndex-negative
//? description: includes on empty array with negative fromIndex is false
//? expect: 0
//? source: ...

assert(includes([], 1, -1) === false);
assert(includes([], 1, -100) === false);
return 0;

//?
//? test: one-element-fromIndex
//? description: includes fromIndex still works on a non-empty array
//? expect: 0
//? source: ...

assert(includes([1], 1, 0) === true);
assert(includes([1], 1, -1) === true);
assert(includes([1, 2], 1, 1) === false);
assert(includes([1, 2], 2, 1) === true);
return 0;
