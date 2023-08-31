#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/yearMonthDuration_bag_size.as
//? customPurpose: Part of core function tests
//? description: Tests for bag_size<yearMonthDuration> function.
//? sourceType: expression
//?
//? test: bag_size<yearMonthDuration>-1
//? description: x
//? expect: integer(5)
//? source: ...

bag_size<yearMonthDuration>(
    bag<yearMonthDuration>(
        yearMonthDuration("P3Y6M"),
        yearMonthDuration("P20M"),
        yearMonthDuration("P0Y20M"),
        yearMonthDuration("P0Y"),
        yearMonthDuration("-P60Y")
    )
)

//?
//? test: bag_size<yearMonthDuration>-2
//? description: duplicates
//? expect: integer(8)
//? source: ...

bag_size<yearMonthDuration>(
    bag<yearMonthDuration>(
        yearMonthDuration("P3Y6M"),
        yearMonthDuration("P20M"),
        yearMonthDuration("P0Y20M"),
        yearMonthDuration("P0Y"),
        yearMonthDuration("P0Y"),
        yearMonthDuration("P0Y"),
        yearMonthDuration("P0Y"),
        yearMonthDuration("-P60Y")
    )
)

//?
//? test: bag_size<yearMonthDuration>-3
//? description: Empty
//? expect: integer(0)
//? source: ...

bag_size<yearMonthDuration>(
    bag<yearMonthDuration>(
    )
)

