#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/yearMonthDuration_at_least_one_member_of.as
//? customPurpose: Part of core function tests
//? description: Tests for at_least_one_member_of<yearMonthDuration> function.
//? sourceType: script
//?
//? test: at_least_one_member_of<yearMonthDuration>-1
//? description: true
//? expect: boolean(true)
//? source: ...

at_least_one_member_of<yearMonthDuration>(
    bag<yearMonthDuration>(
        "P2Y6M"yearMonthDuration,
        "P20M"yearMonthDuration,
        "P0Y20M"yearMonthDuration,
        "P0Y"yearMonthDuration,
        "-P60Y"yearMonthDuration
    ),
    bag<yearMonthDuration>(
        "P2Y6M"yearMonthDuration
    )
)

//?
//? test: at_least_one_member_of<yearMonthDuration>-2
//? description: false
//? expect: boolean(false)
//? source: ...

at_least_one_member_of<yearMonthDuration>(
    bag<yearMonthDuration>(
        "P3Y6M"yearMonthDuration,
        "P20M"yearMonthDuration,
        "P0Y20M"yearMonthDuration,
        "P0Y"yearMonthDuration,
        "-P60Y"yearMonthDuration
    ),
    bag<yearMonthDuration>(
        "P2Y6M"yearMonthDuration
    )
)

//?
//? test: at_least_one_member_of<yearMonthDuration>-3
//? description: true
//? expect: boolean(true)
//? source: ...

at_least_one_member_of<yearMonthDuration>(
    bag<yearMonthDuration>(
        "P2Y6M"yearMonthDuration,
        "P20M"yearMonthDuration,
        "P0Y20M"yearMonthDuration,
        "P0Y"yearMonthDuration,
        "-P60Y"yearMonthDuration
    ),
    bag<yearMonthDuration>(
        "-P60Y"yearMonthDuration
    )
)

