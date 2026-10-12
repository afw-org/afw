#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/filter.as
//? customPurpose: Part of core function tests
//? description: Tests for filter function.
//? sourceType: script
//?
//? test: filter-1
//? description: Test filter function with integers.
//? expect: ...

bag<integer>(
    4,
    2,
    0
)

//? source: ...

filter(
    string("ge"),
    4,
    bag<integer>(
        4,
        2,
        9,
        0
    )
)

//?
//? test: filter-2
//? description: Test filter function with strings.
//? expect: ...

bag<string>(
    string("aaaaa")
)

//? source: ...

filter(
    string("gt"),
    string("mmmmm"),
    bag<string>(
        string("zzzzz"),
        string("mmmmm"),
        string("aaaaa"),
        string("nnnnn")
    )
)

