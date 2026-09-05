#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/service_get.as
//? customPurpose: Part of core function tests
//? description: Tests for the service_get function.
//? sourceType: script
//?
//? test: service_get-error-not-found
//? description: service_get throws when the service does not exist
//? expect: error:Not found
//? source: ...

service_get("does-not-exist-service-xyz")
