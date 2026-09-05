#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/union.as
//? customPurpose: Part of core function tests
//? description: Error-path tests for polymorphic functions. Happy-path coverage lives in the per-dataType files (e.g. integer_union.as).
//? sourceType: script
//?
//? test: union-error-mismatched-types
//? description: union throws when the arrays do not all have the same data type
//? expect: error:all arrays must have the same data type
//? source: ...

union<integer>(
    bag<integer>(integer(1)),
    bag<string>(string("1"))
)
