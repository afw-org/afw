const types = retrieve_objects(
    "afw", "_AdaptiveObjectType_",
    undefined, undefined, undefined, 0);
assert(length(types) > 0);
return length(types);
