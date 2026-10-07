#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: index_remove_recreate.as
//? customPurpose: Part of lmdb tests
//? description: An index removed and created again in the same process keeps working.
//? sourceType: script
//?
//? test: index_remove_recreate
//? description: index_remove drops the index database, which closes its handle; the adapter used to keep the closed handle, so the next index write failed ("Unable to add index value.", EINVAL) or reached another database that reused the handle.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const ot: string = "TestIndexRecreateType";
const q = function (v: string): integer {
    return length(retrieve_objects("lmdb", ot,
        { "filter": { "op": "eq", "property": "surname", "value": v } }));
};

index_create("lmdb", "surname", undefined, [ot], undefined, undefined, false, false);
add_object("lmdb", ot, { surname: "Smith" }, generate_uuid());

for (const round of [1, 2, 3]) {
    index_remove("lmdb", "surname");
    index_create("lmdb", "surname", undefined, [ot], undefined, undefined, true, false);

    const id: string = generate_uuid();
    add_object("lmdb", ot, { surname: "Jones" }, id);
    modify_object("lmdb", ot, id, [["set_property", "surname", "Brown"]]);
    assert(q("Smith") === 1, "round " + string(round) + ": Smith");
    assert(q("Brown") === 1, "round " + string(round) + ": Brown");
    assert(q("Jones") === 0, "round " + string(round) + ": Jones");
    delete_object("lmdb", ot, id);
}

index_remove("lmdb", "surname");
return 0;
