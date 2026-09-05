#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/adapter_objectCallback_signature.as
//? customPurpose: Part of core function tests
//? description: adapter_objectCallback_signature exists only to document the callback shape expected by retrieve_objects_to_callback / retrieve_objects_with_uri_to_callback. It always throws if called directly.
//? sourceType: script
//?
//? test: adapter_objectCallback_signature-error-do-not-call-directly
//? description: adapter_objectCallback_signature always throws "Do not call directly"
//? expect: error:Do not call directly
//? source: ...

adapter_objectCallback_signature({})
