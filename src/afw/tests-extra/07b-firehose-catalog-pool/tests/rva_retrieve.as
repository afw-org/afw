const list = retrieve_objects(
    "afw",
    "_AdaptiveRuntimeValueAccessor_",
    {},
    undefined,
    undefined,
    0);
assert(length(list) >= 20, "expected core value accessors");
return length(list);
