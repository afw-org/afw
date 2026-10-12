#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/string_to_date.as
//? customPurpose: Part of core function tests
//? description: Tests for to_date<string> function.
//? sourceType: script
//?
//? test: to_date<string>-1
//? description: One
//? expect: "1970-01-01"date
//? source: ...

to_date<string>(
    string("1970-01-01")
)

//?
//? test: to_date<string>-2
//? description: Two
//? expect: "-1970-01-01"date
//? source: ...

to_date<string>(
    string("-1970-01-01")
)

//?
//? test: to_date<string>-3
//? description: Three
//? expect: "1970-01-01+05:00"date
//? source: ...

to_date<string>(
    string("1970-01-01+05:00")
)

