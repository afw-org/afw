const o = get_object(
    "afw",
    "_AdaptiveRuntimeValueAccessor_",
    "stopping_adapter_instances");
assert(o.key === "stopping_adapter_instances");
assert(o.copiesUnderLock === true);
return o.key;
