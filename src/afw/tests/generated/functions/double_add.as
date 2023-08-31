#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/double_add.as
//? customPurpose: Part of core function tests
//? description: Tests for add<double> function.
//? sourceType: expression
//?
//? test: add<double>-1
//? description: Add an array
//? expect: double(5.05)
//? source: ...

add<double>(
    double(1.01),
    double(1.01),
    double(1.01),
    double(1.01),
    double(1.01)
)

//?
//? test: add<double>-2
//? description: Add an array
//? expect: double(555555.5)
//? source: ...

add<double>(
    double(111111.1),
    double(111111.1),
    double(111111.1),
    double(111111.1),
    double(111111.1)
)

//?
//? test: add<double>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

add<double>(
    double(x.0)
)

