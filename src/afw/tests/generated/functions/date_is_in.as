#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/date_is_in.as
//? customPurpose: Part of core function tests
//? description: Tests for is_in<date> function.
//? sourceType: script
//?
//? test: is_in<date>-1
//? description: true
//? expect: boolean(true)
//? source: ...

is_in<date>(
    "1970-01-01"date,
    array<date>(
        "1970-01-01"date,
        "1980-01-01"date
    )
)

//?
//? test: is_in<date>-2
//? description: false
//? expect: boolean(false)
//? source: ...

is_in<date>(
    "-1970-01-01"date,
    array<date>(
        "1970-01-01"date,
        "1980-01-01"date
    )
)

