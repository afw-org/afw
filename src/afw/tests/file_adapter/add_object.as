#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: add_object.as
//? customPurpose: Part of file adapter tests
//? description: Test file adapter add_object.
//? sourceType: script
//?
//? test: add_object_test-1
//? description: Test file adapter add_object.
//? skip: false
//? expect: 0
//? source: ...

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

// delete it to clean up
delete_object("file", "TestObjectType1", uuid);

// create an object without an objectId
result = add_object(
    "file",
    "TestObjectType1",
    { "TestString1": "This is a test string for test1." },    
);
assert((is_defined(result.objectId)), "objectId was not created without uuid properly");

// delete it to clean up
delete_object("file", "TestObjectType1", result.objectId);

return 0;

//?
//? test: add_object_already_exists
//? description: add_object of an existing id is conflict
//? expect: 0
//? source: ...

const uuid: string = generate_uuid();
add_object(
    "file",
    "TestObjectType1",
    { "TestString1": "first" },
    uuid
);
let id: string = "";
try {
    add_object(
        "file",
        "TestObjectType1",
        { "TestString1": "second" },
        uuid
    );
}
catch (e) {
    id = e.id;
}
delete_object("file", "TestObjectType1", uuid);
assert(id === "conflict", id);
return 0;