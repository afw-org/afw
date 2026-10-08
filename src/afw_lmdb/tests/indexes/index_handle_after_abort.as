#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: index_handle_after_abort.as
//? customPurpose: Part of lmdb tests
//? description: An index database first opened by a write that fails still works for the next write.
//? sourceType: script
//?
//? test: index_handle_after_abort
//? description: An index on all object types opens a database per object type on first use. When that first write failed, its transaction aborted and LMDB closed the handle, but the adapter had already cached it; every later index write for that type failed with EINVAL until restart.
//? expect: 1
//? source: ...
#!/usr/bin/env afw

const ot: string = "TestIndexHandleAbortType";

index_create("lmdb", "u", undefined, [], undefined, ["unique"], false, false);

let failed: boolean = false;
try {
    add_object("lmdb", ot, { u: ["a", "a"] }, generate_uuid());
}
catch (e) {
    failed = true;
}
assert(failed, "duplicate unique value was accepted");

add_object("lmdb", ot, { u: "b" }, generate_uuid());
const n = length(retrieve_objects("lmdb", ot,
    { "filter": { "op": "eq", "property": "u", "value": "b" } }));

index_remove("lmdb", "u");
return n;
