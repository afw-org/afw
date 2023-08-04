#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dateTime_bag_size.as
//? customPurpose: Part of core function tests
//? description: Tests for bag_size<dateTime> function.
//? sourceType: expression
//?
//? test: bag_size<dateTime>-1
//? description: 7
//? expect: integer(7)
//? source: ...

bag_size<dateTime>(
    bag<dateTime>(
        dateTime("2004-04-12T13:20:00"),
        dateTime("2004-04-12T13:20:15.5"),
        dateTime("2004-04-12T13:20:00-05:00"),
        dateTime("2004-04-12T13:20:00Z"),
        dateTime("2004-04-12T13:20:00Z"),
        dateTime("2004-04-12T13:20:00Z"),
        dateTime("2004-04-12T13:20:00Z")
    )
)

//?
//? test: bag_size<dateTime>-2
//? description: 0
//? expect: integer(0)
//? source: ...

bag_size<dateTime>(
    bag<dateTime>(
    )
)

