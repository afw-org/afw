#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: onDeleteObject.as
//? customPurpose: Part of model tests
//? description: Test model onDeleteObject function
//? sourceType: script
//?
//? test: Test useDefaultProcessing
//? description: Test using current::useDefaultProcessing
//? expect: 0
//? source: ...
#!/usr/bin/env afw

loc result: object;

const uuid: string = generate_uuid();

// create a simple object
result = add_object(
    "model",
    "MyObjectType1",
    { "MyTestString1": "This is a test string for test1." },
    uuid
);
assert((result.objectId == uuid), "objectId was not created with uuid properly");

// delete it
delete_object("model", "MyObjectType1", uuid);

// make sure it was removed
assert(
    safe_evaluate(
        get_object('model', 'MyObjectType1', uuid), 
        "error"
    ) == "error", 
    "get_object returned an object after it was deleted!"
);

return 0;