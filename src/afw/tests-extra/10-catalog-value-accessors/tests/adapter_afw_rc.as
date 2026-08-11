const a = get_object("afw", "_AdaptiveAdapter_", "afw");
assert(a.adapterId === "afw");
assert(a.referenceCount !== null && a.referenceCount !== undefined);
assert(a.referenceCount >= 1, "active adapter should have refcount >= 1");
assert(a.metrics !== null && a.metrics !== undefined,
    "metrics present while adapter active");
return a.referenceCount;
