#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/reverse.as
//? customPurpose: Part of core function tests
//? description: Tests for reverse function.
//? sourceType: script
//?
//? test: reverse-1
//? description: Test reverse function with integers.
//? expect: ...

array<integer>(
    0,
    5,
    9,
    2
)

//? source: ...

reverse(
    array<integer>(
        2,
        9,
        5,
        0
    )
)

//?
//? test: reverse-2
//? description: Test reverse function with strings.
//? expect: ...

array<string>(
    string("nnnnn"),
    string("aaaaa"),
    string("zzzzz"),
    string("mmmmm")
)

//? source: ...

reverse(
    array<string>(
        string("mmmmm"),
        string("zzzzz"),
        string("aaaaa"),
        string("nnnnn")
    )
)

