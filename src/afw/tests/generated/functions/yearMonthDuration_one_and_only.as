#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/yearMonthDuration_one_and_only.as
//? customPurpose: Part of core function tests
//? description: Tests for one_and_only<yearMonthDuration> function.
//? sourceType: script
//?
//? test: one_and_only<yearMonthDuration>-1
//? description: x
//? expect: "P20M"yearMonthDuration
//? source: ...

one_and_only<yearMonthDuration>(
    bag<yearMonthDuration>(
        "P20M"yearMonthDuration
    )
)

//?
//? test: one_and_only<yearMonthDuration>-2
//? description: x
//? expect: "P2Y6M"yearMonthDuration
//? source: ...

one_and_only<yearMonthDuration>(
    bag<yearMonthDuration>(
        "P2Y6M"yearMonthDuration
    )
)

