#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/integer_to_string.as
//? customPurpose: Part of core function tests
//? description: Tests for to_string<integer> function.
//? sourceType: expression
//?
//? test: to_string<integer>-1
//? description: String from integer 0
//? expect: string("0")
//? source: ...

to_string<integer>(
    integer(0)
)

//?
//? test: to_string<integer>-2
//? description: String from integer -10982
//? expect: string("-10982")
//? source: ...

to_string<integer>(
    integer(-10982)
)

//?
//? test: to_string<integer>-3
//? description: String from integer 24024
//? expect: string("24024")
//? source: ...

to_string<integer>(
    integer(24024)
)

