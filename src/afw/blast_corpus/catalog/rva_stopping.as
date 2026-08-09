#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: rva_stopping.as
//? customPurpose: blast_corpus — value accessor catalog (#149)
//? description: Get lock-copy stopping accessor catalog entry
//? sourceType: script
//?
//? test: rva_stopping_entry
//? description: stopping_adapter_instances is copiesUnderLock
//? expect: 0
//? source: ...

const o = get_object(
    "afw",
    "_AdaptiveRuntimeValueAccessor_",
    "stopping_adapter_instances"
);
assert(o !== null && o !== undefined, "missing stopping_adapter_instances");
assert(o.key === "stopping_adapter_instances", "key");
assert(o.copiesUnderLock === true, "expected copiesUnderLock");
assert(o.returnsLiveReference === false, "expected not live ref");
return 0;
