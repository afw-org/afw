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

bag<integer>(
    integer(0),
    integer(5),
    integer(9),
    integer(2)
)

//? source: ...

reverse(
    bag<integer>(
        integer(2),
        integer(9),
        integer(5),
        integer(0)
    )
)

//?
//? test: reverse-2
//? description: Test reverse function with strings.
//? expect: ...

bag<string>(
    string("nnnnn"),
    string("aaaaa"),
    string("zzzzz"),
    string("mmmmm")
)

//? source: ...

reverse(
    bag<string>(
        string("mmmmm"),
        string("zzzzz"),
        string("aaaaa"),
        string("nnnnn")
    )
)

