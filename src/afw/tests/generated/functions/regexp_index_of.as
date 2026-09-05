#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/regexp_index_of.as
//? customPurpose: Part of core function tests
//? description: regexp_index_of is currently an unimplemented stub (afw_function_execute_regexp_index_of always throws). This locks in that known behavior so a future implementation change is a deliberate test update, not a silent behavior change.
//? sourceType: script
//?
//? test: regexp_index_of-error-not-implemented
//? description: regexp_index_of always throws "Not implemented"
//? expect: error:Not implemented
//? source: ...

regexp_index_of(
    string("abc"),
    string("a")
)
