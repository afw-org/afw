#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/time_le.as
//? customPurpose: Part of core function tests
//? description: Tests for le<time> function.
//? sourceType: script
//?
//? test: le<time>-1
//? description: x
//? expect: boolean(true)
//? source: ...

le<time>(
    "13:20:30.5555"time,
    "13:20:30.5555"time
)

//?
//? test: le<time>-2
//? description: a time without a time zone has the implicit (local) one; 01:00:00+05:00 is 20:00:00Z the day before, earlier than 23:00:00 in any zone
//? expect: boolean(true)
//? source: ...

le<time>(
    "01:00:00+05:00"time,
    "23:00:00"time
)

//?
//? test: le<time>-3
//? description: x
//? expect: boolean(true)
//? source: ...

le<time>(
    "13:20:30.5554"time,
    "13:20:30.5555"time
)

