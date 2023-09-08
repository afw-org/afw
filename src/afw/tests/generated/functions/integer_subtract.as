#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/integer_subtract.as
//? customPurpose: Part of core function tests
//? description: Tests for subtract<integer> function.
//? sourceType: script
//?
//? test: subtract<integer>-1
//? description: Subtract 67 and 1
//? expect: integer(66)
//? source: ...

subtract<integer>(
    integer(67),
    integer(1)
)

//?
//? test: subtract<integer>-2
//? description: Subtract -9223372036854775807 and -7
//? expect: integer(-9223372036854775800)
//? source: ...

subtract<integer>(
    integer(-9223372036854775807),
    integer(-7)
)

//?
//? test: subtract<integer>-3
//? description: Subtract 9223372036854775807 and 10
//? expect: integer(9223372036854775797)
//? source: ...

subtract<integer>(
    integer(9223372036854775807),
    integer(10)
)

//?
//? test: subtract<integer>-4
//? description: Subtract 9223372036854775807 and -1 which will overflow
//? expect: error
//? source: ...

subtract<integer>(
    integer(9223372036854775807),
    integer(-1)
)

//?
//? test: subtract<integer>-5
//? description: Invalid arguments test.
//? expect: error
//? source: ...

subtract<integer>(
    integer(x),
    integer(x)
)

