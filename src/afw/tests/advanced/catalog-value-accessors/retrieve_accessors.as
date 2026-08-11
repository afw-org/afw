// maxObjects is 6th arg; 0 = unlimited (#49)
const list = retrieve_objects(
    "afw",
    "_AdaptiveRuntimeValueAccessor_",
    {},
    undefined,
    undefined,
    0);
assert(length(list) >= 20, "expected core value accessors");
let foundDefault = false;
let lockCopy = 0;
for (const e of list) {
    if (e.key === "default") {
        foundDefault = true;
        assert(e.brief !== null && e.brief !== undefined);
        assert(e.returnsLiveReference === true,
            "default should return live reference");
    }
    if (e.copiesUnderLock === true)
        lockCopy = lockCopy + 1;
}
assert(foundDefault, "missing default accessor");
assert(lockCopy >= 2, "expected at least stopping_* lock-copy accessors");
return length(list);
