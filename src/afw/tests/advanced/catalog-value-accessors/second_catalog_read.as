// Second request xctx — catalog still serves full objects.
// adapter_metrics and adapter_properties are managed snapshots.
const o = get_object(
    "afw",
    "_AdaptiveRuntimeValueAccessor_",
    "adapter_metrics");
assert(o.key === "adapter_metrics");
assert(o.returnsLiveReference === false);
assert(o.copiesUnderLock === false);
const p = get_object(
    "afw",
    "_AdaptiveRuntimeValueAccessor_",
    "adapter_properties");
assert(p.key === "adapter_properties");
assert(p.returnsLiveReference === false);
return o.key;
