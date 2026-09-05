#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/create_array.as
//? customPurpose: Part of core function tests
//? description: Tests for the create_array function.
//? sourceType: script
//?
//? test: create_array-1
//? description: create_array creates a pre-sized array of undefined slots
//? expect: 0
//? source: ...

assert(length(create_array(integer(3))) === 3);
return 0;

//?
//? test: create_array-error-negative-length
//? description: create_array throws when length is negative
//? expect: error:create_array length must be between 0 and 1000000 inclusive
//? source: ...

create_array(integer(-1))
