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

array<dayTimeDuration>(
    "-P1DT10H30M"dayTimeDuration,
    "P1DT10H30M"dayTimeDuration
)

//? source: ...

intersection<dayTimeDuration>(
    array<dayTimeDuration>(
        "-P1DT10H30M"dayTimeDuration,
        "P1DT10H30M"dayTimeDuration,
        "-P1DT10H30M"dayTimeDuration,
        "P1DT10H30M"dayTimeDuration,
        "-P1DT10H35M"dayTimeDuration
    ),
    array<dayTimeDuration>(
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
    array<dayTimeDuration>(
        "x"dayTimeDuration,
        "x"dayTimeDuration
    ),
    array<dayTimeDuration>(
        "x"dayTimeDuration,
        "x"dayTimeDuration
    )
)

