#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/string_ends_with.as
//? customPurpose: Part of core function tests
//? description: Tests for ends_with<string> function.
//? sourceType: script
//?
//? test: ends_with<string>-1
//? description: ends with true
//? expect: boolean(true)
//? source: ...

ends_with<string>(
    string("first string then second string ends with"),
    string("second string ends with")
)

//?
//? test: ends_with<string>-2
//? description: ends with false
//? expect: boolean(false)
//? source: ...

ends_with<string>(
    string("second string ends with"),
    string("first string then second string ends with")
)

//?
//? test: ends_with<string>-3
//? description: equal
//? expect: boolean(true)
//? source: ...

ends_with<string>(
    string("string ends_with"),
    string("string ends_with")
)

