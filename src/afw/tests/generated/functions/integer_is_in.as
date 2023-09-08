#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/integer_is_in.as
//? customPurpose: Part of core function tests
//? description: Tests for is_in<integer> function.
//? sourceType: script
//?
//? test: is_in<integer>-1
//? description: 0 is not in bag
//? expect: boolean(false)
//? source: ...

is_in<integer>(
    integer(0),
    bag<integer>(
        integer(-1),
        integer(34567),
        integer(1),
        integer(134)
    )
)

//?
//? test: is_in<integer>-2
//? description: 9223372036854775807 is in bag
//? expect: boolean(true)
//? source: ...

is_in<integer>(
    integer(9223372036854775807),
    bag<integer>(
        integer(0),
        integer(1),
        integer(9223372036854775807),
        integer(134)
    )
)

//?
//? test: is_in<integer>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

is_in<integer>(
    integer(x),
    bag<integer>(
        integer(x),
        integer(x)
    )
)

