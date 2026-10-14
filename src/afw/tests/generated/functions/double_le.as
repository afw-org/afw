#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/double_le.as
//? customPurpose: Part of core function tests
//? description: Tests for le<double> function.
//? sourceType: script
//?
//? test: le<double>-1
//? description: 109 less than 1.09e2
//? expect: boolean(true)
//? source: ...

le<double>(
    109.0,
    1.09E2
)

//?
//? test: le<double>-2
//? description: 109 less than 1.19e2
//? expect: boolean(true)
//? source: ...

le<double>(
    109.0,
    1.19E2
)

//?
//? test: le<double>-3
//? description: -INF less than INF
//? expect: boolean(true)
//? source: ...

le<double>(
    -Infinity,
    Infinity
)

//?
//? test: le<double>-4
//? description: NaN equal NaN
//? expect: boolean(false)
//? source: ...

le<double>(
    NaN,
    NaN
)

