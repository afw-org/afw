#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dateTime_to_string.as
//? customPurpose: Part of core function tests
//? description: Tests for to_string<dateTime> function.
//? sourceType: expression
//?
//? test: to_string<dateTime>-1
//? description: x
//? expect: string("2004-04-12T13:20:00")
//? source: ...

to_string<dateTime>(
    dateTime("2004-04-12T13:20:00")
)

//?
//? test: to_string<dateTime>-2
//? description: x
//? expect: string("2004-04-12T13:20:00-05:00")
//? source: ...

to_string<dateTime>(
    dateTime("2004-04-12T13:20:00-05:00")
)

//?
//? test: to_string<dateTime>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

to_string<dateTime>(
    dateTime("x")
)

