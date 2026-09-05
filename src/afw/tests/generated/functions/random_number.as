#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/random_number.as
//? customPurpose: Part of core function tests
//? description: Tests for the random_number function.
//? sourceType: script
//?
//? test: random_number-error-min-greater-than-max
//? description: random_number throws when min is greater than max
//? expect: error:range between min and max is out of bounds
//? source: ...

random_number(double(5.0), double(1.0))
