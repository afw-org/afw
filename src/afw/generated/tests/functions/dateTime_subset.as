#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dateTime_subset.as
//? customPurpose: Part of core function tests
//? description: Tests for subset<dateTime> function.
//? sourceType: expression
//?
//? test: subset<dateTime>-1
//? description: true
//? expect: boolean(true)
//? source: ...

subset<dateTime>(
    bag<dateTime>(
        dateTime("2004-04-12T13:20:15.5"),
        dateTime("2004-04-12T13:20:15.6")
    ),
    bag<dateTime>(
        dateTime("2004-04-12T13:20:15.7"),
        dateTime("2004-04-12T13:20:15.8"),
        dateTime("2004-04-12T13:20:15.5"),
        dateTime("2004-04-12T13:20:15.6")
    )
)

//?
//? test: subset<dateTime>-2
//? description: false
//? expect: boolean(false)
//? source: ...

subset<dateTime>(
    bag<dateTime>(
        dateTime("2004-04-12T13:20:15.9"),
        dateTime("2004-04-12T13:20:15.6")
    ),
    bag<dateTime>(
        dateTime("2004-04-12T13:20:15.7"),
        dateTime("2004-04-12T13:20:15.8"),
        dateTime("2004-04-12T13:20:15.5"),
        dateTime("2004-04-12T13:20:15.6")
    )
)

