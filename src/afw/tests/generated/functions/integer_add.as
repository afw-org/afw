#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/integer_add.as
//? customPurpose: Part of core function tests
//? description: Tests for add<integer> function.
//? sourceType: script
//?
//? test: add<integer>-1
//? description: Add 67 and -1
//? expect: integer(66)
//? source: ...

add<integer>(
    integer(67),
    integer(-1)
)

//?
//? test: add<integer>-2
//? description: Add 9223372036854775807 and -1
//? expect: integer(9223372036854775803)
//? source: ...

add<integer>(
    integer(9223372036854775807),
    integer(-1),
    integer(-1),
    integer(-1),
    integer(-1)
)

//?
//? test: add<integer>-3
//? description: Add -9223372036854775807 and 7
//? expect: integer(-9223372036854775800)
//? source: ...

add<integer>(
    integer(-9223372036854775807),
    integer(7)
)

//?
//? test: add<integer>-4
//? description: Add 9223372036854775807 and 10 which will overflow
//? expect: error
//? source: ...

add<integer>(
    integer(9223372036854775807),
    integer(10)
)

//?
//? test: add<integer>-5
//? description: Invalid arguments test.
//? expect: error
//? source: ...

add<integer>(
    integer(x)
)

