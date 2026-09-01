#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: adapter_non_uuid_id_tests.as
//? customPurpose: Part of lmdb tests
//? description: Test lmdb adapter CRUD with a non-UUID (human-readable) objectId (#244).
//? sourceType: script
//?
//? test: adapter_non_uuid_id_tests
//? description: Script to test lmdb adapter with non-UUID suggested objectIds.
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw


let object: object;
let result: object;

const humanId: string = "MyCustomId";

// create an object, specifying a non-UUID objectId
result = add_object('lmdb', '_AdaptiveObject_', { data: "first" }, humanId);
assert((result.objectId == humanId), "objectId was not preserved for a non-UUID suggested_object_id");

// get it back by the same human id
object = get_object('lmdb', '_AdaptiveObject_', humanId, { objectId: true });
assert((meta(object).objectId == humanId), "objectId from get_object does not match the human id");
assert((object.data == "first"), "data does not match after add_object with a human id");

// retrieve_objects (full-type scan) must also report the human alias,
// not the internal uuid it's actually stored under
let found: boolean = false;
const objects: array = retrieve_objects('lmdb', '_AdaptiveObject_', undefined, { objectId: true });
for (const obj: object of objects) {
    if (meta(obj).objectId == humanId) {
        found = true;
        assert(obj.data == "first", "data does not match in retrieve_objects for a human id");
    }
}
assert(found, "retrieve_objects did not report the human alias as objectId");

// a second object with the same human id must fail (alias is not silently reused)
assert(
    safe_evaluate(
        add_object('lmdb', '_AdaptiveObject_', {}, humanId),
        "error"
    ) == "error",
    "add_object should not silently accept a duplicate human objectId"
);

// replace it
replace_object('lmdb', '_AdaptiveObject_', humanId, { data: "second" });
object = get_object('lmdb', '_AdaptiveObject_', humanId);
assert(object.data == "second", "Data doesn't match 'second' after replace_object with a human id");

// modify it
modify_object('lmdb', '_AdaptiveObject_', humanId,
    [["set_property", "data", "third"]]);
object = get_object('lmdb', '_AdaptiveObject_', humanId);
assert(object.data == "third", "Data doesn't match 'third' after modify_object with a human id");

// delete it
delete_object('lmdb', '_AdaptiveObject_', humanId);

// verify it's gone
assert(
    safe_evaluate(
        get_object('lmdb', '_AdaptiveObject_', humanId, { objectId: true }),
        "error"
    ) == "error",
    "get_object returned an object after it was deleted!"
);

// the human id should be free again after delete (alias was cleaned up)
result = add_object('lmdb', '_AdaptiveObject_', { data: "reborn" }, humanId);
assert((result.objectId == humanId), "objectId was not preserved when re-adding a previously deleted human id");
object = get_object('lmdb', '_AdaptiveObject_', humanId);
assert(object.data == "reborn", "Data doesn't match 'reborn' after re-adding a previously deleted human id");

delete_object('lmdb', '_AdaptiveObject_', humanId);

return 0;
