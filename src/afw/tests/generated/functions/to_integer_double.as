#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/to_integer_double.as
//? customPurpose: Part of core function tests
//? description: Tests for the to_integer<double> function.
//? sourceType: script
//?
//? test: to_integer_double-error-nan
//? description: to_integer<double> throws when the argument is NaN
//? expect: error:arg is not a number
//? source: ...

to_integer(double("NaN"))

//?
//? test: to_integer_double-error-infinite
//? description: to_integer<double> throws when the argument is infinite
//? expect: error:arg is infinite
//? source: ...

to_integer(double("Infinity"))
