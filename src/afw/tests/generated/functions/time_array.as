#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/time_array.as
//? customPurpose: Part of core function tests
//? description: Tests for array<time> function.
//? sourceType: script
//?
//? test: array<time>-1
//? description: One
//? expect: ...

array<time>(
    "13:20:00"time,
    "13:20:30.5555"time,
    "13:20:00-05:00"time,
    "13:20:00Z"time,
    "00:00:00"time
)

//? source: ...

array<time>(
    "13:20:00"time,
    "13:20:30.5555"time,
    "13:20:00-05:00"time,
    "13:20:00Z"time,
    "00:00:00"time
)

//?
//? test: array<time>-2
//? description: Empty
//? expect: ...

array<time>(
)

//? source: array<time>()
