#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/integer_eq.as
//? customPurpose: Part of core function tests
//? description: Tests for eq<integer> function.
//? sourceType: script
//?
//? test: eq<integer>-1
//? description: -9223372036854775807 equal
//? expect: boolean(true)
//? source: ...

eq<integer>(
    integer(-9223372036854775807),
    integer(-9223372036854775807)
)

//?
//? test: eq<integer>-2
//? description: 9223372036854775807 equal
//? expect: boolean(true)
//? source: ...

eq<integer>(
    integer(9223372036854775807),
    integer(9223372036854775807)
)

//?
//? test: eq<integer>-3
//? description: -10 and 10 not equal
//? expect: boolean(false)
//? source: ...

eq<integer>(
    integer(-10),
    integer(10)
)

//?
//? test: eq<integer>-4
//? description: Invalid arguments test.
//? expect: error
//? source: ...

eq<integer>(
    integer(x),
    integer(x)
)

