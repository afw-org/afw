#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/time_in_range.as
//? customPurpose: Part of core function tests
//? description: Tests for in_range<time> function.
//? sourceType: expression
//?
//? test: in_range<time>-1
//? description: x
//? expect: boolean(false)
//? source: ...

in_range<time>(
    time("13:20:00"),
    time("13:20:30.5555"),
    time("13:20:40.5555")
)

//?
//? test: in_range<time>-2
//? description: x
//? expect: boolean(true)
//? source: ...

in_range<time>(
    time("13:20:35"),
    time("13:20:30.5555"),
    time("23:20:40.5555")
)

//?
//? test: in_range<time>-3
//? description: x
//? expect: boolean(true)
//? source: ...

in_range<time>(
    time("00:00:01"),
    time("00:00:00+05:00"),
    time("23:59:40.5555")
)

