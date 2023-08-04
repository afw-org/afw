#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/integer_ge.as
//? customPurpose: Part of core function tests
//? description: Tests for ge<integer> function.
//? sourceType: expression
//?
//? test: ge<integer>-1
//? description: 9223372036854775807 >= 9223372036854775807
//? expect: boolean(true)
//? source: ...

ge<integer>(
    integer(9223372036854775807),
    integer(9223372036854775807)
)

//?
//? test: ge<integer>-2
//? description: 9223372036854775807 >= 9223372036854775806
//? expect: boolean(true)
//? source: ...

ge<integer>(
    integer(9223372036854775807),
    integer(9223372036854775806)
)

//?
//? test: ge<integer>-3
//? description: -9223372036854775807 >= -9223372036854775806
//? expect: boolean(false)
//? source: ...

ge<integer>(
    integer(-9223372036854775807),
    integer(-9223372036854775806)
)

//?
//? test: ge<integer>-4
//? description: 9223372036854775807 >= -9223372036854775806
//? expect: boolean(true)
//? source: ...

ge<integer>(
    integer(9223372036854775807),
    integer(-9223372036854775806)
)

//?
//? test: ge<integer>-5
//? description: Invalid arguments test.
//? expect: error
//? source: ...

ge<integer>(
    integer(x),
    integer(x)
)

