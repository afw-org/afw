#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/time_bag.as
//? customPurpose: Part of core function tests
//? description: Tests for bag<time> function.
//? sourceType: expression
//?
//? test: bag<time>-1
//? description: One
//? expect: ...

bag<time>(
    time("13:20:00"),
    time("13:20:30.5555"),
    time("13:20:00-05:00"),
    time("13:20:00Z"),
    time("00:00:00")
)

//? source: ...

bag<time>(
    time("13:20:00"),
    time("13:20:30.5555"),
    time("13:20:00-05:00"),
    time("13:20:00Z"),
    time("00:00:00")
)

//?
//? test: bag<time>-2
//? description: Empty
//? expect: ...

bag<time>(
)

//? source: bag<time>()
