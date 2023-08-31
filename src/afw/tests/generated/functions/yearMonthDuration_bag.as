#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/yearMonthDuration_bag.as
//? customPurpose: Part of core function tests
//? description: Tests for bag<yearMonthDuration> function.
//? sourceType: expression
//?
//? test: bag<yearMonthDuration>-1
//? description: duplicates
//? expect: ...

bag<yearMonthDuration>(
    yearMonthDuration("P3Y6M"),
    yearMonthDuration("P20M"),
    yearMonthDuration("P0Y20M"),
    yearMonthDuration("P0Y"),
    yearMonthDuration("-P60Y"),
    yearMonthDuration("-P60Y"),
    yearMonthDuration("-P60Y"),
    yearMonthDuration("-P60Y")
)

//? source: ...

bag<yearMonthDuration>(
    yearMonthDuration("P3Y6M"),
    yearMonthDuration("P20M"),
    yearMonthDuration("P0Y20M"),
    yearMonthDuration("P0Y"),
    yearMonthDuration("-P60Y"),
    yearMonthDuration("-P60Y"),
    yearMonthDuration("-P60Y"),
    yearMonthDuration("-P60Y")
)

//?
//? test: bag<yearMonthDuration>-2
//? description: Empty
//? expect: ...

bag<yearMonthDuration>(
)

//? source: bag<yearMonthDuration>()
