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

array<time>(
    "13:20:00"time,
    "13:20:30.5555"time,
    "13:20:40.5555"time
)

//? source: ...

intersection<time>(
    array<time>(
        "13:20:00"time,
        "13:20:30.5555"time,
        "13:20:40.5555"time
    ),
    array<time>(
        "13:20:00"time,
        "13:20:30.5555"time,
        "13:20:40.5555"time
    )
)

//?
//? test: intersection<time>-2
//? description: empty
//? expect: ...

array<time>(
)

//? source: ...

intersection<time>(
    array<time>(
    ),
    array<time>(
    )
)

//?
//? test: intersection<time>-3
//? description: x
//? expect: ...

array<time>(
)

//? source: ...

intersection<time>(
    array<time>(
        "13:20:00"time,
        "13:20:30.5555"time,
        "13:20:40.5555"time
    ),
    array<time>(
        "14:20:00"time,
        "14:20:30.5555"time,
        "14:20:40.5555"time
    )
)

