#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/join.as
//? customPurpose: Part of core function tests
//? description: Tests for join function.
//? sourceType: expression
//?
//? test: join-1
//? description: Test join function with integers.
//? expect: string("4,2,9,0")
//? source: ...

join(
    bag<integer>(
        integer(4),
        integer(2),
        integer(9),
        integer(0)
    )
)

//?
//? test: join-2
//? description: Test join function with strings.
//? expect: string("zzzzz::mmmmm::aaaaa::nnnnn")
//? source: ...

join(
    bag<string>(
        string("zzzzz"),
        string("mmmmm"),
        string("aaaaa"),
        string("nnnnn")
    ),
    string("::")
)

