#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/time_at_least_one_member_of.as
//? customPurpose: Part of core function tests
//? description: Tests for at_least_one_member_of<time> function.
//? sourceType: script
//?
//? test: at_least_one_member_of<time>-1
//? description: x
//? expect: boolean(true)
//? source: ...

at_least_one_member_of<time>(
    bag<time>(
        time("13:20:30.5555"),
        time("13:20:30.5556")
    ),
    bag<time>(
        time("13:20:30.5555"),
        time("13:20:30.5555")
    )
)

//?
//? test: at_least_one_member_of<time>-2
//? description: x
//? expect: boolean(false)
//? source: ...

at_least_one_member_of<time>(
    bag<time>(
        time("13:20:30.5557"),
        time("13:20:30.5556")
    ),
    bag<time>(
        time("13:20:30.5555"),
        time("13:20:30.5555")
    )
)

