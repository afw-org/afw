#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: modify_object.as
//? customPurpose: Part of file adaptor tests
//? description: Test file adaptor modify_object.
//? sourceType: script
//?
//? test: modify_object_test-1
//? description: Test file adaptor modify_object.
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let obj: object;
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

// now modify it (set_property)
result = modify_object(
    "file",
    "TestObjectType1",
    uuid,
    [
        [
            "set_property",
            "TestString1",
            "A new value"
        ]
    ]
);

// now get it back
obj = get_object(
    "file",
    "TestObjectType1",
    uuid
);
assert(obj.TestString1 == "A new value", "Modify object failed");

// delete it
delete_object("file", "TestObjectType1", uuid);

return 0;