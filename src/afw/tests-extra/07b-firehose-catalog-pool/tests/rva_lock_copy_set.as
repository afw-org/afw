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
