const a = get_object("afw", "_AdaptiveAdapter_", "afw");
assert(a !== null && a !== undefined);
assert(a.metrics !== null && a.metrics !== undefined,
    "metrics should be present while adapter active");
assert(a.metrics.getObjectCount !== undefined,
    "metrics.getObjectCount");
return true;
