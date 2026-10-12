#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/abs.as
//? customPurpose: Part of core function tests
//? description: Tests for polymorphic functions.
//? sourceType: script
//?
//? test: abs-1
//? description: ABS of a negative integer
//? expect: 5
//? source: ...

abs(-5)

//?
//? test: abs-error-integer-overflow
//? description: ABS throws when the integer minimum value cannot be represented as its positive equivalent
//? expect: error:Integer abs overflow
//? source: ...

abs((-9223372036854775807 - 1))
