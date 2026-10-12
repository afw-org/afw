#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/integer_divide.as
//? customPurpose: Part of core function tests
//? description: Tests for divide<integer> function.
//? sourceType: script
//?
//? test: divide<integer>-1
//? description: Divide 200 by 5
//? expect: 40
//? source: ...

divide<integer>(
    200,
    5
)

//?
//? test: divide<integer>-2
//? description: Divide 9223372036854775807 by 2
//? expect: 4611686018427387903
//? source: ...

divide<integer>(
    9223372036854775807,
    2
)

//?
//? test: divide<integer>-3
//? description: Divide 200 by 0
//? expect: error
//? source: ...

divide<integer>(
    200,
    0
)

//?
//? test: divide<integer>-4
//? description: Invalid arguments test.
//? expect: error
//? source: ...

divide<integer>(
    to_integer(x)
)

