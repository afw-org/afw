#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/string_eq_ignore_case.as
//? customPurpose: Part of core function tests
//? description: Tests for eq_ignore_case<string> function.
//? sourceType: script
//?
//? test: eq_ignore_case<string>-1
//? description: true
//? expect: boolean(true)
//? source: ...

eq_ignore_case<string>(
    string("abcdefghijklmnop"),
    string("AbCdEfGhijklmnop")
)

//?
//? test: eq_ignore_case<string>-2
//? description: false
//? expect: boolean(false)
//? source: ...

eq_ignore_case<string>(
    string("abcdefghijklmnop"),
    string("AbCdEfGhijklmnopq")
)

//?
//? test: eq_ignore_case<string>-3
//? description: true
//? expect: boolean(true)
//? source: ...

eq_ignore_case<string>(
    string("WILL NOT MATCH"),
    string("will not match")
)

