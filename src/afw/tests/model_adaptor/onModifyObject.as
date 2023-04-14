#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: onModifyObject.as
//? customPurpose: Part of model tests
//? description: Test model onModifyObject function
//? sourceType: script
//?
//? test: Test useDefaultProcessing
//? description: Test using current::useDefaultProcessing
//? expect: 0
//? source: ...
#!/usr/bin/env afw

loc obj: object;
loc result: object;

const uuid: string = generate_uuid();

// create a simple object, specifying an objectId
result = add_object(
    "model",
    "MyObjectType1",
    { "MyTestString1": "This is a test string for test1." },
    uuid
);
assert((result.objectId == uuid), "objectId was not created with uuid properly");

// now modify it (set_property)
result = modify_object(
    "model",
    "MyObjectType1",
    uuid,
    [
        [
            "set_property",
            "MyTestString1",
            "A new value"
        ]
    ]
);

// now get it back
obj = get_object(
    "model",
    "MyObjectType1",
    uuid
);
assert(obj.MyTestString1 == "A new value", "Modify object failed");

// delete it
delete_object("model", "MyObjectType1", uuid);

return 0;