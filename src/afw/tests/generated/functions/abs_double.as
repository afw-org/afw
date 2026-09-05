#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/abs_double.as
//? customPurpose: Part of core function tests
//? description: Tests for the abs<double> function.
//? sourceType: script
//?
//? test: abs_double-error-nan
//? description: abs<double> throws when the argument is NaN
//? expect: error:arg is not a number
//? source: ...

abs(double("NaN"))

//?
//? test: abs_double-error-infinite
//? description: abs<double> throws when the argument is infinite
//? expect: error:arg is infinite
//? source: ...

abs(double("Infinity"))
