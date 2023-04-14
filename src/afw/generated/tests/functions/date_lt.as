#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/date_lt.as
//? customPurpose: Part of core function tests
//? description: Tests for lt<date> function.
//? sourceType: expression
//?
//? test: lt<date>-1
//? description: true
//? expect: boolean(true)
//? source: ...

lt<date>(
    date("1970-01-01"),
    date("1970-01-02")
)

//?
//? test: lt<date>-2
//? description: equal
//? expect: boolean(false)
//? source: ...

lt<date>(
    date("1970-01-01"),
    date("1970-01-01")
)

//?
//? test: lt<date>-3
//? description: false
//? expect: boolean(false)
//? source: ...

lt<date>(
    date("1970-01-02"),
    date("1970-01-01")
)

