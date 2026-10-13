#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/time_in_range.as
//? customPurpose: Part of core function tests
//? description: Tests for in_range<time> function.
//? sourceType: script
//?
//? test: in_range<time>-1
//? description: x
//? expect: boolean(false)
//? source: ...

in_range<time>(
    "13:20:00"time,
    "13:20:30.5555"time,
    "13:20:40.5555"time
)

//?
//? test: in_range<time>-2
//? description: x
//? expect: boolean(true)
//? source: ...

in_range<time>(
    "13:20:35"time,
    "13:20:30.5555"time,
    "23:20:40.5555"time
)

//?
//? test: in_range<time>-3
//? description: 00:00:00+05:00 is 19:00:00Z, so the range is 19:00:00Z to 23:59:40.5555Z and 00:00:01Z is outside it
//? expect: boolean(false)
//? source: ...

in_range<time>(
    "00:00:01Z"time,
    "00:00:00+05:00"time,
    "23:59:40.5555Z"time
)

//?
//? test: in_range<time>-wrap-utc
//? description: 04:00:00+05:00 is 23:00:00Z, so the range wraps past midnight and 23:30:00Z is inside it
//? expect: boolean(true)
//? source: ...

in_range<time>(
    "23:30:00Z"time,
    "04:00:00+05:00"time,
    "01:00:00Z"time
)

