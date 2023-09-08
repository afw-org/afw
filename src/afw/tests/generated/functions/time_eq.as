#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/time_eq.as
//? customPurpose: Part of core function tests
//? description: Tests for eq<time> function.
//? sourceType: script
//?
//? test: eq<time>-1
//? description: x
//? expect: boolean(true)
//? source: ...

eq<time>(
    time("12:20"),
    time("12:20")
)

//?
//? test: eq<time>-2
//? description: x
//? expect: boolean(true)
//? source: ...

eq<time>(
    time("12:20:00"),
    time("12:20")
)

//?
//? test: eq<time>-3
//? description: x
//? expect: boolean(false)
//? source: ...

eq<time>(
    time("12:20"),
    time("12:20:01")
)

//?
//? test: eq<time>-4
//? description: Invalid arguments test.
//? expect: error
//? source: ...

eq<time>(
    time("x"),
    time("x")
)

