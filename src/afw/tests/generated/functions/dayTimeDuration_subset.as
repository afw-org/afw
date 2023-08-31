#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dayTimeDuration_subset.as
//? customPurpose: Part of core function tests
//? description: Tests for subset<dayTimeDuration> function.
//? sourceType: expression
//?
//? test: subset<dayTimeDuration>-1
//? description: true
//? expect: boolean(true)
//? source: ...

subset<dayTimeDuration>(
    bag<dayTimeDuration>(
        dayTimeDuration("P1DT2H"),
        dayTimeDuration("PT20M"),
        dayTimeDuration("PT20M"),
        dayTimeDuration("PT20M"),
        dayTimeDuration("PT20M"),
        dayTimeDuration("PT20M")
    ),
    bag<dayTimeDuration>(
        dayTimeDuration("P1DT2H"),
        dayTimeDuration("PT20M"),
        dayTimeDuration("PT120M"),
        dayTimeDuration("P0DT1H"),
        dayTimeDuration("P0D"),
        dayTimeDuration("-P60D"),
        dayTimeDuration("PT1M30.5S")
    )
)

//?
//? test: subset<dayTimeDuration>-2
//? description: false
//? expect: boolean(false)
//? source: ...

subset<dayTimeDuration>(
    bag<dayTimeDuration>(
        dayTimeDuration("P1DT3H"),
        dayTimeDuration("PT20M"),
        dayTimeDuration("PT20M"),
        dayTimeDuration("PT20M"),
        dayTimeDuration("PT20M"),
        dayTimeDuration("PT20M")
    ),
    bag<dayTimeDuration>(
        dayTimeDuration("P1DT2H"),
        dayTimeDuration("PT20M"),
        dayTimeDuration("PT120M"),
        dayTimeDuration("P0DT1H"),
        dayTimeDuration("P0D"),
        dayTimeDuration("-P60D"),
        dayTimeDuration("PT1M30.5S")
    )
)

