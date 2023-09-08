#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/yearMonthDuration_set_equals.as
//? customPurpose: Part of core function tests
//? description: Tests for set_equals<yearMonthDuration> function.
//? sourceType: script
//?
//? test: set_equals<yearMonthDuration>-1
//? description: x
//? expect: boolean(true)
//? source: ...

set_equals<yearMonthDuration>(
    bag<yearMonthDuration>(
        yearMonthDuration("P2Y6M"),
        yearMonthDuration("P20M"),
        yearMonthDuration("P0Y20M"),
        yearMonthDuration("P0Y"),
        yearMonthDuration("-P60Y")
    ),
    bag<yearMonthDuration>(
        yearMonthDuration("P2Y6M"),
        yearMonthDuration("P20M"),
        yearMonthDuration("P0Y20M"),
        yearMonthDuration("P0Y"),
        yearMonthDuration("-P60Y"),
        yearMonthDuration("-P60Y")
    )
)

//?
//? test: set_equals<yearMonthDuration>-2
//? description: Invalid arguments test.
//? expect: boolean(false)
//? source: ...

set_equals<yearMonthDuration>(
    bag<yearMonthDuration>(
        yearMonthDuration("P3Y6M"),
        yearMonthDuration("P20M"),
        yearMonthDuration("P0Y20M"),
        yearMonthDuration("P0Y"),
        yearMonthDuration("-P60Y")
    ),
    bag<yearMonthDuration>(
        yearMonthDuration("P2Y6M"),
        yearMonthDuration("P20M"),
        yearMonthDuration("P0Y20M"),
        yearMonthDuration("P0Y"),
        yearMonthDuration("-P60Y"),
        yearMonthDuration("-P60Y")
    )
)

