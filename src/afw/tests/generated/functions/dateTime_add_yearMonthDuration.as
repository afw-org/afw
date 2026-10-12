#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dateTime_add_yearMonthDuration.as
//? customPurpose: Part of core function tests
//? description: Tests for add_yearMonthDuration<dateTime> function.
//? sourceType: script
//?
//? test: add_yearMonthDuration<dateTime>-1
//? description: Subtract a year and a month
//? expect: "2003-03-13T13:20:00.5"dateTime
//? source: ...

add_yearMonthDuration<dateTime>(
    "2004-04-13T13:20:00.5"dateTime,
    "-P1Y1M"yearMonthDuration
)

//?
//? test: add_yearMonthDuration<dateTime>-2
//? description: Add a year and a month
//? expect: "2005-05-13T13:20:00.5"dateTime
//? source: ...

add_yearMonthDuration<dateTime>(
    "2004-04-13T13:20:00.5"dateTime,
    "P1Y1M"yearMonthDuration
)

