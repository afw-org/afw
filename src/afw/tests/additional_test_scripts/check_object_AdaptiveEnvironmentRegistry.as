#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: check_object_AdaptiveEnvironmentRegistry.as
//? customPurpose: Part of core function tests
//? description: Checks for "hasErrors" reported on /afw/_AdaptiveEnvironmentRegistry_/current. This is a runtime object that calls many different value accessors and accesses much of the AFW environment.
//? sourceType: script
//?
//? test: check_object_errors
//? description: Checks for "hasErrors" reported on /afw/_AdaptiveEnvironmentRegistry_/current
//? expect: 0
//? source: ...

const obj: object = get_object("afw", "_AdaptiveEnvironmentRegistry_", "current");

print(stringify(meta(obj),,"\t"));

assert(
    meta(obj)->property_get("hasErrors", false) == false, 
    "Object " + meta(obj).path + " has errors"
);  

return 0;
