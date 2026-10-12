#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/yearMonthDuration_to_string.as
//? customPurpose: Part of core function tests
//? description: Tests for to_string<yearMonthDuration> function.
//? sourceType: script
//?
//? test: to_string<yearMonthDuration>-1
//? description: x
//? expect: string("P2Y6M")
//? source: ...

to_string<yearMonthDuration>(
    "P2Y6M"yearMonthDuration
)

//?
//? test: to_string<yearMonthDuration>-2
//? description: x
//? expect: string("-P2Y6M")
//? source: ...

to_string<yearMonthDuration>(
    "-P2Y6M"yearMonthDuration
)

//?
//? test: to_string<yearMonthDuration>-3
//? description: x
//? expect: string("P6M")
//? source: ...

to_string<yearMonthDuration>(
    "P6M"yearMonthDuration
)

//?
//? test: to_string<yearMonthDuration>-4
//? description: Invalid arguments test.
//? expect: error
//? source: ...

to_string<yearMonthDuration>(
    "x"yearMonthDuration
)

