#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/max.as
//? customPurpose: Part of core function tests
//? description: Tests for max function (polymorphic).
//? sourceType: script
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
//? expect: 12.8
//? source: ...

max(
    -2.8,
    2.8,
    -12.8,
    12.8,
    0.0
)

