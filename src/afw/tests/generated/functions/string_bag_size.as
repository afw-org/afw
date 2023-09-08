#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/string_bag_size.as
//? customPurpose: Part of core function tests
//? description: Tests for bag_size<string> function.
//? sourceType: script
//?
//? test: bag_size<string>-1
//? description: String bag size of 0.
//? expect: integer(0)
//? source: ...

bag_size<string>(
    bag<string>(
    )
)

//?
//? test: bag_size<string>-2
//? description: String bag size of 2.
//? expect: integer(2)
//? source: ...

bag_size<string>(
    bag<string>(
        string("one"),
        string("two")
    )
)

//?
//? test: bag_size<string>-3
//? description: String bag size of 21.
//? expect: integer(21)
//? source: ...

bag_size<string>(
    bag<string>(
        string("one"),
        string("two"),
        string("three"),
        string("four"),
        string("five"),
        string("six"),
        string("seven"),
        string("eight"),
        string("nine"),
        string("ten"),
        string("eleven"),
        string("twelve"),
        string("thirteen"),
        string("fourteen"),
        string("fifteen"),
        string("sixteen"),
        string("seventeen"),
        string("eighteen"),
        string("nineteen"),
        string("twenty"),
        string("twenty-one")
    )
)

