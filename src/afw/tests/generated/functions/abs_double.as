#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/abs_double.as
//? customPurpose: Part of core function tests
//? description: Tests for the abs<double> function.
//? sourceType: script
//?
//? test: abs_double-nan
//? description: abs<double> of NaN is NaN, as ECMAScript Math.abs and IEEE 754 abs (quiet)
//? expect: true
//? source: ...

is_NaN(abs(NaN))

//?
//? test: abs_double-infinite
//? description: abs<double> of an infinity is Infinity
//? expect: Infinity
//? source: ...

abs(-Infinity)
