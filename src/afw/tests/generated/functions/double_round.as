#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/double_round.as
//? customPurpose: Part of core function tests
//? description: Tests for round<double> function.
//? sourceType: script
//?
//? test: round<double>-1
//? description: x
//? expect: double(5.0)
//? source: ...

round<double>(
    double(5.2)
)

//?
//? test: round<double>-2
//? description: x
//? expect: double(6.0)
//? source: ...

round<double>(
    double(5.5)
)

//?
//? test: round<double>-3
//? description: x
//? expect: double(888888888888888886.0)
//? source: ...

round<double>(
    double(888888888888888885.5)
)

//?
//? test: round<double>-4
//? description: Invalid arguments test.
//? expect: error
//? source: ...

round<double>(
    double(x.0)
)

