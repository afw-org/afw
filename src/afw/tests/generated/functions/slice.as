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

array<integer>(
    2,
    9,
    5,
    0
)

//? source: ...

slice(
    array<integer>(
        2,
        9,
        5,
        0
    )
)

//?
//? test: slice-2
//? description: ...
Test slice function with integers with startIndex and endIndex.
//? expect: ...

array<integer>(
    9,
    5
)

//? source: ...

slice(
    array<integer>(
        2,
        9,
        5,
        0
    ),
    1,
    3
)

//?
//? test: slice-3
//? description: ...
Test slice function with integers with negative startIndex and endIndex.
//? expect: ...

array<integer>(
    9,
    5
)

//? source: ...

slice(
    array<integer>(
        2,
        9,
        5,
        0
    ),
    -3,
    -1
)

//?
//? test: slice-4
//? description: Test slice function with strings with just startIndex.
//? expect: ...

array<string>(
    string("aaaaa"),
    string("nnnnn")
)

//? source: ...

slice(
    array<string>(
        string("mmmmm"),
        string("zzzzz"),
        string("aaaaa"),
        string("nnnnn")
    ),
    2
)

