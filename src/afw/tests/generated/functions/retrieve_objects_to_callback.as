#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/retrieve_objects_to_callback.as
//? customPurpose: Part of core function tests
//? description: Tests for the retrieve_objects_to_callback function.
//? sourceType: script
//?
//? test: retrieve_objects_to_callback-error-callback-not-boolean
//? description: retrieve_objects_to_callback throws when objectCallback does not return a boolean
//? expect: error:objectCallback did not return boolean
//? source: ...

let cb = function (object, userData) {
    return "not-a-boolean";
};
return retrieve_objects_to_callback(cb, undefined, "afw", "_AdaptiveAdapter_");
