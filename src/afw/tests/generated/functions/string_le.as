#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/string_le.as
//? customPurpose: Part of core function tests
//? description: Tests for le<string> function.
//? sourceType: expression
//?
//? test: le<string>-1
//? description: one
//? expect: boolean(false)
//? source: ...

le<string>(
    string("abc"),
    string("ABC")
)

//?
//? test: le<string>-2
//? description: two
//? expect: boolean(true)
//? source: ...

le<string>(
    string("ABC"),
    string("abc")
)

//?
//? test: le<string>-3
//? description: equal
//? expect: boolean(true)
//? source: ...

le<string>(
    string("abc"),
    string("abc")
)

//?
//? test: le<string>-4
//? description: four
//? expect: boolean(false)
//? source: ...

le<string>(
    string("abcd"),
    string("abc")
)

//?
//? test: le<string>-5
//? description: four
//? expect: boolean(true)
//? source: ...

le<string>(
    string("abc"),
    string("abcd")
)

