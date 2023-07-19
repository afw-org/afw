#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: onGetInitialObjectId.as
//? customPurpose: Part of model tests
//? description: Test model onGetInitialObjectId function
//? sourceType: script
//?
//? test: Test useDefaultProcessing
//? description: Test using current::useDefaultProcessing
//? expect: 0
//? source: ...
#!/usr/bin/env afw

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