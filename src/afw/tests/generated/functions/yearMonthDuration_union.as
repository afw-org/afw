#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/yearMonthDuration_union.as
//? customPurpose: Part of core function tests
//? description: Tests for union<yearMonthDuration> function.
//? sourceType: script
//?
//? test: union<yearMonthDuration>-1
//? description: x
//? expect: ...

array<yearMonthDuration>(
    "P2Y6M"yearMonthDuration,
    "P0Y"yearMonthDuration,
    "P20M"yearMonthDuration
)

//? source: ...

union<yearMonthDuration>(
    array<yearMonthDuration>(
        "P2Y6M"yearMonthDuration,
        "P0Y"yearMonthDuration
    ),
    array<yearMonthDuration>(
        "P0Y20M"yearMonthDuration,
        "P20M"yearMonthDuration
    )
)

//?
//? test: union<yearMonthDuration>-2
//? description: x
//? expect: ...

array<yearMonthDuration>(
    "P2Y6M"yearMonthDuration,
    "P0Y"yearMonthDuration,
    "P20M"yearMonthDuration
)

//? source: ...

union<yearMonthDuration>(
    array<yearMonthDuration>(
        "P2Y6M"yearMonthDuration,
        "P0Y"yearMonthDuration
    ),
    array<yearMonthDuration>(
        "P0Y"yearMonthDuration,
        "P20M"yearMonthDuration
    )
)

//?
//? test: union<yearMonthDuration>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

union<yearMonthDuration>(
    array<yearMonthDuration>(
        "x"yearMonthDuration,
        "x"yearMonthDuration
    ),
    array<yearMonthDuration>(
        "x"yearMonthDuration,
        "x"yearMonthDuration
    )
)

