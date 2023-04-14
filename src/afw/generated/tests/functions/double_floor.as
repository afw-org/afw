#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/double_floor.as
//? customPurpose: Part of core function tests
//? description: Tests for floor<double> function.
//? sourceType: expression
//?
//? test: floor<double>-1
//? description: 2.8
//? expect: double(2.0)
//? source: ...

floor<double>(
    double(2.8)
)

//?
//? test: floor<double>-2
//? description: -2.8
//? expect: double(-3.0)
//? source: ...

floor<double>(
    double(-2.8)
)

//?
//? test: floor<double>-3
//? description: NaN
//? expect: double(NaN)
//? source: ...

floor<double>(
    double(NaN)
)

