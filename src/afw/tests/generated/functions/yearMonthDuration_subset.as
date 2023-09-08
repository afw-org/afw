#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/yearMonthDuration_subset.as
//? customPurpose: Part of core function tests
//? description: Tests for subset<yearMonthDuration> function.
//? sourceType: script
//?
//? test: subset<yearMonthDuration>-1
//? description: x
//? expect: boolean(true)
//? source: ...

subset<yearMonthDuration>(
    bag<yearMonthDuration>(
        yearMonthDuration("P2Y6M"),
        yearMonthDuration("P20M")
    ),
    bag<yearMonthDuration>(
        yearMonthDuration("P3Y6M"),
        yearMonthDuration("P30M"),
        yearMonthDuration("P2Y6M"),
        yearMonthDuration("P20M")
    )
)

//?
//? test: subset<yearMonthDuration>-2
//? description: x
//? expect: boolean(false)
//? source: ...

subset<yearMonthDuration>(
    bag<yearMonthDuration>(
        yearMonthDuration("P2Y6M"),
        yearMonthDuration("P60M")
    ),
    bag<yearMonthDuration>(
        yearMonthDuration("P3Y6M"),
        yearMonthDuration("P30M"),
        yearMonthDuration("P2Y6M"),
        yearMonthDuration("P20M")
    )
)

//?
//? test: subset<yearMonthDuration>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

subset<yearMonthDuration>(
    bag<yearMonthDuration>(
        yearMonthDuration("x"),
        yearMonthDuration("x")
    ),
    bag<yearMonthDuration>(
        yearMonthDuration("x"),
        yearMonthDuration("x")
    )
)

