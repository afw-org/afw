#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/all_of.as
//? customPurpose: Part of core function tests
//? description: Tests for all_of function.
//? sourceType: script
//?
//? test: all_of-1
//? description: ...
True because the first argument (10) is greater than all of the elements of the array (9,3,4 and 2).
//? expect: boolean(true)
//? source: ...

all_of(
    string("gt"),
    10,
    array<integer>(
        4,
        2,
        9,
        0
    )
)

//?
//? test: all_of-2
//? description: ...
false because the first argument (10) is not greater than all of the elements of the array (9,3,4 and 2).
//? expect: boolean(false)
//? source: ...

all_of(
    string("gt"),
    10,
    array<integer>(
        4,
        2,
        9,
        10
    )
)

//?
//? test: all_of-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

all_of(
    string("x")
)

