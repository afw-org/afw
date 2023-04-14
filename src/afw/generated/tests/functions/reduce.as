#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/reduce.as
//? customPurpose: Part of core function tests
//? description: Tests for reduce function.
//? sourceType: expression
//?
//? test: reduce-1
//? description: Test reduce function with integers.
//? expect: integer(20)
//? source: ...

reduce(
    string("add"),
    integer(4),
    bag<integer>(
        integer(2),
        integer(9),
        integer(5),
        integer(0)
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

