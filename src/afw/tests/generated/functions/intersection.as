#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/intersection.as
//? customPurpose: Part of core function tests
//? description: Error-path tests for polymorphic functions. Happy-path coverage lives in the per-dataType files (e.g. integer_intersection.as).
//? sourceType: script
//?
//? test: intersection-error-mismatched-types
//? description: intersection throws when array1 and array2 data types do not match
//? expect: error:array1 and array2 must have a data type of the same type
//? source: ...

intersection<integer>(
    bag<integer>(integer(1)),
    bag<string>(string("1"))
)
