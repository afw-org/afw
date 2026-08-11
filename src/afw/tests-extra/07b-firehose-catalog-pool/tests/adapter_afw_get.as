const a = get_object("afw", "_AdaptiveAdapter_", "afw");
assert(a.adapterId === "afw");
assert(a.referenceCount >= 1);
return a.referenceCount;
