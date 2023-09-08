#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: datatypes/dateTime.as
//? customPurpose: Part of core dataType tests
//? description: Tests for dateTime dataType.
//? sourceType: script
//?
//? test: dateTime-1
//? description: Test 1 for dateTime dataType.
//? expect: boolean(true)
//? source: ...

eq<dateTime>(
    dateTime("2004-04-12T13:20:00"),
    dateTime("2004-04-12T13:20:00")
)

//?
//? test: dateTime-2
//? description: Test 2 for dateTime dataType.
//? expect: boolean(true)
//? source: ...

eq<dateTime>(
    dateTime("2004-04-12T13:20:15.5"),
    dateTime("2004-04-12T13:20:15.5")
)

//?
//? test: dateTime-3
//? description: Test 3 for dateTime dataType.
//? expect: boolean(true)
//? source: ...

eq<dateTime>(
    dateTime("2004-04-12T13:20:00-05:00"),
    dateTime("2004-04-12T13:20:00-05:00")
)

//?
//? test: dateTime-4
//? description: Test 4 for dateTime dataType.
//? expect: boolean(true)
//? source: ...

eq<dateTime>(
    dateTime("2004-04-12T13:20:00Z"),
    dateTime("2004-04-12T13:20:00Z")
)

