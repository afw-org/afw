#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dateTime_union.as
//? customPurpose: Part of core function tests
//? description: Tests for union<dateTime> function.
//? sourceType: script
//?
//? test: union<dateTime>-1
//? description: Union
//? expect: ...

bag<dateTime>(
    dateTime("2004-04-12T13:20:00"),
    dateTime("2004-04-18T13:20:01")
)

//? source: ...

union<dateTime>(
    bag<dateTime>(
        dateTime("2004-04-12T13:20:00")
    ),
    bag<dateTime>(
        dateTime("2004-04-18T13:20:01")
    )
)

//?
//? test: union<dateTime>-2
//? description: Union
//? expect: ...

bag<dateTime>(
    dateTime("2004-04-12T13:20:00"),
    dateTime("2004-04-13T13:20:15.5"),
    dateTime("2004-04-14T13:20:00"),
    dateTime("2004-04-15T13:20:00"),
    dateTime("2004-04-16T13:20:15.5"),
    dateTime("2004-04-17T13:20:00"),
    dateTime("2004-04-18T13:20:00")
)

//? source: ...

union<dateTime>(
    bag<dateTime>(
        dateTime("2004-04-12T13:20:00"),
        dateTime("2004-04-13T13:20:15.5"),
        dateTime("2004-04-14T13:20:00"),
        dateTime("2004-04-15T13:20:00")
    ),
    bag<dateTime>(
        dateTime("2004-04-12T13:20:00"),
        dateTime("2004-04-16T13:20:15.5"),
        dateTime("2004-04-17T13:20:00"),
        dateTime("2004-04-18T13:20:00")
    )
)

