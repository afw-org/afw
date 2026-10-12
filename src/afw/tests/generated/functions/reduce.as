#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/reduce.as
//? customPurpose: Part of core function tests
//? description: Tests for reduce function.
//? sourceType: script
//?
//? test: reduce-1
//? description: Test reduce function with integers.
//? expect: 20
//? source: ...

reduce(
    string("add"),
    4,
    bag<integer>(
        2,
        9,
        5,
        0
    )
)

//?
//? test: reduce-2
//? description: Test reduce function with strings.
//? expect: string("mmmmmmmmmmzzzzzaaaaannnnn")
//? source: ...

reduce(
    string("concat"),
    string("mmmmm"),
    bag<string>(
        string("mmmmm"),
        string("zzzzz"),
        string("aaaaa"),
        string("nnnnn")
    )
)

