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
    6.9,
    5.9
)

//?
//? test: gt<double>-2
//? description: 5 > 5 is false
//? expect: boolean(false)
//? source: ...

gt<double>(
    5.0,
    5.0
)

//?
//? test: gt<double>-3
//? description: 600000 > 500000
//? expect: boolean(true)
//? source: ...

gt<double>(
    6000000.0,
    5000000.0
)

