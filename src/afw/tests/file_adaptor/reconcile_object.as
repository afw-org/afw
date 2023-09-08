#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: reconcile_object.as
//? customPurpose: Part of file adaptor tests
//? description: Test file adaptor reconcile_object.
//? sourceType: script
//?
//? test: reconcile_object_test-1
//? description: Test file adaptor reconcile_object.
//? skip: false
//? expect: 0
//? source: ...

let obj: object;
let obj2: object;
let result: object;

const uuid: string = generate_uuid();

// create a simple object, specifying an objectId
result = add_object(
    "file",
    "TestObjectType1",
    { "TestString1": "This is a test string for test1." },
    uuid
);
assert((result.objectId == uuid), "objectId was not created with uuid properly");

// get it in reconcilable mode
obj = get_object(
    "file",
    "TestObjectType1",
    uuid,
    { reconcilable: true }
);

// now reconcile it (test mode), with no changes
result = reconcile_object(
    obj, true
);
assert(result->bag_size() === 0, "reconcile_object with no changes failed!");

// now change the property
obj2 = clone(obj);
obj2.TestString1 = "A new value";

// now reconcile it (test mode), with no changes
result = reconcile_object(
    obj2, false
);
assert(result->bag_size() !== 0, "reconcile_object with changes failed!");


// now get it back
obj = get_object(
    "file",
    "TestObjectType1",
    uuid
);
assert(obj.TestString1 == "A new value", "reconcile_object failed");

// delete it
delete_object("file", "TestObjectType1", uuid);

return 0;