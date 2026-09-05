#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/random_digits.as
//? customPurpose: Part of core function tests
//? description: Tests for the random_digits function.
//? sourceType: script
//?
//? test: random_digits-1
//? description: random_digits returns a string of the requested length
//? expect: 0
//? source: ...

assert(length(random_digits(integer(5))) === 5);
return 0;

//?
//? test: random_digits-error-too-few
//? description: random_digits throws when numberOfDigits is less than 1
//? expect: error:numberOfDigits can be 1-18
//? source: ...

random_digits(integer(0))

//?
//? test: random_digits-error-too-many
//? description: random_digits throws when numberOfDigits is greater than 18
//? expect: error:numberOfDigits can be 1-18
//? source: ...

random_digits(integer(19))
