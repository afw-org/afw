#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/join.as
//? customPurpose: Part of core function tests
//? description: Tests for join function.
//? sourceType: script
//?
//? test: join-1
//? description: Test join function with integers.
//? expect: string("4,2,9,0")
//? source: ...

join(
    array<integer>(
        4,
        2,
        9,
        0
    )
)

//?
//? test: join-2
//? description: Test join function with strings.
//? expect: string("zzzzz::mmmmm::aaaaa::nnnnn")
//? source: ...

join(
    array<string>(
        string("zzzzz"),
        string("mmmmm"),
        string("aaaaa"),
        string("nnnnn")
    ),
    string("::")
)

