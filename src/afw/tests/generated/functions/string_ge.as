#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/string_ge.as
//? customPurpose: Part of core function tests
//? description: Tests for ge<string> function.
//? sourceType: script
//?
//? test: ge<string>-1
//? description: one
//? expect: boolean(true)
//? source: ...

ge<string>(
    string("abc"),
    string("ABC")
)

//?
//? test: ge<string>-2
//? description: two
//? expect: boolean(false)
//? source: ...

ge<string>(
    string("ABC"),
    string("abc")
)

//?
//? test: ge<string>-3
//? description: equal
//? expect: boolean(true)
//? source: ...

ge<string>(
    string("abc"),
    string("abc")
)

//?
//? test: ge<string>-4
//? description: four
//? expect: boolean(true)
//? source: ...

ge<string>(
    string("abcd"),
    string("abc")
)

//?
//? test: ge<string>-5
//? description: four
//? expect: boolean(false)
//? source: ...

ge<string>(
    string("abc"),
    string("abcd")
)

