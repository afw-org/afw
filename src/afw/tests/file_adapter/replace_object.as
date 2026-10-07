#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: replace_object.as
//? customPurpose: Part of file adapter tests
//? description: Test file adapter replace_object.
//? sourceType: script
//?
//? test: replace_object_test-1
//? description: Test file adapter replace_object.
//? skip: false
//? expect: 0
//? source: ...

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


// now replace it
result = replace_object(
    "file",
    "TestObjectType1",
    uuid,
    { "TestString1": "A new value" },    
);


// now get it back
obj = get_object(
    "file",
    "TestObjectType1",
    uuid
);
assert(obj.TestString1 == "A new value", "replace_object failed");


// delete it
delete_object("file", "TestObjectType1", uuid);

return 0;
//?
//? test: replace_object-missing-not-found
//? description: replace_object of an object that does not exist is not_found and does not create it.
//? expect: "not_found not_found"
//? source: ...

let ids: array = [];
try {
    replace_object("file", "TestObjectType1", "NoSuchObject", {a: 1});
}
catch (e) {
    push(ids, e.id);
}
try {
    get_object("file", "TestObjectType1", "NoSuchObject");
}
catch (e) {
    push(ids, e.id);
}
return join(ids, " ");
