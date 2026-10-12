#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dateTime_subtract_yearMonthDuration.as
//? customPurpose: Part of core function tests
//? description: Tests for subtract_yearMonthDuration<dateTime> function.
//? sourceType: script
//?
//? test: subtract_yearMonthDuration<dateTime>-1
//? description: Sub
//? expect: "2001-10-12T13:20:00"dateTime
//? source: ...

subtract_yearMonthDuration<dateTime>(
    "2004-04-12T13:20:00"dateTime,
    "P2Y6M"yearMonthDuration
)

//?
//? test: subtract_yearMonthDuration<dateTime>-2
//? description: Add
//? expect: "2006-10-12T13:20:00"dateTime
//? source: ...

subtract_yearMonthDuration<dateTime>(
    "2004-04-12T13:20:00"dateTime,
    "-P2Y6M"yearMonthDuration
)

//?
//? test: subtract_yearMonthDuration<dateTime>-3
//? description: Zero
//? expect: "2004-04-12T13:20:00"dateTime
//? source: ...

subtract_yearMonthDuration<dateTime>(
    "2004-04-12T13:20:00"dateTime,
    "-P0Y0M"yearMonthDuration
)

