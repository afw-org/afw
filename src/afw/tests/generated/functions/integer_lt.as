#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/integer_lt.as
//? customPurpose: Part of core function tests
//? description: Tests for lt<integer> function.
//? sourceType: script
//?
//? test: lt<integer>-1
//? description: 0 less than 0
//? expect: boolean(false)
//? source: ...

lt<integer>(
    0,
    0
)

//?
//? test: lt<integer>-2
//? description: 9223372036854775806 less than 9223372036854775807
//? expect: boolean(true)
//? source: ...

lt<integer>(
    9223372036854775806,
    9223372036854775807
)

//?
//? test: lt<integer>-3
//? description: -1 less than 0
//? expect: boolean(true)
//? source: ...

lt<integer>(
    -1,
    0
)

//?
//? test: lt<integer>-4
//? description: Invalid arguments test.
//? expect: error
//? source: ...

lt<integer>(
    to_integer(x),
    to_integer(x)
)

