#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/time_set_equals.as
//? customPurpose: Part of core function tests
//? description: Tests for set_equals<time> function.
//? sourceType: expression
//?
//? test: set_equals<time>-1
//? description: True
//? expect: boolean(true)
//? source: ...

set_equals<time>(
    bag<time>(
        time("13:20:30.5555"),
        time("13:30:30.5555"),
        time("13:40:30.5555"),
        time("13:50:30.5555")
    ),
    bag<time>(
        time("13:50:30.5555"),
        time("13:40:30.5555"),
        time("13:30:30.5555"),
        time("13:20:30.5555")
    )
)

//?
//? test: set_equals<time>-2
//? description: True
//? expect: boolean(false)
//? source: ...

set_equals<time>(
    bag<time>(
        time("13:20:30.5555"),
        time("13:30:30.5555"),
        time("13:40:30.5555"),
        time("13:50:30.5555")
    ),
    bag<time>(
        time("13:50:30.5555"),
        time("13:40:30.5555"),
        time("13:30:30.5555"),
        time("13:30:30.5555")
    )
)

