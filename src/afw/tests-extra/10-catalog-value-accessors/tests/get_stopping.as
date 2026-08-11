const o = get_object(
    "afw",
    "_AdaptiveRuntimeValueAccessor_",
    "stopping_adapter_instances");
assert(o.key === "stopping_adapter_instances");
assert(o.copiesUnderLock === true);
assert(o.returnsLiveReference === false);
return o.key;
