#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dayTimeDuration_bag_size.as
//? customPurpose: Part of core function tests
//? description: Tests for bag_size<dayTimeDuration> function.
//? sourceType: script
//?
//? test: bag_size<dayTimeDuration>-1
//? description: 7 items
//? expect: 7
//? source: ...

bag_size<dayTimeDuration>(
    bag<dayTimeDuration>(
        "PT10H30M"dayTimeDuration,
        "PT10H40M"dayTimeDuration,
        "PT10H50M"dayTimeDuration,
        "PT10H10M"dayTimeDuration,
        "PT10H20M"dayTimeDuration,
        "PT10H31M"dayTimeDuration,
        "PT10H32M"dayTimeDuration
    )
)

//?
//? test: bag_size<dayTimeDuration>-2
//? description: 3 items
//? expect: 3
//? source: ...

bag_size<dayTimeDuration>(
    bag<dayTimeDuration>(
        "PT10H30M"dayTimeDuration,
        "PT10H40M"dayTimeDuration,
        "PT10H50M"dayTimeDuration
    )
)

