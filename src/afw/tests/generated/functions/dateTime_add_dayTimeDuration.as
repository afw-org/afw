#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dateTime_add_dayTimeDuration.as
//? customPurpose: Part of core function tests
//? description: Tests for add_dayTimeDuration<dateTime> function.
//? sourceType: script
//?
//? test: add_dayTimeDuration<dateTime>-1
//? description: Add a day
//? expect: "2004-04-13T13:20:00"dateTime
//? source: ...

add_dayTimeDuration<dateTime>(
    "2004-04-12T13:20:00"dateTime,
    "P1D"dayTimeDuration
)

//?
//? test: add_dayTimeDuration<dateTime>-2
//? description: Add a day and an hour
//? expect: "2004-04-13T14:20:00"dateTime
//? source: ...

add_dayTimeDuration<dateTime>(
    "2004-04-12T13:20:00"dateTime,
    "P1DT1H"dayTimeDuration
)

//?
//? test: add_dayTimeDuration<dateTime>-3
//? description: Add a day and an hour and a second
//? expect: "2004-04-13T14:21:30.5"dateTime
//? source: ...

add_dayTimeDuration<dateTime>(
    "2004-04-12T13:20:00"dateTime,
    "P1DT1H1M30.5S"dayTimeDuration
)

