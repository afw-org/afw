#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/string_starts_with.as
//? customPurpose: Part of core function tests
//? description: Tests for starts_with<string> function.
//? sourceType: expression
//?
//? test: starts_with<string>-1
//? description: string starts with test abc abcd
//? expect: boolean(true)
//? source: ...

starts_with<string>(
    string("abcd"),
    string("abc")
)

//?
//? test: starts_with<string>-2
//? description: string starts with test abc abc
//? expect: boolean(true)
//? source: ...

starts_with<string>(
    string("abc"),
    string("abc")
)

//?
//? test: starts_with<string>-3
//? description: string starts with test abc ab
//? expect: boolean(false)
//? source: ...

starts_with<string>(
    string("ab"),
    string("abc")
)

//?
//? test: starts_with<string>-4
//? description: string starts with test abc eabcd
//? expect: boolean(false)
//? source: ...

starts_with<string>(
    string("eabcd"),
    string("abc")
)

//?
//? test: starts_with<string>-5
//? description: Invalid arguments test.
//? expect: boolean(true)
//? source: ...

starts_with<string>(
    string("x"),
    string("x")
)

