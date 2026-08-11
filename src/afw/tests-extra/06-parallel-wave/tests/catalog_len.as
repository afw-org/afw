// maxObjects 0 = unlimited catalog retrieve (#49)
const types = retrieve_objects(
    "afw", "_AdaptiveObjectType_",
    undefined, undefined, undefined, 0);
assert(length(types) > 0, "expected at least one object type");
return length(types);
