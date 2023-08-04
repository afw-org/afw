#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/time_union.as
//? customPurpose: Part of core function tests
//? description: Tests for union<time> function.
//? sourceType: expression
//?
//? test: union<time>-1
//? description: x
//? expect: ...

bag<time>(
    time("13:20:00"),
    time("13:20:30.5555"),
    time("13:20:40.5555"),
    time("13:20:30.6666")
)

//? source: ...

union<time>(
    bag<time>(
        time("13:20:00"),
        time("13:20:30.5555"),
        time("13:20:40.5555")
    ),
    bag<time>(
        time("13:20:30.6666")
    )
)

//?
//? test: union<time>-2
//? description: With duplicates
//? expect: ...

bag<time>(
    time("13:20:00"),
    time("13:20:30.5555"),
    time("13:20:40.5555"),
    time("13:20:30.6666")
)

//? source: ...

union<time>(
    bag<time>(
        time("13:20:00"),
        time("13:20:00"),
        time("13:20:00"),
        time("13:20:30.5555"),
        time("13:20:40.5555")
    ),
    bag<time>(
        time("13:20:30.6666")
    )
)

