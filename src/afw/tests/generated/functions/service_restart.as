#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/service_restart.as
//? customPurpose: Part of core function tests
//? description: Tests for the service_restart function.
//? sourceType: script
//?
//? test: service_restart-error-not-found
//? description: service_restart throws when the service does not exist. afw_service_restart throws before afw_function_execute_service_restart's own local "Not found" check ever runs, so that local check is currently dead code.
//? expect: error:Service 'does-not-exist-service-xyz' cannot be restarted.  It is not running
//? source: ...

service_restart("does-not-exist-service-xyz")
