#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/integer_multiply.as
//? customPurpose: Part of core function tests
//? description: Tests for multiply<integer> function.
//? sourceType: script
//?
//? test: multiply<integer>-1
//? description: 5 x 4 x 3 x 2 x 1
//? expect: 120
//? source: ...

multiply<integer>(
    5,
    4,
    3,
    2,
    1
)

//?
//? test: multiply<integer>-2
//? description: -5 x -4 x -3 x 2 x 1
//? expect: -120
//? source: ...

multiply<integer>(
    -5,
    -4,
    -3,
    2,
    1
)

//?
//? test: multiply<integer>-3
//? description: Positive Overflow
//? expect: error
//? source: ...

multiply<integer>(
    -4,
    -3,
    2,
    9223372036854775806,
    1
)

//?
//? test: multiply<integer>-4
//? skip: true
//? description: Negative Overflow
//? expect: error
//? source: ...

multiply<integer>(
    4,
    3,
    2,
    -9223372036854775806,
    1
)

//?
//? test: multiply<integer>-5
//? description: Invalid arguments test.
//? expect: error
//? source: ...

multiply<integer>(
    to_integer(x)
)

