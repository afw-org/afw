#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/yearMonthDuration_array.as
//? customPurpose: Part of core function tests
//? description: Tests for array<yearMonthDuration> function.
//? sourceType: script
//?
//? test: array<yearMonthDuration>-1
//? description: duplicates
//? expect: ...

array<yearMonthDuration>(
    "P3Y6M"yearMonthDuration,
    "P20M"yearMonthDuration,
    "P0Y20M"yearMonthDuration,
    "P0Y"yearMonthDuration,
    "-P60Y"yearMonthDuration,
    "-P60Y"yearMonthDuration,
    "-P60Y"yearMonthDuration,
    "-P60Y"yearMonthDuration
)

//? source: ...

array<yearMonthDuration>(
    "P3Y6M"yearMonthDuration,
    "P20M"yearMonthDuration,
    "P0Y20M"yearMonthDuration,
    "P0Y"yearMonthDuration,
    "-P60Y"yearMonthDuration,
    "-P60Y"yearMonthDuration,
    "-P60Y"yearMonthDuration,
    "-P60Y"yearMonthDuration
)

//?
//? test: array<yearMonthDuration>-2
//? description: Empty
//? expect: ...

array<yearMonthDuration>(
)

//? source: array<yearMonthDuration>()
