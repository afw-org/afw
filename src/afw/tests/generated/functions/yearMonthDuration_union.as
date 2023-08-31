#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/yearMonthDuration_union.as
//? customPurpose: Part of core function tests
//? description: Tests for union<yearMonthDuration> function.
//? sourceType: expression
//?
//? test: union<yearMonthDuration>-1
//? description: x
//? expect: ...

bag<yearMonthDuration>(
    yearMonthDuration("P2Y6M"),
    yearMonthDuration("P0Y"),
    yearMonthDuration("P20M")
)

//? source: ...

union<yearMonthDuration>(
    bag<yearMonthDuration>(
        yearMonthDuration("P2Y6M"),
        yearMonthDuration("P0Y")
    ),
    bag<yearMonthDuration>(
        yearMonthDuration("P0Y20M"),
        yearMonthDuration("P20M")
    )
)

//?
//? test: union<yearMonthDuration>-2
//? description: x
//? expect: ...

bag<yearMonthDuration>(
    yearMonthDuration("P2Y6M"),
    yearMonthDuration("P0Y"),
    yearMonthDuration("P20M")
)

//? source: ...

union<yearMonthDuration>(
    bag<yearMonthDuration>(
        yearMonthDuration("P2Y6M"),
        yearMonthDuration("P0Y")
    ),
    bag<yearMonthDuration>(
        yearMonthDuration("P0Y"),
        yearMonthDuration("P20M")
    )
)

//?
//? test: union<yearMonthDuration>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

union<yearMonthDuration>(
    bag<yearMonthDuration>(
        yearMonthDuration("x"),
        yearMonthDuration("x")
    ),
    bag<yearMonthDuration>(
        yearMonthDuration("x"),
        yearMonthDuration("x")
    )
)

