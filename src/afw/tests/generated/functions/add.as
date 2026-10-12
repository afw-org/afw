#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/add.as
//? customPurpose: Part of core function tests
//? description: Tests for polymorphic functions.
//? sourceType: script
//?
//? test: add-1
//? description: ADD with integers
//? expect: 17
//? source: ...

add(
    6,
    5,
    4,
    2
)

//?
//? test: add-error-integer-overflow
//? description: ADD throws when the integer sum overflows
//? expect: error:Integer add overflow
//? source: ...

add(
    9223372036854775807,
    1
)

