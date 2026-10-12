#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/sort.as
//? customPurpose: Part of core function tests
//? description: Tests for sort function.
//? sourceType: script
//?
//? test: sort-1
//? description: Test sort function with integers.
//? expect: ...

array<integer>(
    0,
    2,
    5,
    9
)

//? source: ...

sort(
    string("le"),
    array<integer>(
        2,
        9,
        5,
        0
    )
)

//?
//? test: sort-2
//? description: Test sort function with strings.
//? expect: ...

array<string>(
    string("zzzzz"),
    string("nnnnn"),
    string("mmmmm"),
    string("aaaaa")
)

//? source: ...

sort(
    string("gt"),
    array<string>(
        string("mmmmm"),
        string("zzzzz"),
        string("aaaaa"),
        string("nnnnn")
    )
)

//?
//? test: sort-empty-untyped-array
//? description: sort of an empty untyped array is an empty array, as in ECMAScript; it threw "sort() requires array to be typed"
//? expect: array()
//? source: ...

sort(
    string("le<integer>"),
    []
)

