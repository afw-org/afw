#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: compiler_literals.as
//? customPurpose: Compiler literals
//? description: #doubleMax and other compiler literals fold to permanent values
//? sourceType: script
//?
//? test: double-limits
//? description: #doubleMax / #doubleMin / #doubleEpsilon / #doubleMinSubnormal
//? expect: 0
//? source: ...

assert(#doubleMax === 1.7976931348623157E308);
assert(#doubleMin > 0.0);
assert(#doubleMin < 1.0);
assert(#doubleMinSubnormal > 0.0);
assert(#doubleMinSubnormal < #doubleMin);
assert(#doubleEpsilon > 0.0);
assert(#doubleEpsilon < 1.0);
assert(1.0 + #doubleEpsilon !== 1.0);
return 0;

//?
//? test: integer-limits
//? description: #integerMax / #integerMin
//? expect: 0
//? source: ...

assert(#integerMax > 0);
assert(#integerMin < 0);
assert(#integerMax + #integerMin === -1);
return 0;

//?
//? test: math-and-ieee-aliases
//? description: #pi #e #infinity #inf #minusInfinity #nan
//? expect: 0
//? source: ...

assert(#pi > 3.14);
assert(#pi < 3.15);
assert(#e > 2.71);
assert(#e < 2.72);
assert(#infinity === Infinity);
assert(#inf === Infinity);
assert(#minusInfinity === -Infinity);
assert(is_NaN(#nan));
return 0;

//?
//? test: unknown-hash-name-still-error
//? description: Unknown #name in expression is still an error
//? expect: error
//? source: ...

return #doubleMaximum;
