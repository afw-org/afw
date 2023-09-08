#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/integer_gt.as
//? customPurpose: Part of core function tests
//? description: Tests for gt<integer> function.
//? sourceType: script
//?
//? test: gt<integer>-1
//? description: 9223372036854775807 > 9223372036854775807
//? expect: boolean(false)
//? source: ...

gt<integer>(
    integer(9223372036854775807),
    integer(9223372036854775807)
)

//?
//? test: gt<integer>-2
//? description: 9223372036854775807 > 9223372036854775806
//? expect: boolean(true)
//? source: ...

gt<integer>(
    integer(9223372036854775807),
    integer(9223372036854775806)
)

//?
//? test: gt<integer>-3
//? description: -9223372036854775807 > -9223372036854775806
//? expect: boolean(false)
//? source: ...

gt<integer>(
    integer(-9223372036854775807),
    integer(-9223372036854775806)
)

//?
//? test: gt<integer>-4
//? description: 9223372036854775807 > -9223372036854775806
//? expect: boolean(true)
//? source: ...

gt<integer>(
    integer(9223372036854775807),
    integer(-9223372036854775806)
)

//?
//? test: gt<integer>-5
//? description: Invalid arguments test.
//? expect: error
//? source: ...

gt<integer>(
    integer(x),
    integer(x)
)

