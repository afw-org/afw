#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/negative.as
//? customPurpose: Part of core function tests
//? description: Tests for polymorphic functions.
//? sourceType: script
//?
//? test: negative-1
//? description: NEGATIVE of a positive integer
//? expect: integer(-5)
//? source: ...

negative(integer(5))

//?
//? test: negative-error-integer-overflow
//? description: NEGATIVE throws when negating integer minimum value overflows
//? expect: error:Integer negate overflow
//? source: ...

negative(integer(-9223372036854775807 - 1))
