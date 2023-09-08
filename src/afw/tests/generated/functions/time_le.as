#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/time_le.as
//? customPurpose: Part of core function tests
//? description: Tests for le<time> function.
//? sourceType: script
//?
//? test: le<time>-1
//? description: x
//? expect: boolean(true)
//? source: ...

le<time>(
    time("13:20:30.5555"),
    time("13:20:30.5555")
)

//?
//? test: le<time>-2
//? description: x
//? expect: error
//? source: ...

le<time>(
    time("13:20:30.5555+05:00"),
    time("13:20:30.5554")
)

//?
//? test: le<time>-3
//? description: x
//? expect: boolean(true)
//? source: ...

le<time>(
    time("13:20:30.5554"),
    time("13:20:30.5555")
)

