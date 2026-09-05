#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/service_stop.as
//? customPurpose: Part of core function tests
//? description: Tests for the service_stop function.
//? sourceType: script
//?
//? test: service_stop-error-not-found
//? description: service_stop throws when the service does not exist. afw_service_stop throws before afw_function_execute_service_stop's own local "Not found" check ever runs, so that local check is currently dead code.
//? expect: error:Service 'does-not-exist-service-xyz' is not running
//? source: ...

service_stop("does-not-exist-service-xyz")
