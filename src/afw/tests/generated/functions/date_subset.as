#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/date_subset.as
//? customPurpose: Part of core function tests
//? description: Tests for subset<date> function.
//? sourceType: script
//?
//? test: subset<date>-1
//? description: x
//? expect: boolean(true)
//? source: ...

subset<date>(
    bag<date>(
        date("2004-04-12"),
        date("-1970-01-01"),
        date("2004-04-12"),
        date("2004-04-12"),
        date("2004-04-12")
    ),
    bag<date>(
        date("2004-04-12"),
        date("-1970-01-01"),
        date("2004-04-12"),
        date("2004-04-12"),
        date("2004-04-12")
    )
)

//?
//? test: subset<date>-2
//? description: x
//? expect: boolean(true)
//? source: ...

subset<date>(
    bag<date>(
        date("2004-04-12"),
        date("2004-04-12"),
        date("2004-04-12"),
        date("2004-04-12")
    ),
    bag<date>(
        date("2004-04-12"),
        date("-1970-01-01"),
        date("2004-04-12"),
        date("2004-04-12"),
        date("2004-04-12")
    )
)

