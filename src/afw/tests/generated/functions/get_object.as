#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/get_object.as
//? customPurpose: Part of core function tests
//? description: Tests for the get_object function.
//? sourceType: script
//?
//? test: get_object-1
//? description: get_object retrieves a runtime object from the built-in afw adapter
//? expect: string("afw")
//? source: ...

get_object("afw", "_AdaptiveAdapter_", "afw").adapterId

//?
//? test: get_object-error-not-found
//? description: get_object throws when the object does not exist
//? expect: error:Not found
//? source: ...

get_object(
    "afw",
    "_AdaptiveAdapter_",
    "does-not-exist-xyz"
)
