#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/date_gt.as
//? customPurpose: Part of core function tests
//? description: Tests for gt<date> function.
//? sourceType: script
//?
//? test: gt<date>-1
//? description: true
//? expect: boolean(true)
//? source: ...

gt<date>(
    date("1970-01-02"),
    date("1970-01-01")
)

//?
//? test: gt<date>-2
//? description: equal
//? expect: boolean(false)
//? source: ...

gt<date>(
    date("1970-01-02"),
    date("1970-01-02")
)

//?
//? test: gt<date>-3
//? description: false
//? expect: boolean(false)
//? source: ...

gt<date>(
    date("1970-01-02-05:00"),
    date("1970-01-02-05:00")
)

