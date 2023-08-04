#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/yearMonthDuration_is_in.as
//? customPurpose: Part of core function tests
//? description: Tests for is_in<yearMonthDuration> function.
//? sourceType: expression
//?
//? test: is_in<yearMonthDuration>-1
//? description: x
//? expect: boolean(true)
//? source: ...

is_in<yearMonthDuration>(
    yearMonthDuration("P0Y"),
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
//? test: is_in<yearMonthDuration>-2
//? description: x
//? expect: boolean(true)
//? source: ...

is_in<yearMonthDuration>(
    yearMonthDuration("P2Y6M"),
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
//? test: is_in<yearMonthDuration>-3
//? description: x
//? expect: boolean(false)
//? source: ...

is_in<yearMonthDuration>(
    yearMonthDuration("P7Y"),
    bag<yearMonthDuration>(
        yearMonthDuration("P2Y6M"),
        yearMonthDuration("P20M"),
        yearMonthDuration("P0Y20M"),
        yearMonthDuration("P0Y"),
        yearMonthDuration("-P60Y"),
        yearMonthDuration("-P60Y")
    )
)

