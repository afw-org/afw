#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dayTimeDuration_union.as
//? customPurpose: Part of core function tests
//? description: Tests for union<dayTimeDuration> function.
//? sourceType: expression
//?
//? test: union<dayTimeDuration>-1
//? description: true
//? expect: ...

bag<dayTimeDuration>(
    dayTimeDuration("P1DT3H"),
    dayTimeDuration("PT20M"),
    dayTimeDuration("PT1M30.5S")
)

//? source: ...

union<dayTimeDuration>(
    bag<dayTimeDuration>(
        dayTimeDuration("P1DT3H"),
        dayTimeDuration("PT20M"),
        dayTimeDuration("PT20M"),
        dayTimeDuration("PT20M"),
        dayTimeDuration("PT20M"),
        dayTimeDuration("PT20M")
    ),
    bag<dayTimeDuration>(
        dayTimeDuration("PT1M30.5S")
    )
)

//?
//? test: union<dayTimeDuration>-2
//? description: true
//? expect: ...

bag<dayTimeDuration>(
    dayTimeDuration("P1DT3H"),
    dayTimeDuration("PT20M"),
    dayTimeDuration("PT1M30.5S")
)

//? source: ...

union<dayTimeDuration>(
    bag<dayTimeDuration>(
        dayTimeDuration("P1DT3H"),
        dayTimeDuration("PT20M")
    ),
    bag<dayTimeDuration>(
        dayTimeDuration("PT1M30.5S")
    )
)

