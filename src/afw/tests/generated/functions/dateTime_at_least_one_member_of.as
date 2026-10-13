#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dateTime_at_least_one_member_of.as
//? customPurpose: Part of core function tests
//? description: Tests for at_least_one_member_of<dateTime> function.
//? sourceType: script
//?
//? test: at_least_one_member_of<dateTime>-1
//? description: False
//? expect: boolean(false)
//? source: ...

at_least_one_member_of<dateTime>(
    array<dateTime>(
        "2004-04-12T13:20:00"dateTime,
        "2004-04-12T13:20:15.5"dateTime,
        "2004-04-12T13:20:00"dateTime,
        "2004-04-12T13:20:00"dateTime
    ),
    array<dateTime>(
        "2005-04-12T13:20:00"dateTime,
        "2005-04-12T13:20:15.5"dateTime,
        "2005-04-12T13:20:00"dateTime,
        "2005-04-12T13:20:00"dateTime
    )
)

//?
//? test: at_least_one_member_of<dateTime>-2
//? description: values without a time zone have the implicit one; no 2005 value matches, and 13:20:07Z differs from 13:20:00 and 13:20:15.5 by seconds no zone offset can make up
//? expect: boolean(false)
//? source: ...

at_least_one_member_of<dateTime>(
    array<dateTime>(
        "2004-04-12T13:20:00"dateTime,
        "2004-04-12T13:20:15.5"dateTime,
        "2004-04-12T13:20:00"dateTime,
        "2004-04-12T13:20:00"dateTime
    ),
    array<dateTime>(
        "2005-04-12T13:20:00"dateTime,
        "2005-04-12T13:20:15.5"dateTime,
        "2005-04-12T13:20:00"dateTime,
        "2004-04-12T13:20:07Z"dateTime
    )
)

