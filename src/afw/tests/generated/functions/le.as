#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/le.as
//? customPurpose: Part of core function tests
//? description: Error-path tests for polymorphic functions. Happy-path coverage lives in the per-dataType files (e.g. integer_le.as).
//? sourceType: script
//?
//? test: le-error-mismatched-types
//? description: LE throws when arg1 and arg2 data types do not match
//? expect: error:Data types do not match
//? source: ...

le(
    integer(6),
    string("6")
)
