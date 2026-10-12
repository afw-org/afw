#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/integer_add.as
//? customPurpose: Part of core function tests
//? description: Tests for add<integer> function.
//? sourceType: script
//?
//? test: add<integer>-1
//? description: Add 67 and -1
//? expect: 66
//? source: ...

add<integer>(
    67,
    -1
)

//?
//? test: add<integer>-2
//? description: Add 9223372036854775807 and -1
//? expect: 9223372036854775803
//? source: ...

add<integer>(
    9223372036854775807,
    -1,
    -1,
    -1,
    -1
)

//?
//? test: add<integer>-3
//? description: Add -9223372036854775807 and 7
//? expect: -9223372036854775800
//? source: ...

add<integer>(
    -9223372036854775807,
    7
)

//?
//? test: add<integer>-4
//? description: Add 9223372036854775807 and 10 which will overflow
//? expect: error
//? source: ...

add<integer>(
    9223372036854775807,
    10
)

//?
//? test: add<integer>-5
//? description: Invalid arguments test.
//? expect: error
//? source: ...

add<integer>(
    to_integer(x)
)

