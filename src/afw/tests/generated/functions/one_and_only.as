#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/one_and_only.as
//? customPurpose: Part of core function tests
//? description: Error-path tests for polymorphic functions. Happy-path coverage lives in the per-dataType files (e.g. integer_one_and_only.as).
//? sourceType: script
//?
//? test: one_and_only-error-too-many
//? description: one_and_only throws when array has more than one value
//? expect: error:arg must have exactly one value
//? source: ...

one_and_only<integer>(
    bag<integer>(integer(1), integer(2))
)

//?
//? test: one_and_only-error-empty
//? description: one_and_only throws when array is empty
//? expect: error:arg must have exactly one value
//? source: ...

one_and_only<integer>(
    bag<integer>()
)
