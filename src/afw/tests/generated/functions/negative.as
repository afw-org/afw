#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/negative.as
//? customPurpose: Part of core function tests
//? description: Tests for polymorphic functions.
//? sourceType: script
//?
//? test: negative-1
//? description: NEGATIVE of a positive integer
//? expect: -5
//? source: ...

negative(5)

//?
//? test: negative-error-integer-overflow
//? description: NEGATIVE throws when negating integer minimum value overflows
//? expect: error:Integer negate overflow
//? source: ...

negative(#integerMin)
