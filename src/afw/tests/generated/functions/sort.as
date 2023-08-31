#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/sort.as
//? customPurpose: Part of core function tests
//? description: Tests for sort function.
//? sourceType: expression
//?
//? test: sort-1
//? description: Test sort function with integers.
//? expect: ...

bag<integer>(
    integer(0),
    integer(2),
    integer(5),
    integer(9)
)

//? source: ...

sort(
    string("le"),
    bag<integer>(
        integer(2),
        integer(9),
        integer(5),
        integer(0)
    )
)

//?
//? test: sort-2
//? description: Test sort function with strings.
//? expect: ...

bag<string>(
    string("zzzzz"),
    string("nnnnn"),
    string("mmmmm"),
    string("aaaaa")
)

//? source: ...

sort(
    string("gt"),
    bag<string>(
        string("mmmmm"),
        string("zzzzz"),
        string("aaaaa"),
        string("nnnnn")
    )
)

