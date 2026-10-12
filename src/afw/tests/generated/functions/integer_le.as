#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/integer_le.as
//? customPurpose: Part of core function tests
//? description: Tests for le<integer> function.
//? sourceType: script
//?
//? test: le<integer>-1
//? description: 0 less than or equal 0
//? expect: boolean(true)
//? source: ...

le<integer>(
    0,
    0
)

//?
//? test: le<integer>-2
//? description: 9223372036854775806 less than 9223372036854775807
//? expect: boolean(true)
//? source: ...

le<integer>(
    9223372036854775806,
    9223372036854775807
)

//?
//? test: le<integer>-3
//? description: 0 is not less than -1
//? expect: boolean(false)
//? source: ...

le<integer>(
    0,
    -1
)

//?
//? test: le<integer>-4
//? description: Invalid arguments test.
//? expect: error
//? source: ...

le<integer>(
    to_integer(x),
    to_integer(x)
)

