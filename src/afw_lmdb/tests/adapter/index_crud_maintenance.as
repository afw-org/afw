#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: index_crud_maintenance.as
//? customPurpose: Part of lmdb tests
//? description: Index maintenance through normal add/modify/delete CRUD (issue #57), not just retroactive index_create.
//? sourceType: script
//?
//? test: index_crud_maintenance
//? description: An index created before any objects exist is correctly maintained by add_object, modify_object, and delete_object (regression for issue #232's LMDB indexer stale-txn bug, which broke every indexed write, not just retroactive builds).
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const ot: string = "TestIndexCrudType";
const id: string = generate_uuid();

// Index the "surname" property directly (value script omitted: key IS the
// property name). Created before any matching objects exist.
const created: object = index_create("lmdb", "surname", undefined, [ot], undefined, undefined, false, false);
assert(created.num_indexed === 0, "non-retroactive create over an empty adapter should index nothing yet");

// add_object should maintain the index (exercises impl_afw_adapter_impl_index_add).
add_object("lmdb", ot, { surname: "Smith" }, id);

const afterAdd: array = retrieve_objects("lmdb", ot,
    { "filter": { "op": "eq", "property": "surname", "value": "Smith" } });
assert(length(afterAdd) === 1, "object should be found by its indexed value after add_object");

// modify_object should reindex (delete old value, add new value).
modify_object("lmdb", ot, id, [["set_property", "surname", "Johnson"]]);

const oldValueGone: array = retrieve_objects("lmdb", ot,
    { "filter": { "op": "eq", "property": "surname", "value": "Smith" } });
assert(length(oldValueGone) === 0, "old indexed value should no longer match after modify_object");

const newValuePresent: array = retrieve_objects("lmdb", ot,
    { "filter": { "op": "eq", "property": "surname", "value": "Johnson" } });
assert(length(newValuePresent) === 1, "new indexed value should match after modify_object");

// delete_object should unindex (exercises impl_afw_adapter_impl_index_delete).
delete_object("lmdb", ot, id);

const afterDelete: array = retrieve_objects("lmdb", ot,
    { "filter": { "op": "eq", "property": "surname", "value": "Johnson" } });
assert(length(afterDelete) === 0, "object should no longer be found by its indexed value after delete_object");

safe_evaluate(index_remove("lmdb", "surname"), null);

return 0;
