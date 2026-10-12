#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/double_add.as
//? customPurpose: Part of core function tests
//? description: Tests for add<double> function.
//? sourceType: script
//?
//? test: add<double>-1
//? description: Add an array
//? expect: 5.05
//? source: ...

add<double>(
    1.01,
    1.01,
    1.01,
    1.01,
    1.01
)

//?
//? test: add<double>-2
//? description: Add an array
//? expect: 555555.5
//? source: ...

add<double>(
    111111.1,
    111111.1,
    111111.1,
    111111.1,
    111111.1
)

//?
//? test: add<double>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

add<double>(
    to_double(x.0)
)

