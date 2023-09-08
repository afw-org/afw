#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/double_subtract.as
//? customPurpose: Part of core function tests
//? description: Tests for subtract<double> function.
//? sourceType: script
//?
//? test: subtract<double>-1
//? description: INF minus INF
//? expect: double(NaN)
//? source: ...

subtract<double>(
    double(Infinity),
    double(Infinity)
)

//?
//? test: subtract<double>-2
//? description: NaN minus INF
//? expect: double(NaN)
//? source: ...

subtract<double>(
    double(NaN),
    double(Infinity)
)

//?
//? test: subtract<double>-3
//? description: Normal
//? expect: double(50.0)
//? source: ...

subtract<double>(
    double(50.1),
    double(0.1)
)

