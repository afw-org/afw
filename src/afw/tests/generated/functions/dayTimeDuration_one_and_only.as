#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dayTimeDuration_one_and_only.as
//? customPurpose: Part of core function tests
//? description: Tests for one_and_only<dayTimeDuration> function.
//? sourceType: expression
//?
//? test: one_and_only<dayTimeDuration>-1
//? description: One
//? expect: dayTimeDuration("PT1M30.5S")
//? source: ...

one_and_only<dayTimeDuration>(
    bag<dayTimeDuration>(
        dayTimeDuration("PT1M30.5S")
    )
)

//?
//? test: one_and_only<dayTimeDuration>-2
//? description: Two
//? expect: error
//? source: ...

one_and_only<dayTimeDuration>(
    bag<dayTimeDuration>(
        dayTimeDuration("PT1M30.5S"),
        dayTimeDuration("PT1M40.5S")
    )
)

