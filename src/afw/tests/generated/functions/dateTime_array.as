#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dateTime_array.as
//? customPurpose: Part of core function tests
//? description: Tests for array<dateTime> function.
//? sourceType: script
//?
//? test: array<dateTime>-1
//? description: zero
//? expect: ...

array<dateTime>(
)

//? source: array<dateTime>()
//?
//? test: array<dateTime>-2
//? description: multiple
//? expect: ...

array<dateTime>(
    "2004-04-12T13:20:00"dateTime,
    "2004-04-12T13:20:15.5"dateTime,
    "2004-04-12T13:20:00-05:00"dateTime,
    "2004-04-12T13:20:00Z"dateTime,
    "2004-04-12T13:20:00Z"dateTime,
    "2004-04-12T13:20:00Z"dateTime,
    "2004-04-12T13:20:00Z"dateTime
)

//? source: ...

array<dateTime>(
    "2004-04-12T13:20:00"dateTime,
    "2004-04-12T13:20:15.5"dateTime,
    "2004-04-12T13:20:00-05:00"dateTime,
    "2004-04-12T13:20:00Z"dateTime,
    "2004-04-12T13:20:00Z"dateTime,
    "2004-04-12T13:20:00Z"dateTime,
    "2004-04-12T13:20:00Z"dateTime
)

