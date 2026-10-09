#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: index_remove_clear.as
//? customPurpose: Part of lmdb tests
//? description: index_remove clears an index's databases, and index_create reuses one only with the same options (#511).
//? sourceType: script
//?
//? test: index_remove_clears_all_types
//? description: Removing an index on all object types left the entries of each object type's database. An object changed while the index was gone was still found by its old value once the index was created again.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const ot: string = "TestIndexClearAllTypes";
const id: string = generate_uuid();

index_create("lmdb", "cq", undefined, [], undefined, undefined, false, false);
add_object("lmdb", ot, { cq: "old" }, id);
index_remove("lmdb", "cq");

modify_object("lmdb", ot, id, [["set_property", "cq", "new"]]);
index_create("lmdb", "cq", undefined, [], undefined, undefined, false, false);

const n = length(retrieve_objects("lmdb", ot,
    { "filter": { "op": "eq", "property": "cq", "value": "old" } }));
index_remove("lmdb", "cq");
return n;

//?
//? test: index_remove_clears_no_object_type
//? description: Removing an index created with no objectType at all cleared nothing.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const ot: string = "TestIndexClearNoObjectType";
const id: string = generate_uuid();

index_create("lmdb", "nq", undefined, undefined, undefined, undefined, false, false);
add_object("lmdb", ot, { nq: "old" }, id);
index_remove("lmdb", "nq");

modify_object("lmdb", ot, id, [["set_property", "nq", "new"]]);
index_create("lmdb", "nq", undefined, undefined, undefined, undefined, false, false);

const n = length(retrieve_objects("lmdb", ot,
    { "filter": { "op": "eq", "property": "nq", "value": "old" } }));
index_remove("lmdb", "nq");
return n;

//?
//? test: index_recreate_same_options
//? description: An index removed and created again with the same options reuses its cleared database.
//? expect: 1
//? source: ...
#!/usr/bin/env afw

const ot: string = "TestIndexRecreateSame";

index_create("lmdb", "sq", undefined, [ot], undefined, ["unique"], false, false);
add_object("lmdb", ot, { sq: "a" }, generate_uuid());
index_remove("lmdb", "sq");

index_create("lmdb", "sq", undefined, [ot], undefined, ["unique"], true, false);
const n = length(retrieve_objects("lmdb", ot,
    { "filter": { "op": "eq", "property": "sq", "value": "a" } }));
index_remove("lmdb", "sq");
return n;

//?
//? test: index_recreate_different_options
//? description: An index created again with different options than the removed one throws until AFW restarts, since its cleared database keeps the old flags.
//? expect: true
//? source: ...
#!/usr/bin/env afw

const ot: string = "TestIndexRecreateDifferent";

index_create("lmdb", "dq", undefined, [ot], undefined, undefined, false, false);
add_object("lmdb", ot, { dq: "a" }, generate_uuid());
index_remove("lmdb", "dq");

let message: string = "";
try {
    index_create("lmdb", "dq", undefined, [ot], undefined, ["unique"], false, false);
}
catch (e) {
    message = e.message;
}
assert(includes<string>(message, "Restart AFW"), "expected a restart error, got: " + message);

const l = index_list("lmdb");
return l.dq === undefined;
