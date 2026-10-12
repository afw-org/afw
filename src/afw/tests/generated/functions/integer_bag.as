#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/integer_bag.as
//? customPurpose: Part of core function tests
//? description: Tests for bag<integer> function.
//? sourceType: script
//?
//? test: bag<integer>-1
//? description: Create empty integer bag.
//? expect: ...

bag<integer>(
)

//? source: bag<integer>()
//?
//? test: bag<integer>-2
//? description: Create integer bag with 2 integers.
//? expect: ...

bag<integer>(
    52,
    -15
)

//? source: ...

bag<integer>(
    52,
    -15
)

//?
//? test: bag<integer>-3
//? description: Create integer bag with 21 integers.
//? expect: ...

bag<integer>(
    200,
    11,
    -11,
    96,
    99,
    -102,
    -586,
    986,
    387,
    5126189454,
    -5126189454,
    200,
    11,
    -11,
    96,
    99,
    -102,
    -586,
    986,
    0,
    0
)

//? source: ...

bag<integer>(
    200,
    11,
    -11,
    96,
    99,
    -102,
    -586,
    986,
    387,
    5126189454,
    -5126189454,
    200,
    11,
    -11,
    96,
    99,
    -102,
    -586,
    986,
    -0,
    0
)

