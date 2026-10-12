#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/integer_array.as
//? customPurpose: Part of core function tests
//? description: Tests for array<integer> function.
//? sourceType: script
//?
//? test: array<integer>-1
//? description: Create empty integer array.
//? expect: ...

array<integer>(
)

//? source: array<integer>()
//?
//? test: array<integer>-2
//? description: Create integer array with 2 integers.
//? expect: ...

array<integer>(
    52,
    -15
)

//? source: ...

array<integer>(
    52,
    -15
)

//?
//? test: array<integer>-3
//? description: Create integer array with 21 integers.
//? expect: ...

array<integer>(
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

array<integer>(
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

