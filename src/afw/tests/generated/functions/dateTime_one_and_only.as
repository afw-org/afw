#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dateTime_one_and_only.as
//? customPurpose: Part of core function tests
//? description: Tests for one_and_only<dateTime> function.
//? sourceType: script
//?
//? test: one_and_only<dateTime>-1
//? description: true
//? expect: "2004-04-12T13:20:15.5"dateTime
//? source: ...

one_and_only<dateTime>(
    array<dateTime>(
        "2004-04-12T13:20:15.5"dateTime
    )
)

//?
//? test: one_and_only<dateTime>-2
//? description: false
//? expect: error
//? source: ...

one_and_only<dateTime>(
    array<dateTime>(
        "2004-04-12T13:20:15.5"dateTime,
        "2004-04-12T13:20:15.5"dateTime
    )
)

