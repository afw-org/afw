#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/yearMonthDuration_eq.as
//? customPurpose: Part of core function tests
//? description: Tests for eq<yearMonthDuration> function.
//? sourceType: script
//?
//? test: eq<yearMonthDuration>-1
//? description: x
//? expect: boolean(true)
//? source: ...

eq<yearMonthDuration>(
    yearMonthDuration("P3Y6M"),
    yearMonthDuration("P3Y6M")
)

//?
//? test: eq<yearMonthDuration>-2
//? description: true
//? expect: boolean(true)
//? source: ...

eq<yearMonthDuration>(
    yearMonthDuration("-P0Y"),
    yearMonthDuration("P0Y")
)

//?
//? test: eq<yearMonthDuration>-3
//? description: false
//? expect: boolean(false)
//? source: ...

eq<yearMonthDuration>(
    yearMonthDuration("-P1Y"),
    yearMonthDuration("P0Y")
)

