#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dateTime_subtract_yearMonthDuration.as
//? customPurpose: Part of core function tests
//? description: Tests for subtract_yearMonthDuration<dateTime> function.
//? sourceType: expression
//?
//? test: subtract_yearMonthDuration<dateTime>-1
//? description: Sub
//? expect: dateTime("2001-10-12T13:20:00")
//? source: ...

subtract_yearMonthDuration<dateTime>(
    dateTime("2004-04-12T13:20:00"),
    yearMonthDuration("P2Y6M")
)

//?
//? test: subtract_yearMonthDuration<dateTime>-2
//? description: Add
//? expect: dateTime("2006-10-12T13:20:00")
//? source: ...

subtract_yearMonthDuration<dateTime>(
    dateTime("2004-04-12T13:20:00"),
    yearMonthDuration("-P2Y6M")
)

//?
//? test: subtract_yearMonthDuration<dateTime>-3
//? description: Zero
//? expect: dateTime("2004-04-12T13:20:00")
//? source: ...

subtract_yearMonthDuration<dateTime>(
    dateTime("2004-04-12T13:20:00"),
    yearMonthDuration("-P0Y0M")
)

