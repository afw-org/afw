#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/string_eq.as
//? customPurpose: Part of core function tests
//? description: Tests for eq<string> function.
//? sourceType: script
//?
//? test: eq<string>-1
//? description: true
//? expect: boolean(true)
//? source: ...

eq<string>(
    string("AbCDefGH lorem ipsum"),
    string("AbCDefGH lorem ipsum")
)

//?
//? test: eq<string>-2
//? description: false
//? expect: boolean(false)
//? source: ...

eq<string>(
    string("AbCDefGH lorem ipsum"),
    string("AbCDefGH lorem ipsumm")
)

