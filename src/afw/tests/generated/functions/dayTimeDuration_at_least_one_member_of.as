#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dayTimeDuration_at_least_one_member_of.as
//? customPurpose: Part of core function tests
//? description: Tests for at_least_one_member_of<dayTimeDuration> function.
//? sourceType: script
//?
//? test: at_least_one_member_of<dayTimeDuration>-1
//? description: 3 days, 10 hours, and 30 minutes
//? expect: boolean(true)
//? source: ...

at_least_one_member_of<dayTimeDuration>(
    bag<dayTimeDuration>(
        dayTimeDuration("P3DT10H30M"),
        dayTimeDuration("P3DT10H10M")
    ),
    bag<dayTimeDuration>(
        dayTimeDuration("P3D"),
        dayTimeDuration("P3DT10H30M30S"),
        dayTimeDuration("P3D"),
        dayTimeDuration("P3DT10H30M")
    )
)

//?
//? test: at_least_one_member_of<dayTimeDuration>-2
//? description: 3 days, 10 hours, and 30 minutes
//? expect: boolean(false)
//? source: ...

at_least_one_member_of<dayTimeDuration>(
    bag<dayTimeDuration>(
        dayTimeDuration("P3D")
    ),
    bag<dayTimeDuration>(
        dayTimeDuration("P3DT10H30M30S"),
        dayTimeDuration("P3DT10H30M")
    )
)

//?
//? test: at_least_one_member_of<dayTimeDuration>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

at_least_one_member_of<dayTimeDuration>(
    bag<dayTimeDuration>(
        dayTimeDuration("x"),
        dayTimeDuration("x")
    ),
    bag<dayTimeDuration>(
        dayTimeDuration("x"),
        dayTimeDuration("x")
    )
)

