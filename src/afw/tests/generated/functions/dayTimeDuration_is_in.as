#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dayTimeDuration_is_in.as
//? customPurpose: Part of core function tests
//? description: Tests for is_in<dayTimeDuration> function.
//? sourceType: script
//?
//? test: is_in<dayTimeDuration>-1
//? description: P3DT10H30M is in bag
//? expect: boolean(true)
//? source: ...

is_in<dayTimeDuration>(
    "P3DT10H30M"dayTimeDuration,
    bag<dayTimeDuration>(
        "P3DT10H"dayTimeDuration,
        "P3DT10H31M"dayTimeDuration,
        "P3DT10H30M"dayTimeDuration,
        "P3D"dayTimeDuration,
        "P3DT10H30M"dayTimeDuration
    )
)

//?
//? test: is_in<dayTimeDuration>-2
//? description: P3DT10H30M is not in bag
//? expect: boolean(false)
//? source: ...

is_in<dayTimeDuration>(
    "P3DT10H30M"dayTimeDuration,
    bag<dayTimeDuration>(
        "-P3DT10H30M"dayTimeDuration,
        "P4DT10H30M"dayTimeDuration,
        "P3DT10H3M"dayTimeDuration,
        "P3DT10H3M"dayTimeDuration,
        "P3D"dayTimeDuration
    )
)

//?
//? test: is_in<dayTimeDuration>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

is_in<dayTimeDuration>(
    "x"dayTimeDuration,
    bag<dayTimeDuration>(
        "x"dayTimeDuration,
        "x"dayTimeDuration
    )
)

