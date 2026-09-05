#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/log.as
//? customPurpose: Part of core function tests
//? description: log is currently an unimplemented stub (afw_function_execute_log always throws). This locks in that known behavior so a future implementation change is a deliberate test update, not a silent behavior change.
//? sourceType: script
//?
//? test: log-error-not-implemented
//? description: log always throws "Not implemented"
//? expect: error:Not implemented
//? source: ...

log("hello")
