#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dayTimeDuration_set_equals.as
//? customPurpose: Part of core function tests
//? description: Tests for set_equals<dayTimeDuration> function.
//? sourceType: expression
//?
//? test: set_equals<dayTimeDuration>-1
//? description: equals
//? expect: boolean(true)
//? source: ...

set_equals<dayTimeDuration>(
    bag<dayTimeDuration>(
        dayTimeDuration("PT1M40.5S"),
        dayTimeDuration("PT1M30.5S")
    ),
    bag<dayTimeDuration>(
        dayTimeDuration("PT1M30.5S"),
        dayTimeDuration("PT1M40.5S")
    )
)

//?
//? test: set_equals<dayTimeDuration>-2
//? description: not equal
//? expect: boolean(false)
//? source: ...

set_equals<dayTimeDuration>(
    bag<dayTimeDuration>(
        dayTimeDuration("PT1M40.5S"),
        dayTimeDuration("PT1M30.5S")
    ),
    bag<dayTimeDuration>(
        dayTimeDuration("PT1M30.5S"),
        dayTimeDuration("PT1M50.5S")
    )
)

