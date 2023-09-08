#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/min.as
//? customPurpose: Part of core function tests
//? description: Tests for min function (polymorphic).
//? sourceType: script
//?
//? test: min-1
//? description: String
//? expect: string("a")
//? source: ...

min(
    string("extra"),
    string("x"),
    string("t"),
    string("r"),
    string("a")
)

//?
//? test: min-2
//? description: Number
//? expect: double(-12.8)
//? source: ...

min(
    double(-2.8),
    double(2.8),
    double(-12.8),
    double(12.8),
    double(0.0)
)

