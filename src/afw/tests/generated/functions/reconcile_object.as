#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/reconcile_object.as
//? customPurpose: Part of core function tests
//? description: Tests for the reconcile_object function.
//? sourceType: script
//?
//? test: reconcile_object-error-not-reconcilable
//? description: reconcile_object throws when the object was not retrieved with reconcilable meta (e.g. a plain object literal)
//? expect: error:object is not reconcilable
//? source: ...

reconcile_object({})
