#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: void_result.as
//? customPurpose: Part of language/script tests
//? description: ...
Built-in functions declared void return the void singleton, not
undefined. Two such calls yield the same value (address identity).
//? sourceType: script
//?
//? test: print-is-not-undefined
//? description: print() is not undefined or null
//? expect: 0
//? source: ...

const r = print();
assert(r !== undefined);
assert(r !== null);
return 0;

//?
//? test: void-identity
//? description: two void functions return the same singleton
//? expect: 0
//? source: ...

assert(print() === println());
return 0;

//?
//? test: assert-is-void
//? description: assert(true) is the same singleton as print()
//? expect: 0
//? source: ...

assert(assert(true) === print());
return 0;
