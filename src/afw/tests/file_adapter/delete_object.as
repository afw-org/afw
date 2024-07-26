#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: delete_object.as
//? customPurpose: Part of file adapter tests
//? description: Test file adapter delete_object.
//? sourceType: script
//?
//? test: delete_object_test-1
//? description: Test file adapter delete_object.
//? skip: false
//? expect: 0
//? source: ...

let result: object;

const uuid: string = generate_uuid();

// create a simple object
result = add_object(
    "file",
    "TestObjectType1",
    { "TestString1": "This is a test string for test1." },
    uuid
);
assert((result.objectId == uuid), "objectId was not created with uuid properly");

// delete it
delete_object("file", "TestObjectType1", uuid);

// make sure it was removed
assert(
    safe_evaluate(
        get_object('file', 'TestObjectType1', uuid), 
        "error"
    ) == "error", 
    "get_object returned an object after it was deleted!"
);

return 0;