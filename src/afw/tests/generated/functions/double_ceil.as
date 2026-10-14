#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/double_ceil.as
//? customPurpose: Part of core function tests
//? description: Tests for ceil<double> function.
//? sourceType: script
//?
//? test: ceil<double>-1
//? description: 2.8
//? expect: 3.0
//? source: ...

ceil<double>(
    2.8
)

//?
//? test: ceil<double>-2
//? description: -2.8
//? expect: -2.0
//? source: ...

ceil<double>(
    -2.8
)

//?
//? test: ceil<double>-3
//? description: NaN (#628)
//? expect: error:Parameter 1 of function 'ceil<double>' is NaN; no double operation takes NaN (is_NaN(v) tests for it)
//? source: ...

ceil<double>(
    NaN
)

