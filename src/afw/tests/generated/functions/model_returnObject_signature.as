#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/model_returnObject_signature.as
//? customPurpose: Part of core function tests
//? description: model_returnObject_signature exists only to document the callback shape used internally by model mapping. It always throws if called directly.
//? sourceType: script
//?
//? test: model_returnObject_signature-error-do-not-call-directly
//? description: model_returnObject_signature always throws "Do not call directly"
//? expect: error:Do not call directly
//? source: ...

model_returnObject_signature({})
