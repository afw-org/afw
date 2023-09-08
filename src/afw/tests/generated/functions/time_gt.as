#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/time_gt.as
//? customPurpose: Part of core function tests
//? description: Tests for gt<time> function.
//? sourceType: script
//?
//? test: gt<time>-1
//? description: x
//? expect: boolean(false)
//? source: ...

gt<time>(
    time("13:20:30.5555"),
    time("13:20:30.5555")
)

//?
//? test: gt<time>-2
//? description: x
//? expect: boolean(true)
//? source: ...

gt<time>(
    time("13:20:30.5555"),
    time("13:20:30.5554")
)

//?
//? test: gt<time>-3
//? description: x
//? expect: boolean(false)
//? source: ...

gt<time>(
    time("13:20:30.5554"),
    time("13:20:30.5555")
)

