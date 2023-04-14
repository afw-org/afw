#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/max.as
//? customPurpose: Part of core function tests
//? description: Tests for max function (polymorphic).
//? sourceType: expression
//?
//? test: max-1
//? description: String
//? expect: string("x")
//? source: ...

max(
    string("extra"),
    string("x"),
    string("t"),
    string("r"),
    string("a")
)

//?
//? test: max-2
//? description: Number
//? expect: double(12.8)
//? source: ...

max(
    double(-2.8),
    double(2.8),
    double(-12.8),
    double(12.8),
    double(0.0)
)

