#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dayTimeDuration_eq.as
//? customPurpose: Part of core function tests
//? description: Tests for eq<dayTimeDuration> function.
//? sourceType: script
//?
//? test: eq<dayTimeDuration>-1
//? description: Are equal
//? expect: boolean(true)
//? source: ...

eq<dayTimeDuration>(
    dayTimeDuration("PT10H30M"),
    dayTimeDuration("PT10H30M")
)

//?
//? test: eq<dayTimeDuration>-2
//? description: Are not equal
//? expect: boolean(false)
//? source: ...

eq<dayTimeDuration>(
    dayTimeDuration("PT10H30M"),
    dayTimeDuration("PT10H31M")
)

