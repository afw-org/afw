#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: onAddObject.as
//? customPurpose: Part of model tests
//? description: Test model onAddObject function
//? sourceType: script
//?
//? test: Test useDefaultProcessing
//? description: Test using current::useDefaultProcessing
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let result: object;

const uuid: string = generate_uuid();

// create a simple object, specifying an objectId.  MyObjectType2 uses defaultProcessing for onAddObject
result = add_object(
    "model",
    "MyObjectType2",
    { "MyTestString1": "This is a test string for test1." },
    uuid
);
assert((result.objectId == uuid), "objectId was not created with uuid properly");

// delete it to clean up
delete_object("model", "MyObjectType2", uuid);

return 0;

//?
//? test: Test overriding the objectId
//? description: Overrides the objectId with a custom value
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let result: object;

const uuid: string = generate_uuid();

// create a simple object, specifying an objectId.  MyObjectType3 overrides the objectId
result = add_object(
    "model",
    "MyObjectType3",
    { "MyTestString1": "This is a test string for test1." },
    uuid
);
assert((result.objectId !== uuid), "objectId was created with uuid");
assert((result.objectId === "TestObject3"), "objectId was not created with onAddObject");

// delete it to clean up
delete_object("model", "MyObjectType3", "TestObject3");

return 0;