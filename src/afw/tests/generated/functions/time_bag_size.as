#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/time_bag_size.as
//? customPurpose: Part of core function tests
//? description: Tests for bag_size<time> function.
//? sourceType: script
//?
//? test: bag_size<time>-1
//? description: x
//? expect: integer(5)
//? source: ...

bag_size<time>(
    bag<time>(
        time("13:20:00"),
        time("13:20:30.5555"),
        time("13:20:00-05:00"),
        time("13:20:00Z"),
        time("00:00:00")
    )
)

//?
//? test: bag_size<time>-2
//? description: x
//? expect: integer(1)
//? source: ...

bag_size<time>(
    bag<time>(
        time("00:00:00")
    )
)

