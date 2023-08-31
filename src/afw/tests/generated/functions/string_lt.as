#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/string_lt.as
//? customPurpose: Part of core function tests
//? description: Tests for lt<string> function.
//? sourceType: expression
//?
//? test: lt<string>-1
//? description: one
//? expect: boolean(false)
//? source: ...

lt<string>(
    string("abc"),
    string("ABC")
)

//?
//? test: lt<string>-2
//? description: two
//? expect: boolean(true)
//? source: ...

lt<string>(
    string("ABC"),
    string("abc")
)

//?
//? test: lt<string>-3
//? description: equal
//? expect: boolean(false)
//? source: ...

lt<string>(
    string("abc"),
    string("abc")
)

//?
//? test: lt<string>-4
//? description: four
//? expect: boolean(false)
//? source: ...

lt<string>(
    string("abcd"),
    string("abc")
)

//?
//? test: lt<string>-5
//? description: four
//? expect: boolean(true)
//? source: ...

lt<string>(
    string("abc"),
    string("abcd")
)

