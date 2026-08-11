// Second request xctx — catalog still serves full objects
const o = get_object(
    "afw",
    "_AdaptiveRuntimeValueAccessor_",
    "adapter_metrics");
assert(o.key === "adapter_metrics");
assert(o.returnsLiveReference === true);
assert(o.copiesUnderLock === false);
const p = get_object(
    "afw",
    "_AdaptiveRuntimeValueAccessor_",
    "adapter_properties");
assert(p.key === "adapter_properties");
assert(p.returnsLiveReference === true);
return o.key;
