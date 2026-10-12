#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/date_eq.as
//? customPurpose: Part of core function tests
//? description: Tests for eq<date> function.
//? sourceType: script
//?
//? test: eq<date>-1
//? description: False
//? expect: boolean(false)
//? source: ...

eq<date>(
    "1970-01-01"date,
    "-1970-01-01"date
)

//?
//? test: eq<date>-2
//? description: True
//? expect: boolean(true)
//? source: ...

eq<date>(
    "1970-01-01"date,
    "1970-01-01"date
)

//?
//? test: eq<date>-3
//? description: false
//? expect: boolean(true)
//? source: ...

eq<date>(
    "2004-04-12"date,
    "2004-04-12"date
)

