#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/integer_divide.as
//? customPurpose: Part of core function tests
//? description: Tests for divide<integer> function.
//? sourceType: expression
//?
//? test: divide<integer>-1
//? description: Divide 200 by 5
//? expect: integer(40)
//? source: ...

divide<integer>(
    integer(200),
    integer(5)
)

//?
//? test: divide<integer>-2
//? description: Divide 9223372036854775807 by 2
//? expect: integer(4611686018427387903)
//? source: ...

divide<integer>(
    integer(9223372036854775807),
    integer(2)
)

//?
//? test: divide<integer>-3
//? description: Divide 200 by 0
//? expect: error
//? source: ...

divide<integer>(
    integer(200),
    integer(0)
)

//?
//? test: divide<integer>-4
//? description: Invalid arguments test.
//? expect: error
//? source: ...

divide<integer>(
    integer(x)
)

