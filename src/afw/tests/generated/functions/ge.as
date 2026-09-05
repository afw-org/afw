#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/ge.as
//? customPurpose: Part of core function tests
//? description: Error-path tests for polymorphic functions. Happy-path coverage lives in the per-dataType files (e.g. integer_ge.as).
//? sourceType: script
//?
//? test: ge-error-mismatched-types
//? description: GE arg2's generated signature is locked to arg1's data type (unlike eq/le/ne, whose arg2 is untyped), so this is caught by the generic argument evaluator before afw_function_execute_ge's own "Data types do not match" runtime check can ever run.
//? expect: error:Parameter 2 of function 'ge<integer>' must evaluate to data type 'integer' but evaluated to be 'string'
//? source: ...

ge(
    integer(6),
    string("6")
)
