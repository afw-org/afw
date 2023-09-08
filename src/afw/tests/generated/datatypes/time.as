#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: datatypes/time.as
//? customPurpose: Part of core dataType tests
//? description: Tests for time dataType.
//? sourceType: script
//?
//? test: time-1
//? description: Test 1 for time dataType.
//? expect: boolean(true)
//? source: ...

eq<time>(
    time("13:20:00"),
    time("13:20:00")
)

//?
//? test: time-2
//? description: Test 2 for time dataType.
//? expect: boolean(true)
//? source: ...

eq<time>(
    time("13:20:30.5555"),
    time("13:20:30.5555")
)

//?
//? test: time-3
//? description: Test 3 for time dataType.
//? expect: boolean(true)
//? source: ...

eq<time>(
    time("13:20:00-05:00"),
    time("13:20:00-05:00")
)

//?
//? test: time-4
//? description: Test 4 for time dataType.
//? expect: boolean(true)
//? source: ...

eq<time>(
    time("13:20:00Z"),
    time("13:20:00Z")
)

//?
//? test: time-5
//? description: Test 5 for time dataType.
//? expect: boolean(true)
//? source: ...

eq<time>(
    time("00:00:00"),
    time("00:00:00")
)

