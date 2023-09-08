#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/yearMonthDuration_intersection.as
//? customPurpose: Part of core function tests
//? description: Tests for intersection<yearMonthDuration> function.
//? sourceType: script
//?
//? test: intersection<yearMonthDuration>-1
//? description: x
//? expect: ...

bag<yearMonthDuration>(
    yearMonthDuration("P2Y6M"),
    yearMonthDuration("P0Y"),
    yearMonthDuration("-P60Y")
)

//? source: ...

intersection<yearMonthDuration>(
    bag<yearMonthDuration>(
        yearMonthDuration("P2Y6M"),
        yearMonthDuration("P20M"),
        yearMonthDuration("P0Y20M"),
        yearMonthDuration("P0Y"),
        yearMonthDuration("-P60Y"),
        yearMonthDuration("-P60Y")
    ),
    bag<yearMonthDuration>(
        yearMonthDuration("P2Y6M"),
        yearMonthDuration("P0Y"),
        yearMonthDuration("-P60Y"),
        yearMonthDuration("-P60Y")
    )
)

//?
//? test: intersection<yearMonthDuration>-2
//? description: Empty
//? expect: ...

bag<yearMonthDuration>(
)

//? source: ...

intersection<yearMonthDuration>(
    bag<yearMonthDuration>(
        yearMonthDuration("P2Y6M"),
        yearMonthDuration("P20M"),
        yearMonthDuration("P0Y20M"),
        yearMonthDuration("P0Y"),
        yearMonthDuration("-P60Y")
    ),
    bag<yearMonthDuration>(
        yearMonthDuration("P3Y6M")
    )
)

