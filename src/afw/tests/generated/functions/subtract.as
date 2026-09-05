#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/subtract.as
//? customPurpose: Part of core function tests
//? description: Tests for polymorphic functions.
//? sourceType: script
//?
//? test: subtract-1
//? description: SUBTRACT with integers
//? expect: integer(6)
//? source: ...

subtract(
    integer(10),
    integer(4)
)

//?
//? test: subtract-error-integer-overflow
//? description: SUBTRACT throws when the integer difference overflows
//? expect: error:Integer subtract overflow
//? source: ...

subtract(
    integer(-9223372036854775807),
    integer(9223372036854775807)
)
