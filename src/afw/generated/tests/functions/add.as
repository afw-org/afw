#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/add.as
//? customPurpose: Part of core function tests
//? description: Tests for polymorphic functions.
//? sourceType: expression
//?
//? test: add-1
//? description: ADD with integers
//? expect: integer(17)
//? source: ...

add(
    integer(6),
    integer(5),
    integer(4),
    integer(2)
)

