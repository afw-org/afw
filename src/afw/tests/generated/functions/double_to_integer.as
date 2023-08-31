#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/double_to_integer.as
//? customPurpose: Part of core function tests
//? description: Tests for to_integer<double> function.
//? sourceType: expression
//?
//? test: to_integer<double>-1
//? description: 50.0666
//? expect: integer(50)
//? source: ...

to_integer<double>(
    double(50.0666)
)

//?
//? test: to_integer<double>-2
//? description: -50.0666
//? expect: integer(-50)
//? source: ...

to_integer<double>(
    double(-50.0666)
)

//?
//? test: to_integer<double>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

to_integer<double>(
    double(NaN)
)

