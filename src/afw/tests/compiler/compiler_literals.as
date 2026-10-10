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

//?
//? test: double-literal-large-integer-part
//? description: a double literal whose integer part does not fit an integer is still a double
//? expect: 0
//? source: ...

assert(99999999999999999999.5 === 1e20, "99999999999999999999.5");
assert(123456789012345680000.0 === 1.2345678901234568e20, "123456789012345680000.0");
assert(18446744073709551616e0 === 1.8446744073709552e19, "2^64 with exponent");
return 0;

//?
//? test: double-literal-subnormal
//? description: a subnormal double literal is accepted (strtod reports ERANGE for it)
//? expect: 0
//? source: ...

assert(5e-324 > 0.0, "5e-324");
assert(4.9e-324 === 5e-324, "4.9e-324 rounds to the smallest subnormal");
assert(2.2250738585072011e-308 < 2.2250738585072014e-308, "largest subnormal");
return 0;

//?
//? test: integer-literal-out-of-range
//? description: 2^63 is out of range like any larger integer (it was "Invalid number"); -2^63 fits
//? expect: 0
//? source: ...

function compileError(source) {
    try {
        compile<script>(script(source));
    }
    catch (e) {
        return e.message;
    }
    return "compiled";
}

assert(includes(compileError("return 9223372036854775808;"),
    "Integer is out of range"), "2^63");
assert(includes(compileError("return 99999999999999999999;"),
    "Integer is out of range"), "2^66");
assert(compileError("return -9223372036854775808;") === "compiled", "-2^63");
return 0;
