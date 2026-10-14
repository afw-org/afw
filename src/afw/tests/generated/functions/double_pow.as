#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/double_pow.as
//? customPurpose: Part of core function tests
//? description: Tests for pow<double> function.
//? sourceType: script
//?
//? test: pow<double>-1
//? description: 2.0 cubed
//? expect: 8.0
//? source: ...

pow<double>(
    2.0,
    3.0
)

//?
//? test: pow<double>-2
//? description: 2.8 cubed
//? expect: 21.951999999999995
//? source: ...

pow<double>(
    2.8,
    3.0
)

//?
//? test: pow<double>-3
//? description: -2.8 cubed
//? expect: -21.951999999999995
//? source: ...

pow<double>(
    -2.8,
    3.0
)

//?
//? test: pow<double>-4
//? description: NaN (#628)
//? expect: error:Parameter 2 of function 'pow<double>' is NaN; no double operation takes NaN (is_NaN(v) tests for it)
//? source: ...

pow<double>(
    NaN,
    NaN
)

