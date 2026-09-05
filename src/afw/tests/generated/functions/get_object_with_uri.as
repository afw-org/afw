#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/get_object_with_uri.as
//? customPurpose: Part of core function tests
//? description: Tests for the get_object_with_uri function.
//? sourceType: script
//?
//? test: get_object_with_uri-1
//? description: get_object_with_uri retrieves a runtime object by local path
//? expect: string("afw")
//? source: ...

get_object_with_uri(anyURI("/afw/_AdaptiveAdapter_/afw")).adapterId

//?
//? test: get_object_with_uri-error-invalid-uri
//? description: get_object_with_uri throws when the uri does not parse. The message comes from afw_uri_parse and is not asserted verbatim.
//? expect: error
//? source: ...

get_object_with_uri(anyURI("::::"))

//?
//? test: get_object_with_uri-error-not-local
//? description: get_object_with_uri throws when the uri is not a local entity object path
//? expect: error:Only local entity object paths are currently supported
//? source: ...

get_object_with_uri(anyURI("http://example.com/foo"))

//?
//? test: get_object_with_uri-error-not-found
//? description: get_object_with_uri throws when the object does not exist
//? expect: error:Not found
//? source: ...

get_object_with_uri(anyURI("/afw/_AdaptiveAdapter_/does-not-exist-xyz"))
