#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/any_of_any.as
//? customPurpose: Part of core function tests
//? description: Tests for any_of_any function.
//? sourceType: script
//?
//? test: any_of_any-1
//? description: x
//? expect: boolean(true)
//? source: ...

any_of_any(
    string("eq<string>"),
    bag<string>(
        string("Ringo"),
        string("Mary")
    ),
    bag<string>(
        string("Jonh"),
        string("Paul"),
        string("George"),
        string("Ringo")
    )
)

//?
//? test: any_of_any-2
//? description: x
//? expect: boolean(false)
//? source: ...

any_of_any(
    string("gt<integer>"),
    bag<integer>(
        integer(10000006)
    ),
    bag<integer>(
        integer(10000006)
    )
)

//?
//? test: any_of_any-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

any_of_any(
    string("x")
)

