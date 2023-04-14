#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/time_ge.as
//? customPurpose: Part of core function tests
//? description: Tests for ge<time> function.
//? sourceType: expression
//?
//? test: ge<time>-1
//? description: x
//? expect: error
//? source: ...

ge<time>(
    time("13:69:30.5555"),
    time("24:00:00")
)

//?
//? test: ge<time>-2
//? description: x
//? expect: error
//? source: ...

ge<time>(
    time("13:20:30.5555+05:00"),
    time("13:20:30.5554")
)

//?
//? test: ge<time>-3
//? description: x
//? expect: boolean(false)
//? source: ...

ge<time>(
    time("13:20:30.5554"),
    time("13:20:30.5555")
)

