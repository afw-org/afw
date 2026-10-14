#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/double_lt.as
//? customPurpose: Part of core function tests
//? description: Tests for lt<double> function.
//? sourceType: script
//?
//? test: lt<double>-1
//? description: 109 less than 1.09e2
//? expect: boolean(false)
//? source: ...

lt<double>(
    109.0,
    1.09E2
)

//?
//? test: lt<double>-2
//? description: 109 less than 1.19e2
//? expect: boolean(true)
//? source: ...

lt<double>(
    109.0,
    1.19E2
)

//?
//? test: lt<double>-3
//? description: -INF less than INF
//? expect: boolean(true)
//? source: ...

lt<double>(
    -Infinity,
    Infinity
)

//?
//? test: lt<double>-4
//? description: NaN has no order (#628)
//? expect: error:lt<double> of NaN; NaN has no order (is_NaN(v) tests for it)
//? source: ...

lt<double>(
    NaN,
    Infinity
)

