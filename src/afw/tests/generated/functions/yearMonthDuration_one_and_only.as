#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/yearMonthDuration_one_and_only.as
//? customPurpose: Part of core function tests
//? description: Tests for one_and_only<yearMonthDuration> function.
//? sourceType: expression
//?
//? test: one_and_only<yearMonthDuration>-1
//? description: x
//? expect: yearMonthDuration("P20M")
//? source: ...

one_and_only<yearMonthDuration>(
    bag<yearMonthDuration>(
        yearMonthDuration("P20M")
    )
)

//?
//? test: one_and_only<yearMonthDuration>-2
//? description: x
//? expect: yearMonthDuration("P2Y6M")
//? source: ...

one_and_only<yearMonthDuration>(
    bag<yearMonthDuration>(
        yearMonthDuration("P2Y6M")
    )
)

