#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/all_of.as
//? customPurpose: Part of core function tests
//? description: Tests for all_of function.
//? sourceType: script
//?
//? test: all_of-1
//? description: ...
True because the first argument (10) is greater than all of the elements of the bag (9,3,4 and 2).
//? expect: boolean(true)
//? source: ...

all_of(
    string("gt"),
    integer(10),
    bag<integer>(
        integer(4),
        integer(2),
        integer(9),
        integer(0)
    )
)

//?
//? test: all_of-2
//? description: ...
false because the first argument (10) is not greater than all of the elements of the bag (9,3,4 and 2).
//? expect: boolean(false)
//? source: ...

all_of(
    string("gt"),
    integer(10),
    bag<integer>(
        integer(4),
        integer(2),
        integer(9),
        integer(10)
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

