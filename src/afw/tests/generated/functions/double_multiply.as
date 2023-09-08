#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/double_multiply.as
//? customPurpose: Part of core function tests
//? description: Tests for multiply<double> function.
//? sourceType: script
//?
//? test: multiply<double>-1
//? description: NaN times anything is NaN
//? expect: double(NaN)
//? source: ...

multiply<double>(
    double(NaN),
    double(Infinity)
)

//?
//? test: multiply<double>-2
//? description: 3e2 times 3
//? expect: double(900.0)
//? source: ...

multiply<double>(
    double(3.0e2),
    double(3.0)
)

