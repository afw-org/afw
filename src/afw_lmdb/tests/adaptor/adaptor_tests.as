#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: adaptor_tests.as
//? customPurpose: Part of lmdb tests
//? description: Test lmdb adaptor methods.
//? sourceType: script
//?
//? test: adaptor_tests
//? description: Script to test lmdb adaptor.
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw


loc object: object;
loc result: object;
loc objectId: string;

// Generate a uuid
const uuid: string = generate_uuid();

// create an object, specifying the objectId (uuid)
result = add_object('lmdb','_AdaptiveObject_', {}, uuid);
assert((result.objectId == uuid), "objectId was not created with uuid properly");

// create an object without specifying the objectId (uuid)
result = add_object('lmdb','_AdaptiveObject_', {});
assert((is_defined(result.objectId)), "objectId was not created without uuid properly");
objectId = result.objectId;

// now get the objects we created with uuid
object = get_object('lmdb', '_AdaptiveObject_', uuid, { objectId: true });
assert((meta(object).objectId == uuid), "objectId from get_object does not match uuid");

// now get the objects we created with result
object = get_object('lmdb', '_AdaptiveObject_', result.objectId, { objectId: true });
assert((meta(object).objectId == result.objectId), "objectId from get_object does not match result.objectId");

// now delete the first object
result = delete_object('lmdb', '_AdaptiveObject_', uuid);

// verify it's gone
assert(
    safe_evaluate(
        get_object('lmdb', '_AdaptiveObject_', uuid, { objectId: true }), 
        "error"
    ) == "error", 
    "get_object returned an object after it was deleted!"
);


// replace the second object
replace_object('lmdb', '_AdaptiveObject_', objectId, { data: "Hello world!" });

// make sure it now contains Hello world!
object = get_object('lmdb', '_AdaptiveObject_', objectId);
assert(object.data == "Hello world!", "Data doesn't match 'Hello world!'");

// test modify_object()
modify_object('lmdb', '_AdaptiveObject_', objectId,
    [["set_property", "data", "This is a new value!"]]);

object = get_object('lmdb', '_AdaptiveObject_', objectId);
assert(object.data == "This is a new value!", "Data doesn't match 'This is a new value!'");

// delete our second object
delete_object('lmdb', '_AdaptiveObject_', objectId);

// verify it's gone
assert(
    safe_evaluate(
        get_object('lmdb', '_AdaptiveObject_', objectId, { objectId: true }),
        "error"
    ) == "error",
    "get_object returned an object after it was deleted!"
);

// test retrieve_objects
const objects: list = retrieve_objects('lmdb', '_AdaptiveObject_');
//print(bag_size(objects));

return 0;
