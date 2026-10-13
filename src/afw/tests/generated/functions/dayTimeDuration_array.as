#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dayTimeDuration_array.as
//? customPurpose: Part of core function tests
//? description: Tests for array<dayTimeDuration> function.
//? sourceType: script
//?
//? test: array<dayTimeDuration>-1
//? description: Two values
//? expect: ...

array<dayTimeDuration>(
    "PT10H30M"dayTimeDuration,
    "PT10H10M"dayTimeDuration
)

//? source: ...

array<dayTimeDuration>(
    "PT10H30M"dayTimeDuration,
    "PT10H10M"dayTimeDuration
)

//?
//? test: array<dayTimeDuration>-2
//? description: Zero values
//? expect: ...

array<dayTimeDuration>(
)

//? source: array<dayTimeDuration>()
