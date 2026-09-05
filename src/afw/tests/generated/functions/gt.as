#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/gt.as
//? customPurpose: Part of core function tests
//? description: Tests for polymorphic functions.
//? sourceType: script
//?
//? test: gt-1
//? description: GT with integers
//? expect: boolean(true)
//? source: ...

gt(
    integer(6),
    integer(5)
)

//?
//? test: gt-error-mismatched-types
//? description: GT arg2's generated signature is locked to arg1's data type (unlike eq/le/ne, whose arg2 is untyped), so this is caught by the generic argument evaluator before afw_function_execute_gt's own "Data types do not match" runtime check can ever run.
//? expect: error:Parameter 2 of function 'gt<integer>' must evaluate to data type 'integer' but evaluated to be 'string'
//? source: ...

gt(
    integer(6),
    string("5")
)

