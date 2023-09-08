#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dateTime_eq.as
//? customPurpose: Part of core function tests
//? description: Tests for eq<dateTime> function.
//? sourceType: script
//?
//? test: eq<dateTime>-1
//? description: x
//? expect: boolean(true)
//? source: ...

eq<dateTime>(
    dateTime("2004-04-12T13:20:00"),
    dateTime("2004-04-12T13:20:00")
)

//?
//? test: eq<dateTime>-2
//? description: x
//? expect: boolean(true)
//? source: ...

eq<dateTime>(
    dateTime("2004-04-12T13:20:15.5"),
    dateTime("2004-04-12T13:20:15.5")
)

//?
//? test: eq<dateTime>-3
//? description: x
//? expect: boolean(true)
//? source: ...

eq<dateTime>(
    dateTime("2004-04-12T13:20:00-05:00"),
    dateTime("2004-04-12T13:20:00-05:00")
)

//?
//? test: eq<dateTime>-4
//? description: x
//? expect: boolean(false)
//? source: ...

eq<dateTime>(
    dateTime("2004-04-12T13:20:00Z"),
    dateTime("2004-04-12T13:20:01Z")
)

