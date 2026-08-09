#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: rva_lock_copy_set.as
//? customPurpose: tests_special — value accessor catalog (#149)
//? description: Known copiesUnderLock accessors are registered
//? sourceType: script
//?
//? test: lock_copy_keys_present
//? description: stopping_* and reference_count accessors
//? expect: 0
//? source: ...

const keys = [
    "stopping_adapter_instances",
    "stopping_authorization_handler_instances",
    "adapter_reference_count",
    "authorization_handler_reference_count"
];
for (const k of keys) {
    const o = get_object("afw", "_AdaptiveRuntimeValueAccessor_", k);
    assert(o !== null && o !== undefined, "missing " + k);
    assert(o.copiesUnderLock === true, k + " should lock-copy");
    assert(o.returnsLiveReference === false, k + " should not be live ref");
}
return 0;
