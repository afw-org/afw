#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dateTime_is_in.as
//? customPurpose: Part of core function tests
//? description: Tests for is_in<dateTime> function.
//? sourceType: expression
//?
//? test: is_in<dateTime>-1
//? description: true
//? expect: boolean(true)
//? source: ...

is_in<dateTime>(
    dateTime("2004-04-12T13:20:00"),
    bag<dateTime>(
        dateTime("2004-04-12T13:20:00"),
        dateTime("2004-04-12T13:20:15.5"),
        dateTime("2004-04-12T13:20:00"),
        dateTime("2004-04-12T13:20:00")
    )
)

//?
//? test: is_in<dateTime>-2
//? description: false
//? expect: error
//? source: ...

is_in<dateTime>(
    dateTime("2005-04-12T13:20:00-05:00"),
    bag<dateTime>(
        dateTime("2004-04-12T13:20:00"),
        dateTime("2004-04-12T13:20:15.5"),
        dateTime("2004-04-12T13:20:00-05:00"),
        dateTime("2004-04-12T13:20:00Z")
    )
)

