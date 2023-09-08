#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/double_divide.as
//? customPurpose: Part of core function tests
//? description: Tests for divide<double> function.
//? sourceType: script
//?
//? test: divide<double>-1
//? description: Divide by self
//? expect: double(1.0)
//? source: ...

divide<double>(
    double(50.0),
    double(50.0)
)

//?
//? test: divide<double>-2
//? description: Divide by 0
//? expect: double(Infinity)
//? source: ...

divide<double>(
    double(5.0),
    double(0.0)
)

//?
//? test: divide<double>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

divide<double>(
    double(x.0),
    double(x.0)
)

