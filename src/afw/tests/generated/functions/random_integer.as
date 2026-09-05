#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/random_integer.as
//? customPurpose: Part of core function tests
//? description: Tests for the random_integer function.
//? sourceType: script
//?
//? test: random_integer-error-min-greater-than-max
//? description: random_integer throws when min is greater than max
//? expect: error:range between min and max is out of bounds
//? source: ...

random_integer(integer(10), integer(5))
