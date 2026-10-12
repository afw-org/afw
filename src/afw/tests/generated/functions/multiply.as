#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/multiply.as
//? customPurpose: Part of core function tests
//? description: Tests for polymorphic functions.
//? sourceType: script
//?
//? test: multiply-1
//? description: MULTIPLY with integers
//? expect: 24
//? source: ...

multiply(
    2,
    3,
    4
)

//?
//? test: multiply-error-integer-overflow
//? description: MULTIPLY throws when the integer product overflows
//? expect: error:Integer multiply overflow
//? source: ...

multiply(
    9223372036854775807,
    2
)
