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

//?
//? test: function-let-is-undefined
//? description: function body with only let yields undefined
//? expect: 0
//? source: ...

const f = function() { let x = 7; };
assert(f() === undefined);
return 0;

//?
//? test: function-assign-writes
//? description: function body assignment is the result
//? expect: 0
//? source: ...

const f = function() { let x; x = 7; };
assert(f() === 7);
return 0;

//?
//? test: function-if-let
//? description: if with only let does not yield the let value
//? expect: 0
//? source: ...

const f = function() { if (true) { let y = 2; } };
assert(f() === undefined);
return 0;

//?
//? test: function-if-assign
//? description: assignment inside if writes the function result
//? expect: 0
//? source: ...

const f = function() { let y; if (true) { y = 2; } };
assert(f() === 2);
return 0;

//?
//? test: function-expr-body
//? description: expression body is unchanged
//? expect: 0
//? source: ...

const f = function() 7;
assert(f() === 7);
return 0;
