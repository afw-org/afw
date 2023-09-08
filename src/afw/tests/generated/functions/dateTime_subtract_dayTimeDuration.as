#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dateTime_subtract_dayTimeDuration.as
//? customPurpose: Part of core function tests
//? description: Tests for subtract_dayTimeDuration<dateTime> function.
//? sourceType: script
//?
//? test: subtract_dayTimeDuration<dateTime>-1
//? description: Subtract a day
//? expect: dateTime("2004-04-11T13:20:00")
//? source: ...

subtract_dayTimeDuration<dateTime>(
    dateTime("2004-04-12T13:20:00"),
    dayTimeDuration("P1D")
)

//?
//? test: subtract_dayTimeDuration<dateTime>-2
//? description: Subtract a day and an hour
//? expect: dateTime("2004-04-11T12:20:00")
//? source: ...

subtract_dayTimeDuration<dateTime>(
    dateTime("2004-04-12T13:20:00"),
    dayTimeDuration("P1DT1H")
)

//?
//? test: subtract_dayTimeDuration<dateTime>-3
//? description: Subtract negative day and an hour and a second
//? expect: dateTime("2004-04-13T14:21:30.5")
//? source: ...

subtract_dayTimeDuration<dateTime>(
    dateTime("2004-04-12T13:20:00"),
    dayTimeDuration("-P1DT1H1M30.5S")
)

