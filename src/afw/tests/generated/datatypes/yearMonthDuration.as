#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: datatypes/yearMonthDuration.as
//? customPurpose: Part of core dataType tests
//? description: Tests for yearMonthDuration dataType.
//? sourceType: script
//?
//? test: yearMonthDuration-1
//? description: Test 1 for yearMonthDuration dataType.
//? expect: boolean(true)
//? source: ...

eq<yearMonthDuration>(
    yearMonthDuration("P2Y6M"),
    yearMonthDuration("P2Y6M")
)

//?
//? test: yearMonthDuration-2
//? description: Test 2 for yearMonthDuration dataType.
//? expect: boolean(true)
//? source: ...

eq<yearMonthDuration>(
    yearMonthDuration("P20M"),
    yearMonthDuration("P20M")
)

//?
//? test: yearMonthDuration-3
//? description: Test 3 for yearMonthDuration dataType.
//? expect: boolean(true)
//? source: ...

eq<yearMonthDuration>(
    yearMonthDuration("P0Y20M"),
    yearMonthDuration("P0Y20M")
)

//?
//? test: yearMonthDuration-4
//? description: Test 4 for yearMonthDuration dataType.
//? expect: boolean(true)
//? source: ...

eq<yearMonthDuration>(
    yearMonthDuration("P0Y"),
    yearMonthDuration("P0Y")
)

//?
//? test: yearMonthDuration-5
//? description: Test 5 for yearMonthDuration dataType.
//? expect: boolean(true)
//? source: ...

eq<yearMonthDuration>(
    yearMonthDuration("-P60Y"),
    yearMonthDuration("-P60Y")
)

