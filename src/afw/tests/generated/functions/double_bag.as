#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/double_bag.as
//? customPurpose: Part of core function tests
//? description: Tests for bag<double> function.
//? sourceType: script
//?
//? test: bag<double>-1
//? description: Create empty double bag.
//? expect: ...

bag<double>(
)

//? source: bag<double>()
//?
//? test: bag<double>-2
//? description: Create double bag with 2 doubles.
//? expect: ...

bag<double>(
    52.40215,
    -15.8
)

//? source: ...

bag<double>(
    52.40215,
    -15.8
)

//?
//? test: bag<double>-3
//? description: Create double bag with 21 doubles.
//? expect: ...

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

//? source: ...

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

