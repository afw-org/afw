#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/date_ge.as
//? customPurpose: Part of core function tests
//? description: Tests for ge<date> function.
//? sourceType: script
//?
//? test: ge<date>-1
//? description: true
//? expect: boolean(true)
//? source: ...

ge<date>(
    date("1970-01-02"),
    date("1970-01-01")
)

//?
//? test: ge<date>-2
//? description: true
//? expect: boolean(true)
//? source: ...

ge<date>(
    date("1970-01-02"),
    date("1970-01-02")
)

//?
//? test: ge<date>-3
//? description: false
//? expect: boolean(true)
//? source: ...

ge<date>(
    date("1970-01-02-05:00"),
    date("1970-01-02-05:00")
)

