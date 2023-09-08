#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/date_le.as
//? customPurpose: Part of core function tests
//? description: Tests for le<date> function.
//? sourceType: script
//?
//? test: le<date>-1
//? description: true
//? expect: boolean(true)
//? source: ...

le<date>(
    date("1970-01-01"),
    date("1970-01-02")
)

//?
//? test: le<date>-2
//? description: equal
//? expect: boolean(true)
//? source: ...

le<date>(
    date("1970-01-01"),
    date("1970-01-01")
)

//?
//? test: le<date>-3
//? description: false
//? expect: boolean(false)
//? source: ...

le<date>(
    date("1970-01-02"),
    date("1970-01-01")
)

