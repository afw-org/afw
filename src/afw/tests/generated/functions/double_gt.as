#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/double_gt.as
//? customPurpose: Part of core function tests
//? description: Tests for gt<double> function.
//? sourceType: script
//?
//? test: gt<double>-1
//? description: 6 > 5 is true
//? expect: boolean(true)
//? source: ...

gt<double>(
    double(6.9),
    double(5.9)
)

//?
//? test: gt<double>-2
//? description: 5 > 5 is false
//? expect: boolean(false)
//? source: ...

gt<double>(
    double(5.0),
    double(5.0)
)

//?
//? test: gt<double>-3
//? description: 600000 > 500000
//? expect: boolean(true)
//? source: ...

gt<double>(
    double(6000000.0),
    double(5000000.0)
)

