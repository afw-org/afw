#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/time_subset.as
//? customPurpose: Part of core function tests
//? description: Tests for subset<time> function.
//? sourceType: expression
//?
//? test: subset<time>-1
//? description: x
//? expect: boolean(true)
//? source: ...

subset<time>(
    bag<time>(
        time("13:20:00"),
        time("13:20:30.5555"),
        time("13:20:40.5555")
    ),
    bag<time>(
        time("13:20:00"),
        time("13:20:30.5555"),
        time("13:20:40.5555")
    )
)

//?
//? test: subset<time>-2
//? description: x
//? expect: boolean(false)
//? source: ...

subset<time>(
    bag<time>(
        time("13:20:00"),
        time("13:20:50.5555"),
        time("13:20:40.5555")
    ),
    bag<time>(
        time("13:20:00"),
        time("13:20:30.5555"),
        time("13:20:40.5555")
    )
)

