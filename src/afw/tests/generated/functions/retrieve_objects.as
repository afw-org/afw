#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/retrieve_objects.as
//? customPurpose: Part of core function tests
//? description: Tests for the retrieve_objects function.
//? sourceType: script
//?
//? test: retrieve_objects-1
//? description: retrieve_objects returns an array of objects from the built-in afw adapter
//? expect: success
//? source: ...

retrieve_objects("afw", "_AdaptiveAdapter_")

//?
//? test: retrieve_objects-error-negative-maxObjects
//? description: retrieve_objects throws when maxObjects is negative
//? expect: error:Parameter maxObjects must be >= 0
//? source: ...

retrieve_objects(
    "afw",
    "_AdaptiveAdapter_",
    undefined,
    undefined,
    undefined,
    integer(-1)
)

//?
//? test: retrieve_objects-error-limit-exceeded
//? description: retrieve_objects throws when the number of matching objects exceeds maxObjects. _AdaptiveFunction_ has well over 1 registered instance in the afw adapter.
//? expect: error:Object retrieve limit exceeded.
//? source: ...

retrieve_objects(
    "afw",
    "_AdaptiveFunction_",
    undefined,
    undefined,
    undefined,
    integer(1)
)
