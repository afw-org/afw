#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: rva_retrieve.as
//? customPurpose: blast_corpus — value accessor catalog (#149)
//? description: Retrieve all runtime value accessor objects (maxObjects 0)
//? sourceType: script
//?
//? test: rva_retrieve_all
//? description: Core catalog is non-empty and includes default
//? expect: 0
//? source: ...

const list = retrieve_objects(
    "afw",
    "_AdaptiveRuntimeValueAccessor_",
    {},
    undefined,
    undefined,
    0
);
assert(length(list) >= 1, "expected registered value accessors");
let foundDefault = false;
for (const e of list) {
    if (e.key === "default") {
        foundDefault = true;
        assert(e.brief !== undefined && e.brief !== null, "default.brief");
        break;
    }
}
assert(foundDefault, "missing key default");
return 0;
