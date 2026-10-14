#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/double_multiply.as
//? customPurpose: Part of core function tests
//? description: Tests for multiply<double> function.
//? sourceType: script
//?
//? test: multiply<double>-1
//? description: A NaN operand (#628)
//? expect: error:Parameter 1 of function 'multiply<double>' is NaN; no double operation takes NaN (is_NaN(v) tests for it)
//? source: ...

multiply<double>(
    NaN,
    Infinity
)

//?
//? test: multiply<double>-2
//? description: 3e2 times 3
//? expect: 900.0
//? source: ...

multiply<double>(
    3.0e2,
    3.0
)

