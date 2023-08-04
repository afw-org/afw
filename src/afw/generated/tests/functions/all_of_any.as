#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/all_of_any.as
//? customPurpose: Part of core function tests
//? description: Tests for all_of_any function.
//? sourceType: expression
//?
//? test: all_of_any-1
//? description: ...
true because each of the elements of the first bag is greater than at least one of the elements of the second bag. 
//? expect: boolean(true)
//? source: ...

all_of_any(
    string("gt"),
    bag<integer>(
        integer(6),
        integer(5)
    ),
    bag<integer>(
        integer(4),
        integer(2),
        integer(5)
    )
)

//?
//? test: all_of_any-2
//? description: ...
false because each of the elements of the first bag is not greater than at least one of the elements of the second bag. 
//? expect: boolean(false)
//? source: ...

all_of_any(
    string("gt"),
    bag<integer>(
        integer(6),
        integer(5)
    ),
    bag<integer>(
        integer(6),
        integer(5)
    )
)

//?
//? test: all_of_any-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

all_of_any(
    string("x")
)

