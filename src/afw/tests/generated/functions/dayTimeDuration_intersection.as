#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dayTimeDuration_intersection.as
//? customPurpose: Part of core function tests
//? description: Tests for intersection<dayTimeDuration> function.
//? sourceType: script
//?
//? test: intersection<dayTimeDuration>-1
//? description: x
//? expect: ...

bag<dayTimeDuration>(
    "-P1DT10H30M"dayTimeDuration,
    "P1DT10H30M"dayTimeDuration
)

//? source: ...

intersection<dayTimeDuration>(
    bag<dayTimeDuration>(
        "-P1DT10H30M"dayTimeDuration,
        "P1DT10H30M"dayTimeDuration,
        "-P1DT10H30M"dayTimeDuration,
        "P1DT10H30M"dayTimeDuration,
        "-P1DT10H35M"dayTimeDuration
    ),
    bag<dayTimeDuration>(
        "-P1DT10H30M"dayTimeDuration,
        "P1DT10H30M"dayTimeDuration,
        "P1DT10H30M"dayTimeDuration,
        "P1DT10H30M"dayTimeDuration,
        "P1DT10H30M"dayTimeDuration,
        "P1DT30M"dayTimeDuration
    )
)

//?
//? test: intersection<dayTimeDuration>-2
//? description: Invalid arguments test.
//? expect: error
//? source: ...

intersection<dayTimeDuration>(
    bag<dayTimeDuration>(
        "x"dayTimeDuration,
        "x"dayTimeDuration
    ),
    bag<dayTimeDuration>(
        "x"dayTimeDuration,
        "x"dayTimeDuration
    )
)

