#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/double_subtract.as
//? customPurpose: Part of core function tests
//? description: Tests for subtract<double> function.
//? sourceType: script
//?
//? test: subtract<double>-1
//? description: INF minus INF would be NaN (#628)
//? expect: error:Function 'subtract<double>' would give NaN (not a number) for these arguments
//? source: ...

subtract<double>(
    Infinity,
    Infinity
)

//?
//? test: subtract<double>-2
//? description: NaN minus INF: a NaN operand (#628)
//? expect: error:Parameter 1 of function 'subtract<double>' is NaN; no double operation takes NaN (is_NaN(v) tests for it)
//? source: ...

subtract<double>(
    NaN,
    Infinity
)

//?
//? test: subtract<double>-3
//? description: Normal
//? expect: 50.0
//? source: ...

subtract<double>(
    50.1,
    0.1
)

