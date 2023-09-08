#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/time_is_in.as
//? customPurpose: Part of core function tests
//? description: Tests for is_in<time> function.
//? sourceType: script
//?
//? test: is_in<time>-1
//? description: x
//? expect: boolean(false)
//? source: ...

is_in<time>(
    time("13:20:30.55"),
    bag<time>(
        time("13:20:30.5555"),
        time("13:20:30.555")
    )
)

//?
//? test: is_in<time>-2
//? description: x
//? expect: boolean(true)
//? source: ...

is_in<time>(
    time("13:20:30.55+05:00"),
    bag<time>(
        time("13:20:30.55+05:00"),
        time("13:20:30.555")
    )
)

//?
//? test: is_in<time>-3
//? description: x
//? expect: boolean(false)
//? source: ...

is_in<time>(
    time("10:10:10"),
    bag<time>(
        time("13:20:30.5555"),
        time("13:20:30.555")
    )
)

