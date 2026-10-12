#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/date_subtract_yearMonthDuration.as
//? customPurpose: Part of core function tests
//? description: Tests for subtract_yearMonthDuration<date> function.
//? sourceType: script
//?
//? test: subtract_yearMonthDuration<date>-1
//? description: One
//? expect: "2001-10-12"date
//? source: ...

subtract_yearMonthDuration<date>(
    "2004-04-12"date,
    "P2Y6M"yearMonthDuration
)

//?
//? test: subtract_yearMonthDuration<date>-2
//? description: Two
//? expect: "2006-10-12"date
//? source: ...

subtract_yearMonthDuration<date>(
    "2004-04-12"date,
    "-P2Y6M"yearMonthDuration
)

//?
//? test: subtract_yearMonthDuration<date>-3
//? description: Two
//? expect: "2004-04-12"date
//? source: ...

subtract_yearMonthDuration<date>(
    "2004-04-12"date,
    "-P0Y"yearMonthDuration
)

