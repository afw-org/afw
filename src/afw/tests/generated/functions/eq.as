#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/eq.as
//? customPurpose: Part of core function tests
//? description: Error-path tests for polymorphic functions. Happy-path coverage lives in the per-dataType files (e.g. integer_eq.as).
//? sourceType: script
//?
//? test: eq-error-mismatched-types
//? description: EQ throws when arg1 and arg2 data types do not match
//? expect: error:Data types do not match
//? source: ...

eq(
    integer(6),
    string("6")
)
