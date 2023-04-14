#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dateTime_set_equals.as
//? customPurpose: Part of core function tests
//? description: Tests for set_equals<dateTime> function.
//? sourceType: expression
//?
//? test: set_equals<dateTime>-1
//? description: true
//? expect: boolean(true)
//? source: ...

set_equals<dateTime>(
    bag<dateTime>(
        dateTime("2004-04-12T13:20:15.5"),
        dateTime("2004-04-12T13:20:15.6")
    ),
    bag<dateTime>(
        dateTime("2004-04-12T13:20:15.5"),
        dateTime("2004-04-12T13:20:15.6")
    )
)

//?
//? test: set_equals<dateTime>-2
//? description: true
//? expect: boolean(false)
//? source: ...

set_equals<dateTime>(
    bag<dateTime>(
        dateTime("2004-04-12T13:20:15.5"),
        dateTime("2004-04-12T13:20:15.6")
    ),
    bag<dateTime>(
        dateTime("2004-04-12T13:20:15.6"),
        dateTime("2004-04-12T13:20:15.6")
    )
)

