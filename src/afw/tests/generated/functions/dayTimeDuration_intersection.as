#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dayTimeDuration_intersection.as
//? customPurpose: Part of core function tests
//? description: Tests for intersection<dayTimeDuration> function.
//? sourceType: expression
//?
//? test: intersection<dayTimeDuration>-1
//? description: x
//? expect: ...

bag<dayTimeDuration>(
    dayTimeDuration("-P1DT10H30M"),
    dayTimeDuration("P1DT10H30M")
)

//? source: ...

intersection<dayTimeDuration>(
    bag<dayTimeDuration>(
        dayTimeDuration("-P1DT10H30M"),
        dayTimeDuration("P1DT10H30M"),
        dayTimeDuration("-P1DT10H30M"),
        dayTimeDuration("P1DT10H30M"),
        dayTimeDuration("-P1DT10H35M")
    ),
    bag<dayTimeDuration>(
        dayTimeDuration("-P1DT10H30M"),
        dayTimeDuration("P1DT10H30M"),
        dayTimeDuration("P1DT10H30M"),
        dayTimeDuration("P1DT10H30M"),
        dayTimeDuration("P1DT10H30M"),
        dayTimeDuration("P1DT30M")
    )
)

//?
//? test: intersection<dayTimeDuration>-2
//? description: Invalid arguments test.
//? expect: error
//? source: ...

intersection<dayTimeDuration>(
    bag<dayTimeDuration>(
        dayTimeDuration("x"),
        dayTimeDuration("x")
    ),
    bag<dayTimeDuration>(
        dayTimeDuration("x"),
        dayTimeDuration("x")
    )
)

