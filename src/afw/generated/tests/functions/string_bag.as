#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/string_bag.as
//? customPurpose: Part of core function tests
//? description: Tests for bag<string> function.
//? sourceType: expression
//?
//? test: bag<string>-1
//? description: Create empty string bag.
//? expect: ...

bag<string>(
)

//? source: bag<string>()
//?
//? test: bag<string>-2
//? description: Create string bag with 2 strings.
//? expect: ...

bag<string>(
    string("one"),
    string("two")
)

//? source: ...

bag<string>(
    string("one"),
    string("two")
)

//?
//? test: bag<string>-3
//? description: Create string bag with 21 strings.
//? expect: ...

bag<string>(
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

bag<string>(
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

