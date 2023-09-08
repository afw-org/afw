#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dayTimeDuration_bag.as
//? customPurpose: Part of core function tests
//? description: Tests for bag<dayTimeDuration> function.
//? sourceType: script
//?
//? test: bag<dayTimeDuration>-1
//? description: Two values
//? expect: ...

bag<dayTimeDuration>(
    dayTimeDuration("PT10H30M"),
    dayTimeDuration("PT10H10M")
)

//? source: ...

bag<dayTimeDuration>(
    dayTimeDuration("PT10H30M"),
    dayTimeDuration("PT10H10M")
)

//?
//? test: bag<dayTimeDuration>-2
//? description: Zero values
//? expect: ...

bag<dayTimeDuration>(
)

//? source: bag<dayTimeDuration>()
