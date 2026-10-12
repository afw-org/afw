#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/double_is_in.as
//? customPurpose: Part of core function tests
//? description: Tests for is_in<double> function.
//? sourceType: script
//?
//? test: is_in<double>-1
//? description: In array
//? expect: boolean(true)
//? source: ...

is_in<double>(
    1.01E4,
    array<double>(
        4.0,
        2345678.0,
        23.0E35,
        1.01E4
    )
)

//?
//? test: is_in<double>-2
//? description: In array
//? expect: boolean(true)
//? source: ...

is_in<double>(
    1.01E4,
    array<double>(
        4.0,
        2345678.0,
        23.0E35,
        10100.0
    )
)

//?
//? test: is_in<double>-3
//? description: Not in array
//? expect: boolean(false)
//? source: ...

is_in<double>(
    1.01E4,
    array<double>(
        4.0,
        2345678.0,
        23.0E35,
        1.01E5
    )
)

