#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/yearMonthDuration_intersection.as
//? customPurpose: Part of core function tests
//? description: Tests for intersection<yearMonthDuration> function.
//? sourceType: script
//?
//? test: intersection<yearMonthDuration>-1
//? description: x
//? expect: ...

array<yearMonthDuration>(
    "P2Y6M"yearMonthDuration,
    "P0Y"yearMonthDuration,
    "-P60Y"yearMonthDuration
)

//? source: ...

intersection<yearMonthDuration>(
    array<yearMonthDuration>(
        "P2Y6M"yearMonthDuration,
        "P20M"yearMonthDuration,
        "P0Y20M"yearMonthDuration,
        "P0Y"yearMonthDuration,
        "-P60Y"yearMonthDuration,
        "-P60Y"yearMonthDuration
    ),
    array<yearMonthDuration>(
        "P2Y6M"yearMonthDuration,
        "P0Y"yearMonthDuration,
        "-P60Y"yearMonthDuration,
        "-P60Y"yearMonthDuration
    )
)

//?
//? test: intersection<yearMonthDuration>-2
//? description: Empty
//? expect: ...

array<yearMonthDuration>(
)

//? source: ...

intersection<yearMonthDuration>(
    array<yearMonthDuration>(
        "P2Y6M"yearMonthDuration,
        "P20M"yearMonthDuration,
        "P0Y20M"yearMonthDuration,
        "P0Y"yearMonthDuration,
        "-P60Y"yearMonthDuration
    ),
    array<yearMonthDuration>(
        "P3Y6M"yearMonthDuration
    )
)

