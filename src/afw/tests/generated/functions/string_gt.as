#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/string_gt.as
//? customPurpose: Part of core function tests
//? description: Tests for gt<string> function.
//? sourceType: script
//?
//? test: gt<string>-1
//? description: one
//? expect: boolean(true)
//? source: ...

gt<string>(
    string("abc"),
    string("ABC")
)

//?
//? test: gt<string>-2
//? description: two
//? expect: boolean(false)
//? source: ...

gt<string>(
    string("ABC"),
    string("abc")
)

//?
//? test: gt<string>-3
//? description: equal
//? expect: boolean(false)
//? source: ...

gt<string>(
    string("abc"),
    string("abc")
)

//?
//? test: gt<string>-4
//? description: four
//? expect: boolean(true)
//? source: ...

gt<string>(
    string("abcd"),
    string("abc")
)

//?
//? test: gt<string>-5
//? description: four
//? expect: boolean(false)
//? source: ...

gt<string>(
    string("abc"),
    string("abcd")
)

