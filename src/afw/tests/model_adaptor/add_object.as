#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: add_object.as
//? customPurpose: Part of model adaptor tests
//? description: Test model adaptor add_object.
//? sourceType: script
//?
//? test: add_object_test-1
//? description: Test model adaptor add_object.
//? skip: false
//? expect: 0
//? source: ...

let result: object;

const uuid: string = generate_uuid();

// create a simple object, specifying an objectId
result = add_object(
    "model",
    "MyObjectType1",
    { "TestString1": "This is a test string for test1." },
    uuid
);
assert((result.objectId == uuid), "objectId was not created with uuid properly");

// delete it to clean up
delete_object("model", "MyObjectType1", uuid);

return 0;