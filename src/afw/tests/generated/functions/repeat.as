#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/repeat.as
//? customPurpose: Part of core function tests
//? description: Tests for polymorphic functions.
//? sourceType: script
//?
//? test: repeat-1
//? description: repeat a string 3 times
//? expect: string("ababab")
//? source: ...

repeat(
    string("ab"),
    integer(3)
)

//?
//? test: repeat-error-exceeds-limit
//? description: repeat throws when times exceeds 1000
//? expect: error:Parameter times exceeds 1000
//? source: ...

repeat(
    string("ab"),
    integer(1001)
)
