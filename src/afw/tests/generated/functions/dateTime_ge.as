#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dateTime_ge.as
//? customPurpose: Part of core function tests
//? description: Tests for ge<dateTime> function.
//? sourceType: script
//?
//? test: ge<dateTime>-1
//? description: greater than
//? expect: boolean(false)
//? source: ...

ge<dateTime>(
    dateTime("2004-04-12T13:20:00-04:00"),
    dateTime("2004-04-12T13:20:00-05:00")
)

//?
//? test: ge<dateTime>-2
//? description: equal
//? expect: boolean(true)
//? source: ...

ge<dateTime>(
    dateTime("2004-04-12T13:20:15.5"),
    dateTime("2004-04-12T13:20:15.5")
)

