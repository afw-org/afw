#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/double_floor.as
//? customPurpose: Part of core function tests
//? description: Tests for floor<double> function.
//? sourceType: script
//?
//? test: floor<double>-1
//? description: 2.8
//? expect: 2.0
//? source: ...

floor<double>(
    2.8
)

//?
//? test: floor<double>-2
//? description: -2.8
//? expect: -3.0
//? source: ...

floor<double>(
    -2.8
)

//?
//? test: floor<double>-3
//? description: NaN (#628)
//? expect: error:Parameter 1 of function 'floor<double>' is NaN; no double operation takes NaN (is_NaN(v) tests for it)
//? source: ...

floor<double>(
    NaN
)

