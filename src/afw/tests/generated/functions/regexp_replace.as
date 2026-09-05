#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/regexp_replace.as
//? customPurpose: Part of core function tests
//? description: regexp_replace is currently an unimplemented stub (afw_function_execute_regexp_replace always throws). This locks in that known behavior so a future implementation change is a deliberate test update, not a silent behavior change.
//? sourceType: script
//?
//? test: regexp_replace-error-not-implemented
//? description: regexp_replace always throws "Not implemented"
//? expect: error:Not implemented
//? source: ...

regexp_replace(
    string("abc"),
    string("a"),
    string("b")
)
