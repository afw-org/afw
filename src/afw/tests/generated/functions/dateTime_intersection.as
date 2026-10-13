#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dateTime_intersection.as
//? customPurpose: Part of core function tests
//? description: Tests for intersection<dateTime> function.
//? sourceType: script
//?
//? test: intersection<dateTime>-1
//? description: duplicates
//? expect: ...

array<dateTime>(
    "2004-04-12T13:20:00"dateTime,
    "2004-04-12T13:20:15.5"dateTime
)

//? source: ...

intersection<dateTime>(
    array<dateTime>(
        "2004-04-12T13:20:00"dateTime,
        "2004-04-12T13:20:15.5"dateTime
    ),
    array<dateTime>(
        "2004-04-12T13:20:00"dateTime,
        "2004-04-12T13:20:15.5"dateTime,
        "2004-04-12T13:20:00"dateTime,
        "2004-04-12T13:20:00"dateTime
    )
)

//?
//? test: intersection<dateTime>-2
//? description: one
//? expect: ...

array<dateTime>(
    "2004-04-12T13:20:00"dateTime
)

//? source: ...

intersection<dateTime>(
    array<dateTime>(
        "2004-04-12T13:20:00"dateTime
    ),
    array<dateTime>(
        "2004-04-12T13:20:00"dateTime,
        "2004-04-12T13:20:15.5"dateTime,
        "2004-04-12T13:20:00"dateTime,
        "2004-04-12T13:20:00"dateTime
    )
)

//?
//? test: intersection<dateTime>-3
//? description: empty
//? expect: ...

array<dateTime>(
)

//? source: ...

intersection<dateTime>(
    array<dateTime>(
        "2004-04-12T14:21:00"dateTime
    ),
    array<dateTime>(
        "2004-04-12T13:20:15.5"dateTime,
        "2004-04-12T13:20:00"dateTime,
        "2004-04-12T13:20:00"dateTime
    )
)

