#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/double_ge.as
//? customPurpose: Part of core function tests
//? description: Tests for ge<double> function.
//? sourceType: script
//?
//? test: ge<double>-1
//? description: 6 > 5 is true
//? expect: boolean(true)
//? source: ...

ge<double>(
    6.9,
    5.9
)

//?
//? test: ge<double>-2
//? description: 5 >= 5 is true
//? expect: boolean(true)
//? source: ...

ge<double>(
    5.0,
    5.0
)

//?
//? test: ge<double>-3
//? description: INF >= -INF is true
//? expect: boolean(true)
//? source: ...

ge<double>(
    Infinity,
    -Infinity
)

//?
//? test: ge<double>-4
//? description: 600000 not greater than 700000
//? expect: boolean(false)
//? source: ...

ge<double>(
    6000000.0,
    7000000.0
)

