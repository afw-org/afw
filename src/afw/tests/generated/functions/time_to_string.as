#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/time_to_string.as
//? customPurpose: Part of core function tests
//? description: Tests for to_string<time> function.
//? sourceType: script
//?
//? test: to_string<time>-1
//? description: x
//? expect: string("13:20:30.5555+05:00")
//? source: ...

to_string<time>(
    time("13:20:30.5555+05:00")
)

//?
//? test: to_string<time>-2
//? description: x
//? expect: string("13:20:30.5555")
//? source: ...

to_string<time>(
    time("13:20:30.5555")
)

//?
//? test: to_string<time>-3
//? description: x
//? expect: string("13:20:00")
//? source: ...

to_string<time>(
    time("13:20")
)

//?
//? test: to_string<time>-4
//? description: Invalid arguments test.
//? expect: error
//? source: ...

to_string<time>(
    time("x")
)

