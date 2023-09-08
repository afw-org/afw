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

bag<dateTime>(
    dateTime("2004-04-12T13:20:00"),
    dateTime("2004-04-12T13:20:15.5")
)

//? source: ...

intersection<dateTime>(
    bag<dateTime>(
        dateTime("2004-04-12T13:20:00"),
        dateTime("2004-04-12T13:20:15.5")
    ),
    bag<dateTime>(
        dateTime("2004-04-12T13:20:00"),
        dateTime("2004-04-12T13:20:15.5"),
        dateTime("2004-04-12T13:20:00"),
        dateTime("2004-04-12T13:20:00")
    )
)

//?
//? test: intersection<dateTime>-2
//? description: one
//? expect: ...

bag<dateTime>(
    dateTime("2004-04-12T13:20:00")
)

//? source: ...

intersection<dateTime>(
    bag<dateTime>(
        dateTime("2004-04-12T13:20:00")
    ),
    bag<dateTime>(
        dateTime("2004-04-12T13:20:00"),
        dateTime("2004-04-12T13:20:15.5"),
        dateTime("2004-04-12T13:20:00"),
        dateTime("2004-04-12T13:20:00")
    )
)

//?
//? test: intersection<dateTime>-3
//? description: empty
//? expect: ...

bag<dateTime>(
)

//? source: ...

intersection<dateTime>(
    bag<dateTime>(
        dateTime("2004-04-12T14:21:00")
    ),
    bag<dateTime>(
        dateTime("2004-04-12T13:20:15.5"),
        dateTime("2004-04-12T13:20:00"),
        dateTime("2004-04-12T13:20:00")
    )
)

