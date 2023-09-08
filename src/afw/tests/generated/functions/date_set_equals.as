#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/date_set_equals.as
//? customPurpose: Part of core function tests
//? description: Tests for set_equals<date> function.
//? sourceType: script
//?
//? test: set_equals<date>-1
//? description: x
//? expect: boolean(true)
//? source: ...

set_equals<date>(
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
//? test: set_equals<date>-2
//? description: x
//? expect: boolean(false)
//? source: ...

set_equals<date>(
    bag<date>(
        date("2004-04-12"),
        date("-1970-01-01"),
        date("2004-04-12"),
        date("2004-04-12"),
        date("2004-04-12")
    ),
    bag<date>(
        date("-1970-01-01")
    )
)

