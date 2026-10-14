#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/abs_double.as
//? customPurpose: Part of core function tests
//? description: Tests for the abs<double> function.
//? sourceType: script
//?
//? test: abs_double-error-nan
//? description: abs<double> throws when the argument is NaN (#628)
//? expect: error:Parameter 1 of function 'abs<double>' is NaN; no double operation takes NaN (is_NaN(v) tests for it)
//? source: ...

abs(NaN)

//?
//? test: abs_double-infinite
//? description: abs<double> of an infinity is Infinity
//? expect: Infinity
//? source: ...

abs(Infinity)
