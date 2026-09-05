#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/random_base64Binary.as
//? customPurpose: Part of core function tests
//? description: Tests for the random_base64Binary function.
//? sourceType: script
//?
//? test: random_base64Binary-error-negative-numberOfOctets
//? description: random_base64Binary throws when numberOfOctets is negative
//? expect: error:numberOfOctets must be between 0 and 1000000 inclusive
//? source: ...

random_base64Binary(integer(-1))
