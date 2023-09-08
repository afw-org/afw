#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/integer_abs.as
//? customPurpose: Part of core function tests
//? description: Tests for abs<integer> function.
//? sourceType: script
//?
//? test: abs<integer>-1
//? description: Absolute value of -45.6
//? expect: integer(45)
//? source: ...

abs<integer>(
    integer(-45)
)

//?
//? test: abs<integer>-2
//? description: Absolute value of 5556
//? expect: integer(5556)
//? source: ...

abs<integer>(
    integer(5556)
)

//?
//? test: abs<integer>-3
//? description: ...
Absolute value of -9,223,372,036,854,775,808 largest negative 64 bit integer
//? expect: integer(9223372036854775807)
//? source: ...

abs<integer>(
    integer(-9223372036854775807)
)

//?
//? test: abs<integer>-4
//? description: ...
Absolute value of 9,223,372,036,854,775,807 largest positive 64 bit integer
//? expect: integer(9223372036854775807)
//? source: ...

abs<integer>(
    integer(9223372036854775807)
)

//?
//? test: abs<integer>-5
//? description: Invalid arguments test.
//? expect: error
//? source: ...

abs<integer>(
    integer(x)
)

