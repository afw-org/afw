#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: object_type_storage_tests.as
//? customPurpose: Part of lmdb tests
//? description: Test lmdb adapter storing/retrieving _AdaptiveObjectType_ objects.
//? sourceType: script
//?
//? test: object_type_storage_tests
//? description: Script to test lmdb adapter's support for storing _AdaptiveObjectType_ object type definitions natively.
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw


let objectTypeObject: object;
let result: object;

// LMDB should natively advertise support for storing _AdaptiveObjectType_
// objects, with no conf flag required.
objectTypeObject = get_object('lmdb', '_AdaptiveObjectType_', '_AdaptiveObjectType_');
assert(objectTypeObject.allowAdd == true, "LMDB should allow adding _AdaptiveObjectType_ objects by default");
assert(objectTypeObject.allowChange == true, "LMDB should allow changing _AdaptiveObjectType_ objects by default");
assert(objectTypeObject.allowDelete == true, "LMDB should allow deleting _AdaptiveObjectType_ objects by default");

const typeObjectId: string = "TestObjectTypeLmdb1";

// Add a real, user-defined object type definition.
result = add_object('lmdb', '_AdaptiveObjectType_', {
    allowAdd: true,
    allowChange: true,
    allowDelete: true,
    allowEntity: true,
    propertyTypes: {
        TestString1: {
            dataType: "string"
        }
    }
}, typeObjectId);
assert(result.objectId == typeObjectId, "objectId was not preserved when adding an object type definition");

// Retrieve it back by object_id. This is the same get_object call the
// compiler uses (via afw_adapter_get_object_type) to resolve a type's real
// schema, so it must return the stored definition, not a placeholder.
objectTypeObject = get_object('lmdb', '_AdaptiveObjectType_', typeObjectId);
assert(objectTypeObject.propertyTypes.TestString1.dataType == "string",
    "stored object type definition was not read back correctly");

// It should also show up in a retrieve_objects scan.
let found: boolean = false;
const objectTypes: array = retrieve_objects('lmdb', '_AdaptiveObjectType_', undefined, { objectId: true });
for (const ot: object of objectTypes) {
    if (meta(ot).objectId == typeObjectId) {
        found = true;
    }
}
assert(found, "retrieve_objects did not report the newly stored object type");

// modify it
modify_object('lmdb', '_AdaptiveObjectType_', typeObjectId,
    [["set_property", "allowDelete", false]]);
objectTypeObject = get_object('lmdb', '_AdaptiveObjectType_', typeObjectId);
assert(objectTypeObject.allowDelete == false, "modify_object did not update the stored object type definition");

// put it back so delete_object below is exercising a normal case
modify_object('lmdb', '_AdaptiveObjectType_', typeObjectId,
    [["set_property", "allowDelete", true]]);

// delete it
delete_object('lmdb', '_AdaptiveObjectType_', typeObjectId);

// verify it's gone
assert(
    safe_evaluate(
        get_object('lmdb', '_AdaptiveObjectType_', typeObjectId),
        "error"
    ) == "error",
    "get_object returned an object type definition after it was deleted!"
);

return 0;
