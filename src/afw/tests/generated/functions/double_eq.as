#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/double_eq.as
//? customPurpose: Part of core function tests
//? description: Tests for eq<double> function.
//? sourceType: script
//?
//? test: eq<double>-1
//? description: Equal
//? expect: boolean(true)
//? source: ...

eq<double>(
    double(1234567.8),
    double(1234567.8)
)

//?
//? test: eq<double>-2
//? description: Not Equal
//? expect: boolean(false)
//? source: ...

eq<double>(
    double(1234567.0),
    double(1234568.0)
)

//?
//? test: eq<double>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

eq<double>(
    double(x.0),
    double(x.0)
)

