#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/string_array.as
//? customPurpose: Part of core function tests
//? description: Tests for array<string> function.
//? sourceType: script
//?
//? test: array<string>-1
//? description: Create empty string array.
//? expect: ...

array<string>(
)

//? source: array<string>()
//?
//? test: array<string>-2
//? description: Create string array with 2 strings.
//? expect: ...

array<string>(
    string("one"),
    string("two")
)

//? source: ...

array<string>(
    string("one"),
    string("two")
)

//?
//? test: array<string>-3
//? description: Create string array with 21 strings.
//? expect: ...

array<string>(
    string("one"),
    string("two"),
    string("three"),
    string("four"),
    string("five"),
    string("six"),
    string("seven"),
    string("eight"),
    string("nine"),
    string("ten"),
    string("eleven"),
    string("twelve"),
    string("thirteen"),
    string("fourteen"),
    string("fifteen"),
    string("sixteen"),
    string("seventeen"),
    string("eighteen"),
    string("nineteen"),
    string("twenty"),
    string("twenty-one")
)

//? source: ...

array<string>(
    string("one"),
    string("two"),
    string("three"),
    string("four"),
    string("five"),
    string("six"),
    string("seven"),
    string("eight"),
    string("nine"),
    string("ten"),
    string("eleven"),
    string("twelve"),
    string("thirteen"),
    string("fourteen"),
    string("fifteen"),
    string("sixteen"),
    string("seventeen"),
    string("eighteen"),
    string("nineteen"),
    string("twenty"),
    string("twenty-one")
)

