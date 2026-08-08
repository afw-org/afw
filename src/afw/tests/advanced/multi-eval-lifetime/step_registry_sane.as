#!/usr/bin/env afw --syntax test_script
//?
//? testScript: step_registry_sane.as
//? description: Lightweight registry sanity after multi-eval bursts
//? sourceType: script
//?
//? test: object_types_nonempty
//? description: afw adapter still serves object type catalog
//? expect: true
//? source: ...

const types = retrieve_objects(
    "afw", "_AdaptiveObjectType_",
    undefined, undefined, undefined, 0);
assert(length(types) > 0);
return true;
