#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/string_substring.as
//? customPurpose: Part of core function tests
//? description: Tests for substring<string> function.
//? sourceType: expression
//?
//? test: substring<string>-1
//? description: string substring test abc 0 1
//? expect: string("a")
//? source: ...

substring<string>(
    string("abc"),
    integer(0),
    integer(1)
)

//?
//? test: substring<string>-2
//? description: string substring test abc 1 2
//? expect: string("b")
//? source: ...

substring<string>(
    string("abc"),
    integer(1),
    integer(2)
)

//?
//? test: substring<string>-3
//? description: string substring test abc 1 -1
//? expect: string("bc")
//? source: ...

substring<string>(
    string("abc"),
    integer(1),
    integer(-1)
)

