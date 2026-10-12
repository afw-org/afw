#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/yearMonthDuration_bag_size.as
//? customPurpose: Part of core function tests
//? description: Tests for bag_size<yearMonthDuration> function.
//? sourceType: script
//?
//? test: bag_size<yearMonthDuration>-1
//? description: x
//? expect: 5
//? source: ...

bag_size<yearMonthDuration>(
    bag<yearMonthDuration>(
        "P3Y6M"yearMonthDuration,
        "P20M"yearMonthDuration,
        "P0Y20M"yearMonthDuration,
        "P0Y"yearMonthDuration,
        "-P60Y"yearMonthDuration
    )
)

//?
//? test: bag_size<yearMonthDuration>-2
//? description: duplicates
//? expect: 8
//? source: ...

bag_size<yearMonthDuration>(
    bag<yearMonthDuration>(
        "P3Y6M"yearMonthDuration,
        "P20M"yearMonthDuration,
        "P0Y20M"yearMonthDuration,
        "P0Y"yearMonthDuration,
        "P0Y"yearMonthDuration,
        "P0Y"yearMonthDuration,
        "P0Y"yearMonthDuration,
        "-P60Y"yearMonthDuration
    )
)

//?
//? test: bag_size<yearMonthDuration>-3
//? description: Empty
//? expect: 0
//? source: ...

bag_size<yearMonthDuration>(
    bag<yearMonthDuration>(
    )
)

