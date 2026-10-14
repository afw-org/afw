#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/math.as
//? customPurpose: Part of core function tests
//? description: Tests for the math functions (#622): ECMAScript's Math, doubles only; IEEE 754 exceptions are errors under #compile strictDouble (#628).
//? sourceType: script
//?
//? test: math-roots-and-logs
//? description: sqrt, cbrt, exp, log, log10, log2.
//? expect: [4.0, 3.0, -2.0, 1.0, 1.0, 3.0, 3.0, true]
//? source: ...

[sqrt(16.0), cbrt(27.0), cbrt(-8.0), exp(0.0), log(#e), log10(1000.0), log2(8.0), exp(1.0) === #e]

//?
//? test: math-trig
//? description: sin, cos, tan, asin, acos, atan, atan2 (y first), in radians.
//? expect: [0.0, -1.0, 1.5707963267948966, 3.141592653589793, 0.7853981633974483, -2.356194490192345, true]
//? source: ...

[sin(0.0), cos(#pi), asin(1.0), acos(-1.0), atan(1.0), atan2(-1.0, -1.0), abs(tan(#pi / 4.0) - 1.0) < 1.0e-15]

//?
//? test: math-trunc-sign
//? description: trunc toward zero; sign of a double keeps a zero's sign; sign of an integer is an integer.
//? expect: [-1.0, 1.0, -1.0, 1.0, "-0.0E0", -1, 0, 1]
//? source: ...

[trunc(-1.7), trunc(1.7), sign(-3.5), sign(0.1), string(sign(-0.0)), sign(-9), sign(0), sign(9)]

//?
//? test: math-hypot
//? description: hypot of one or more doubles, without overflow in between.
//? expect: [5.0, 2.0, 13.0, true, Infinity]
//? source: ...

[hypot(3.0, 4.0), hypot(-2.0), hypot(3.0, 4.0, 12.0), hypot(#doubleMax, #doubleMax) > #doubleMax / 2.0 && hypot(#doubleMax, 1.0) === #doubleMax, hypot(#infinity, 1.0)]

//?
//? test: math-hypot-none
//? description: hypot needs at least one value (ECMAScript's hypot() is 0).
//? expect: error
//? source: ...

hypot<double>()

//?
//? test: math-infinity
//? description: Infinity is a value: log(0.0), exp(Infinity), atan(Infinity).
//? expect: [-Infinity, Infinity, 1.5707963267948966]
//? source: ...

[log(0.0), exp(#infinity), atan(#infinity)]

//?
//? test: math-domain
//? description: outside a function's domain the result is NaN, as ECMAScript.
//? expect: [true, true, true, true, true, true]
//? source: ...

[is_NaN(sqrt(-1.0)), is_NaN(log(-1.0)), is_NaN(log10(-0.5)), is_NaN(acos(1.5)), is_NaN(cos(#minusInfinity)), is_NaN(exp(#nan))]

//?
//? test: math-domain-strict
//? description: under #compile strictDouble that is an IEEE 754 invalid operation, an error.
//? expect: error:Function 'sqrt<double>' is an IEEE 754 invalid operation for these arguments: the result would be NaN (#compile strictDouble)
//? source: ...

#compile strictDouble;
return sqrt(-4.0);

//?
//? test: math-log-zero-strict
//? description: under #compile strictDouble log(0.0) is an IEEE 754 division by zero, an error.
//? expect: error:Function 'log<double>' is an IEEE 754 division by zero for these arguments (#compile strictDouble)
//? source: ...

#compile strictDouble;
return log(0.0);

//?
//? test: math-integer
//? description: an integer is not converted to a double.
//? expect: error:'sqrt' is not a method of data type 'integer'; it takes a double: write to_double(v)
//? source: ...

sqrt(4)

//?
//? test: math-method
//? description: math functions are methods of a double.
//? expect: [2.0, 1.0]
//? source: ...

[4.0->sqrt(), 1.5->trunc()]
