#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/service_start.as
//? customPurpose: Part of core function tests
//? description: Tests for the service_start function.
//? sourceType: script
//?
//? test: service_start-error-not-found
//? description: service_start throws when the service does not exist. afw_service_start throws before afw_function_execute_service_start's own local "Not found" check ever runs, so that local check is currently dead code.
//? expect: error:Can not start service 'does-not-exist-service-xyz'
//? source: ...

service_start("does-not-exist-service-xyz")
