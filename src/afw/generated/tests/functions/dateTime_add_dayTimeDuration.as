#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dateTime_add_dayTimeDuration.as
//? customPurpose: Part of core function tests
//? description: Tests for add_dayTimeDuration<dateTime> function.
//? sourceType: expression
//?
//? test: add_dayTimeDuration<dateTime>-1
//? description: Add a day
//? expect: dateTime("2004-04-13T13:20:00")
//? source: ...

add_dayTimeDuration<dateTime>(
    dateTime("2004-04-12T13:20:00"),
    dayTimeDuration("P1D")
)

//?
//? test: add_dayTimeDuration<dateTime>-2
//? description: Add a day and an hour
//? expect: dateTime("2004-04-13T14:20:00")
//? source: ...

add_dayTimeDuration<dateTime>(
    dateTime("2004-04-12T13:20:00"),
    dayTimeDuration("P1DT1H")
)

//?
//? test: add_dayTimeDuration<dateTime>-3
//? description: Add a day and an hour and a second
//? expect: dateTime("2004-04-13T14:21:30.5")
//? source: ...

add_dayTimeDuration<dateTime>(
    dateTime("2004-04-12T13:20:00"),
    dayTimeDuration("P1DT1H1M30.5S")
)

