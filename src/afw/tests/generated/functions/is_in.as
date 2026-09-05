#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/is_in.as
//? customPurpose: Part of core function tests
//? description: Error-path tests for polymorphic functions. Happy-path coverage lives in the per-dataType files (e.g. integer_is_in.as).
//? sourceType: script
//?
//? test: is_in-error-mismatched-types
//? description: is_in throws when value's data type does not match array's data type
//? expect: error:array must be array of value's data type
//? source: ...

is_in<integer>(
    integer(6),
    bag<string>(string("6"))
)
