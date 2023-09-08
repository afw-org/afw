#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/date_add_yearMonthDuration.as
//? customPurpose: Part of core function tests
//? description: Tests for add_yearMonthDuration<date> function.
//? sourceType: script
//?
//? test: add_yearMonthDuration<date>-1
//? description: One
//? expect: date("2006-10-12")
//? source: ...

add_yearMonthDuration<date>(
    date("2004-04-12"),
    yearMonthDuration("P2Y6M")
)

//?
//? test: add_yearMonthDuration<date>-2
//? description: Two
//? expect: date("2001-10-12")
//? source: ...

add_yearMonthDuration<date>(
    date("2004-04-12"),
    yearMonthDuration("-P2Y6M")
)

//?
//? test: add_yearMonthDuration<date>-3
//? description: Two
//? expect: date("2004-04-12")
//? source: ...

add_yearMonthDuration<date>(
    date("2004-04-12"),
    yearMonthDuration("-P0Y")
)

