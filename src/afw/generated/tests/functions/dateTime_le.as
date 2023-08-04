#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dateTime_le.as
//? customPurpose: Part of core function tests
//? description: Tests for le<dateTime> function.
//? sourceType: expression
//?
//? test: le<dateTime>-1
//? description: less than
//? expect: boolean(true)
//? source: ...

le<dateTime>(
    dateTime("2005-04-12T13:20:00-05:00"),
    dateTime("2005-04-12T13:20:01-05:00")
)

//?
//? test: le<dateTime>-2
//? description: equal
//? expect: boolean(true)
//? source: ...

le<dateTime>(
    dateTime("2005-04-12T13:20:00-04:00"),
    dateTime("2005-04-12T13:20:00-04:00")
)

