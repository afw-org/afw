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

array<integer>(
    4,
    2,
    0
)

//? source: ...

filter(
    array<integer>(
        4,
        2,
        9,
        0
    ),
    function (v) { return ge(4, v); }
)

//?
//? test: filter-2
//? description: Test filter function with strings.
//? expect: ...

array<string>(
    string("aaaaa")
)

//? source: ...

filter(
    array<string>(
        string("zzzzz"),
        string("mmmmm"),
        string("aaaaa"),
        string("nnnnn")
    ),
    function (v) { return gt("mmmmm", v); }
)

