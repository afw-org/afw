#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dateTime_lt.as
//? customPurpose: Part of core function tests
//? description: Tests for lt<dateTime> function.
//? sourceType: script
//?
//? test: lt<dateTime>-1
//? description: less than
//? expect: boolean(true)
//? source: ...

lt<dateTime>(
    dateTime("2005-04-12T13:20:00-05:00"),
    dateTime("2005-04-12T13:20:01-05:00")
)

//?
//? test: lt<dateTime>-2
//? description: not less than
//? expect: boolean(false)
//? source: ...

lt<dateTime>(
    dateTime("2005-04-12T13:20:00-04:00"),
    dateTime("2005-04-12T13:20:00-04:00")
)

