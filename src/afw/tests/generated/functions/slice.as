#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/slice.as
//? customPurpose: Part of core function tests
//? description: Tests for slice function.
//? sourceType: script
//?
//? test: slice-1
//? description: Test slice function with integers, no other parms.
//? expect: ...

bag<integer>(
    integer(2),
    integer(9),
    integer(5),
    integer(0)
)

//? source: ...

slice(
    bag<integer>(
        integer(2),
        integer(9),
        integer(5),
        integer(0)
    )
)

//?
//? test: slice-2
//? description: ...
Test slice function with integers with startIndex and endIndex.
//? expect: ...

bag<integer>(
    integer(9),
    integer(5)
)

//? source: ...

slice(
    bag<integer>(
        integer(2),
        integer(9),
        integer(5),
        integer(0)
    ),
    integer(1),
    integer(3)
)

//?
//? test: slice-3
//? description: ...
Test slice function with integers with negative startIndex and endIndex.
//? expect: ...

bag<integer>(
    integer(9),
    integer(5)
)

//? source: ...

slice(
    bag<integer>(
        integer(2),
        integer(9),
        integer(5),
        integer(0)
    ),
    integer(-3),
    integer(-1)
)

//?
//? test: slice-4
//? description: Test slice function with strings with just startIndex.
//? expect: ...

bag<string>(
    string("aaaaa"),
    string("nnnnn")
)

//? source: ...

slice(
    bag<string>(
        string("mmmmm"),
        string("zzzzz"),
        string("aaaaa"),
        string("nnnnn")
    ),
    integer(2)
)

