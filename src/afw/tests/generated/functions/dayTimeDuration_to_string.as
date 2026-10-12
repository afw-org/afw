#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dayTimeDuration_to_string.as
//? customPurpose: Part of core function tests
//? description: Tests for to_string<dayTimeDuration> function.
//? sourceType: script
//?
//? test: to_string<dayTimeDuration>-1
//? description: x
//? expect: string("P1DT1H")
//? source: ...

to_string<dayTimeDuration>(
    "P1DT1H"dayTimeDuration
)

//?
//? test: to_string<dayTimeDuration>-2
//? description: x
//? expect: string("-P1DT1H1M30.5S")
//? source: ...

to_string<dayTimeDuration>(
    "-P1DT1H1M30.5S"dayTimeDuration
)

//?
//? test: to_string<dayTimeDuration>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

to_string<dayTimeDuration>(
    "x"dayTimeDuration
)

