#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/double_bag_size.as
//? customPurpose: Part of core function tests
//? description: Tests for bag_size<double> function.
//? sourceType: script
//?
//? test: bag_size<double>-1
//? description: Double bag size of 0.
//? expect: 0
//? source: ...

bag_size<double>(
    bag<double>(
    )
)

//?
//? test: bag_size<double>-2
//? description: Double bag size of 2.
//? expect: 2
//? source: ...

bag_size<double>(
    bag<double>(
        52.40215,
        -15.8
    )
)

//?
//? test: bag_size<double>-3
//? description: Double bag size of 21.
//? expect: 21
//? source: ...

bag_size<double>(
    bag<double>(
        2000.0215,
        11.0,
        -11.2151,
        961518.0,
        99.0,
        -102.1651054,
        -586.055,
        98600.0,
        0.15387,
        5126189454.0,
        -5126189454.0,
        0.0215200,
        11516.105155,
        -11.0,
        51081.0,
        0.99999999999,
        -102.0,
        -0.111115866,
        986.3211111,
        -0.0,
        0.000000
    )
)

