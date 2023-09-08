#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/string_to_integer.as
//? customPurpose: Part of core function tests
//? description: Tests for to_integer<string> function.
//? sourceType: script
//?
//? test: to_integer<string>-1
//? description: integer from string test 123
//? expect: integer(123)
//? source: ...

to_integer<string>(
    string("123")
)

//?
//? test: to_integer<string>-2
//? description: integer from string test -123
//? expect: integer(-123)
//? source: ...

to_integer<string>(
    string("-123")
)

//?
//? test: to_integer<string>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

to_integer<string>(
    string("x")
)

