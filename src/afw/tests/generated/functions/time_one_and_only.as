#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/time_one_and_only.as
//? customPurpose: Part of core function tests
//? description: Tests for one_and_only<time> function.
//? sourceType: script
//?
//? test: one_and_only<time>-1
//? description: x
//? expect: "13:20:30.5555"time
//? source: ...

one_and_only<time>(
    array<time>(
        "13:20:30.5555"time
    )
)

//?
//? test: one_and_only<time>-2
//? description: Invalid arguments test.
//? expect: error
//? source: ...

one_and_only<time>(
    array<time>(
        "13:20:30.5555"time,
        "13:20:30.5555"time
    )
)

