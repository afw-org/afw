#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: integer_overflow.as
//? customPurpose: Part of miscellaneous category tests
//? description: abs and negative reject #integerMin instead of overflowing
//? sourceType: script
//?
//? test: abs-min
//? description: abs of #integerMin throws
//? expect: error
//? source: ...

abs(#integerMin)

//?
//? test: negative-min
//? description: negative of #integerMin throws
//? expect: error
//? source: ...

negative(#integerMin)

//?
//? test: abs-neighbors
//? description: abs of 0, -1, 1, and #integerMin + 1
//? expect: 0
//? source: ...

assert(abs(0) === 0);
assert(abs(-1) === 1);
assert(abs(1) === 1);
assert(abs(#integerMin + 1) === #integerMax);
assert(abs(#integerMax) === #integerMax);
return 0;

//?
//? test: negative-neighbors
//? description: negative of 0, -1, 1, and #integerMin + 1
//? expect: 0
//? source: ...

assert(negative(0) === 0);
assert(negative(-1) === 1);
assert(negative(1) === -1);
assert(negative(#integerMin + 1) === #integerMax);
assert(negative(#integerMax) === #integerMin + 1);
return 0;
