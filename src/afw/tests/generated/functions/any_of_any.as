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
    array<string>(
        string("Ringo"),
        string("Mary")
    ),
    array<string>(
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
    array<integer>(
        10000006
    ),
    array<integer>(
        10000006
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

//?
//? test: any_of_any-empty-untyped-arrays
//? description: any_of_any of empty untyped arrays is false (nothing to match), as in ECMAScript some(); it threw "must both be typed"
//? expect: false
//? source: ...

any_of_any(
    string("eq<string>"),
    [],
    []
)

