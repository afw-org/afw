#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dateTime_gt.as
//? customPurpose: Part of core function tests
//? description: Tests for gt<dateTime> function.
//? sourceType: script
//?
//? test: gt<dateTime>-1
//? description: greater than
//? expect: boolean(false)
//? source: ...

gt<dateTime>(
    dateTime("2004-04-12T13:20:00-04:00"),
    dateTime("2004-04-12T13:20:00-05:00")
)

//?
//? test: gt<dateTime>-2
//? description: not greater than
//? expect: boolean(false)
//? source: ...

gt<dateTime>(
    dateTime("2004-04-12T13:20:15.5"),
    dateTime("2004-04-12T13:20:15.6")
)

