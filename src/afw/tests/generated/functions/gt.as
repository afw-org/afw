#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/gt.as
//? customPurpose: Part of core function tests
//? description: Tests for polymorphic functions.
//? sourceType: expression
//?
//? test: gt-1
//? description: GT with integers
//? expect: boolean(true)
//? source: ...

gt(
    integer(6),
    integer(5)
)

