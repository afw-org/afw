#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/date_one_and_only.as
//? customPurpose: Part of core function tests
//? description: Tests for one_and_only<date> function.
//? sourceType: script
//?
//? test: one_and_only<date>-1
//? description: One
//? expect: "2004-04-12-05:00"date
//? source: ...

one_and_only<date>(
    bag<date>(
        "2004-04-12-05:00"date
    )
)

//?
//? test: one_and_only<date>-2
//? description: Two
//? expect: error
//? source: ...

one_and_only<date>(
    bag<date>(
        "2004-04-12-05:00"date,
        "2005-04-12-05:00"date
    )
)

