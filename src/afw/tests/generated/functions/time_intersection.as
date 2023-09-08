#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/time_intersection.as
//? customPurpose: Part of core function tests
//? description: Tests for intersection<time> function.
//? sourceType: script
//?
//? test: intersection<time>-1
//? description: x
//? expect: ...

bag<time>(
    time("13:20:00"),
    time("13:20:30.5555"),
    time("13:20:40.5555")
)

//? source: ...

intersection<time>(
    bag<time>(
        time("13:20:00"),
        time("13:20:30.5555"),
        time("13:20:40.5555")
    ),
    bag<time>(
        time("13:20:00"),
        time("13:20:30.5555"),
        time("13:20:40.5555")
    )
)

//?
//? test: intersection<time>-2
//? description: empty
//? expect: ...

bag<time>(
)

//? source: ...

intersection<time>(
    bag<time>(
    ),
    bag<time>(
    )
)

//?
//? test: intersection<time>-3
//? description: x
//? expect: ...

bag<time>(
)

//? source: ...

intersection<time>(
    bag<time>(
        time("13:20:00"),
        time("13:20:30.5555"),
        time("13:20:40.5555")
    ),
    bag<time>(
        time("14:20:00"),
        time("14:20:30.5555"),
        time("14:20:40.5555")
    )
)

