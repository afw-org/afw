#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: modify_object.as
//? customPurpose: Part of model adaptor tests
//? description: Test model adaptor modify_object.
//? sourceType: script
//?
//? test: modify_object_test-1
//? description: Test model adaptor modify_object.
//? skip: false
//? expect: 0
//? source: ...

let obj: object;
let result: object;

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