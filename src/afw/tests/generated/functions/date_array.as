#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/date_array.as
//? customPurpose: Part of core function tests
//? description: Tests for array<date> function.
//? sourceType: script
//?
//? test: array<date>-1
//? description: One
//? expect: ...

array<date>(
    "2004-04-12"date,
    "-1970-01-01"date,
    "2004-04-12"date,
    "2004-04-12-05:00"date,
    "2004-04-12Z"date
)

//? source: ...

array<date>(
    "2004-04-12"date,
    "-1970-01-01"date,
    "2004-04-12"date,
    "2004-04-12-05:00"date,
    "2004-04-12Z"date
)

//?
//? test: array<date>-2
//? description: Empty
//? expect: ...

array<date>(
)

//? source: array<date>()
