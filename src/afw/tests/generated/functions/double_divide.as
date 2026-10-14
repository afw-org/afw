#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/double_divide.as
//? customPurpose: Part of core function tests
//? description: Tests for divide<double> function.
//? sourceType: script
//?
//? test: divide<double>-1
//? description: Divide by self
//? expect: 1.0
//? source: ...

divide<double>(
    50.0,
    50.0
)

//?
//? test: divide<double>-2
//? description: Divide by 0
//? expect: Infinity
//? source: ...

divide<double>(
    5.0,
    0.0
)

//?
//? test: divide<double>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

divide<double>(
    to_double(x.0),
    to_double(x.0)
)

