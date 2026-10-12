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

bag<yearMonthDuration>(
    "P2Y6M"yearMonthDuration,
    "P0Y"yearMonthDuration,
    "P20M"yearMonthDuration
)

//? source: ...

union<yearMonthDuration>(
    bag<yearMonthDuration>(
        "P2Y6M"yearMonthDuration,
        "P0Y"yearMonthDuration
    ),
    bag<yearMonthDuration>(
        "P0Y20M"yearMonthDuration,
        "P20M"yearMonthDuration
    )
)

//?
//? test: union<yearMonthDuration>-2
//? description: x
//? expect: ...

bag<yearMonthDuration>(
    "P2Y6M"yearMonthDuration,
    "P0Y"yearMonthDuration,
    "P20M"yearMonthDuration
)

//? source: ...

union<yearMonthDuration>(
    bag<yearMonthDuration>(
        "P2Y6M"yearMonthDuration,
        "P0Y"yearMonthDuration
    ),
    bag<yearMonthDuration>(
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
    bag<yearMonthDuration>(
        "x"yearMonthDuration,
        "x"yearMonthDuration
    ),
    bag<yearMonthDuration>(
        "x"yearMonthDuration,
        "x"yearMonthDuration
    )
)

