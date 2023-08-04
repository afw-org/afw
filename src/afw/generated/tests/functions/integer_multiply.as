#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/integer_multiply.as
//? customPurpose: Part of core function tests
//? description: Tests for multiply<integer> function.
//? sourceType: expression
//?
//? test: multiply<integer>-1
//? description: 5 x 4 x 3 x 2 x 1
//? expect: integer(120)
//? source: ...

multiply<integer>(
    integer(5),
    integer(4),
    integer(3),
    integer(2),
    integer(1)
)

//?
//? test: multiply<integer>-2
//? description: -5 x -4 x -3 x 2 x 1
//? expect: integer(-120)
//? source: ...

multiply<integer>(
    integer(-5),
    integer(-4),
    integer(-3),
    integer(2),
    integer(1)
)

//?
//? test: multiply<integer>-3
//? description: Positive Overflow
//? expect: error
//? source: ...

multiply<integer>(
    integer(-4),
    integer(-3),
    integer(2),
    integer(9223372036854775806),
    integer(1)
)

//?
//? test: multiply<integer>-4
//? skip: true
//? description: Negative Overflow
//? expect: error
//? source: ...

multiply<integer>(
    integer(4),
    integer(3),
    integer(2),
    integer(-9223372036854775806),
    integer(1)
)

//?
//? test: multiply<integer>-5
//? description: Invalid arguments test.
//? expect: error
//? source: ...

multiply<integer>(
    integer(x)
)

