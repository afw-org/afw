#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/time_lt.as
//? customPurpose: Part of core function tests
//? description: Tests for lt<time> function.
//? sourceType: expression
//?
//? test: lt<time>-1
//? description: x
//? expect: boolean(false)
//? source: ...

lt<time>(
    time("13:20:30.5555"),
    time("13:20:30.5555")
)

//?
//? test: lt<time>-2
//? description: x
//? expect: error
//? source: ...

lt<time>(
    time("13:20:30.5554+05:00"),
    time("13:20:30.5554")
)

//?
//? test: lt<time>-3
//? description: x
//? expect: boolean(true)
//? source: ...

lt<time>(
    time("13:20:30.5554"),
    time("13:20:30.5555")
)

