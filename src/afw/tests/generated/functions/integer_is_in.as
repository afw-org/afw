#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/integer_is_in.as
//? customPurpose: Part of core function tests
//? description: Tests for is_in<integer> function.
//? sourceType: script
//?
//? test: is_in<integer>-1
//? description: 0 is not in array
//? expect: boolean(false)
//? source: ...

is_in<integer>(
    0,
    array<integer>(
        -1,
        34567,
        1,
        134
    )
)

//?
//? test: is_in<integer>-2
//? description: 9223372036854775807 is in array
//? expect: boolean(true)
//? source: ...

is_in<integer>(
    9223372036854775807,
    array<integer>(
        0,
        1,
        9223372036854775807,
        134
    )
)

//?
//? test: is_in<integer>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

is_in<integer>(
    to_integer(x),
    array<integer>(
        to_integer(x),
        to_integer(x)
    )
)

