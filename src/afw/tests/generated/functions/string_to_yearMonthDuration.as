#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/string_to_yearMonthDuration.as
//? customPurpose: Part of core function tests
//? description: Tests for to_yearMonthDuration<string> function.
//? sourceType: script
//?
//? test: to_yearMonthDuration<string>-1
//? description: P2Y6M
//? expect: yearMonthDuration("P2Y6M")
//? source: ...

to_yearMonthDuration<string>(
    string("P2Y6M")
)

//?
//? test: to_yearMonthDuration<string>-2
//? description: -P60Y
//? expect: yearMonthDuration("-P60Y")
//? source: ...

to_yearMonthDuration<string>(
    string("-P60Y")
)

//?
//? test: to_yearMonthDuration<string>-3
//? description: P0Y
//? expect: yearMonthDuration("P0Y")
//? source: ...

to_yearMonthDuration<string>(
    string("P0Y")
)

