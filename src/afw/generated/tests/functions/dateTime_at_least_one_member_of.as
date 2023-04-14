#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dateTime_at_least_one_member_of.as
//? customPurpose: Part of core function tests
//? description: Tests for at_least_one_member_of<dateTime> function.
//? sourceType: expression
//?
//? test: at_least_one_member_of<dateTime>-1
//? description: False
//? expect: boolean(false)
//? source: ...

at_least_one_member_of<dateTime>(
    bag<dateTime>(
        dateTime("2004-04-12T13:20:00"),
        dateTime("2004-04-12T13:20:15.5"),
        dateTime("2004-04-12T13:20:00"),
        dateTime("2004-04-12T13:20:00")
    ),
    bag<dateTime>(
        dateTime("2005-04-12T13:20:00"),
        dateTime("2005-04-12T13:20:15.5"),
        dateTime("2005-04-12T13:20:00"),
        dateTime("2005-04-12T13:20:00")
    )
)

//?
//? test: at_least_one_member_of<dateTime>-2
//? description: True
//? expect: error
//? source: ...

at_least_one_member_of<dateTime>(
    bag<dateTime>(
        dateTime("2004-04-12T13:20:00"),
        dateTime("2004-04-12T13:20:15.5"),
        dateTime("2004-04-12T13:20:00"),
        dateTime("2004-04-12T13:20:00")
    ),
    bag<dateTime>(
        dateTime("2005-04-12T13:20:00"),
        dateTime("2005-04-12T13:20:15.5"),
        dateTime("2005-04-12T13:20:00"),
        dateTime("2004-04-12T13:20:00Z")
    )
)

