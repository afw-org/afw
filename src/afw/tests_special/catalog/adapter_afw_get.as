#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: adapter_afw_get.as
//? customPurpose: tests_special — adapter anchor (#149)
//? description: Get runtime adapter afw and check referenceCount
//? sourceType: script
//?
//? test: adapter_afw_reference_count
//? description: adapter afw has integer referenceCount
//? expect: 0
//? source: ...

const a = get_object("afw", "_AdaptiveAdapter_", "afw");
assert(a !== null && a !== undefined, "missing adapter afw");
assert(a.adapterId === "afw", "adapterId");
assert(a.referenceCount !== undefined && a.referenceCount !== null,
    "referenceCount present");
assert(a.referenceCount >= 0, "referenceCount non-negative");
return 0;
