const o = get_object(
    "afw",
    "_AdaptiveRuntimeValueAccessor_",
    "adapter_reference_count");
assert(o.key === "adapter_reference_count");
assert(o.copiesUnderLock === true);
return o.brief;
