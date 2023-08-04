#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/double_pow.as
//? customPurpose: Part of core function tests
//? description: Tests for pow<double> function.
//? sourceType: expression
//?
//? test: pow<double>-1
//? description: 2.0 cubed
//? expect: double(8.0)
//? source: ...

pow<double>(
    double(2.0),
    double(3)
)

//?
//? test: pow<double>-2
//? description: 2.8 cubed
//? expect: double(21.951999999999995)
//? source: ...

pow<double>(
    double(2.8),
    double(3)
)

//?
//? test: pow<double>-3
//? description: -2.8 cubed
//? expect: double(-21.951999999999995)
//? source: ...

pow<double>(
    double(-2.8),
    double(3)
)

//?
//? test: pow<double>-4
//? description: NaN
//? expect: double(NaN)
//? source: ...

pow<double>(
    double(NaN),
    double(NaN)
)

