#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/retrieve_objects_to_response.as
//? customPurpose: Part of core function tests
//? description: retrieve_objects_to_response is a progressive-write function that requires an active HTTP-style request with an application/x-afw response content type (afwfcgi). The afw CLI test runner has no such request, so this call always exercises that guard here.
//? sourceType: script
//?
//? test: retrieve_objects_to_response-error-no-request
//? description: retrieve_objects_to_response throws when there is no active request with an application/x-afw response content type
//? expect: error:retrieve_objects_to_response() is only allowed to be called during a request with response content type "application/x-afw".
//? source: ...

retrieve_objects_to_response("afw", "_AdaptiveAdapter_")
