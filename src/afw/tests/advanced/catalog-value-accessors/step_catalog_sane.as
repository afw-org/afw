#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: step_catalog_sane.as
//? customPurpose: orchestrated-test sketch #149 catalog leaf
//? description: Hybrid test_script step after multi-request catalog evals
//? sourceType: script
//?
//? test: authorization_handler_reference_count_entry
//? description: auth handler lock-copy accessor is registered
//? expect: 0
//? source: ...

const o = get_object(
    "afw",
    "_AdaptiveRuntimeValueAccessor_",
    "authorization_handler_reference_count"
);
assert(o !== null && o !== undefined);
assert(o.key === "authorization_handler_reference_count");
assert(o.copiesUnderLock === true);
return 0;
