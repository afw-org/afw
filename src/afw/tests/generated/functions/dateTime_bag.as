#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dateTime_bag.as
//? customPurpose: Part of core function tests
//? description: Tests for bag<dateTime> function.
//? sourceType: expression
//?
//? test: bag<dateTime>-1
//? description: zero
//? expect: ...

bag<dateTime>(
)

//? source: bag<dateTime>()
//?
//? test: bag<dateTime>-2
//? description: multiple
//? expect: ...

bag<dateTime>(
    dateTime("2004-04-12T13:20:00"),
    dateTime("2004-04-12T13:20:15.5"),
    dateTime("2004-04-12T13:20:00-05:00"),
    dateTime("2004-04-12T13:20:00Z"),
    dateTime("2004-04-12T13:20:00Z"),
    dateTime("2004-04-12T13:20:00Z"),
    dateTime("2004-04-12T13:20:00Z")
)

//? source: ...

bag<dateTime>(
    dateTime("2004-04-12T13:20:00"),
    dateTime("2004-04-12T13:20:15.5"),
    dateTime("2004-04-12T13:20:00-05:00"),
    dateTime("2004-04-12T13:20:00Z"),
    dateTime("2004-04-12T13:20:00Z"),
    dateTime("2004-04-12T13:20:00Z"),
    dateTime("2004-04-12T13:20:00Z")
)

