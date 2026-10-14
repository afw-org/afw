#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/find.as
//? customPurpose: Part of core function tests
//? description: Tests for find function.
//? sourceType: script
//?
//? test: find-1
//? description: Test find function with integers.
//? expect: 9
//? source: ...

find(
    array<integer>(
        2,
        9,
        4,
        0
    ),
    function (v) { return lt(4, v); }
)

//?
//? test: find-2
//? description: Test find function with strings.
//? expect: string("aaaaa")
//? source: ...

find(
    array<string>(
        string("mmmmm"),
        string("zzzzz"),
        string("aaaaa"),
        string("nnnnn")
    ),
    function (v) { return gt("mmmmm", v); }
)

