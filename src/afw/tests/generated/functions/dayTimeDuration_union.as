#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dayTimeDuration_union.as
//? customPurpose: Part of core function tests
//? description: Tests for union<dayTimeDuration> function.
//? sourceType: script
//?
//? test: union<dayTimeDuration>-1
//? description: true
//? expect: ...

bag<dayTimeDuration>(
    "P1DT3H"dayTimeDuration,
    "PT20M"dayTimeDuration,
    "PT1M30.5S"dayTimeDuration
)

//? source: ...

union<dayTimeDuration>(
    bag<dayTimeDuration>(
        "P1DT3H"dayTimeDuration,
        "PT20M"dayTimeDuration,
        "PT20M"dayTimeDuration,
        "PT20M"dayTimeDuration,
        "PT20M"dayTimeDuration,
        "PT20M"dayTimeDuration
    ),
    bag<dayTimeDuration>(
        "PT1M30.5S"dayTimeDuration
    )
)

//?
//? test: union<dayTimeDuration>-2
//? description: true
//? expect: ...

bag<dayTimeDuration>(
    "P1DT3H"dayTimeDuration,
    "PT20M"dayTimeDuration,
    "PT1M30.5S"dayTimeDuration
)

//? source: ...

union<dayTimeDuration>(
    bag<dayTimeDuration>(
        "P1DT3H"dayTimeDuration,
        "PT20M"dayTimeDuration
    ),
    bag<dayTimeDuration>(
        "PT1M30.5S"dayTimeDuration
    )
)

