#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: modify_object.as
//? customPurpose: Part of file adapter tests
//? description: Test file adapter modify_object.
//? sourceType: script
//?
//? test: modify_object_test-1
//? description: Test file adapter modify_object.
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
//?
//? test: modify_object_test-empty-file
//? description: modify_object of an empty object file is not_found, not a crash.
//? skip: false
//? expect: "not_found"
//? source: ...

let err: string = "none";
try {
    modify_object("file", "EmptyFileType", "Empty",
        [["set_property", "a", 1]]);
}
catch (e) {
    err = e.id;
}
return err;

//?
//? test: modify_object_bad_property_name
//? description: A modify entry whose property name is not a string or an array of strings is invalid (used to dereference NULL).
//? expect: 0
//? source: ...

const id: string = generate_uuid();
add_object("file", "TestObjectType1", { "s": 1 }, id);
const bad: array = [
    ["remove_property", 1],
    ["remove_property", true],
    ["remove_property", null],
    ["remove_property", {}],
    ["set_property", 1, 2],
    ["add_value", 1, 2],
    ["remove_value", 1, 2]
];
for (const e of bad) {
    let message: any = "no error";
    try {
        modify_object("file", "TestObjectType1", id, [e]);
    }
    catch (err) {
        message = err.message;
    }
    assert(includes<string>(message, "is invalid"), stringify(e) + ": " + message);
}
assert(get_object("file", "TestObjectType1", id).s === 1);
delete_object("file", "TestObjectType1", id);
return 0;

//?
//? test: modify_object_array_values
//? description: add_value and remove_value on a stored array property change a copy (the array read from the file is immutable; this used to fail with "List immutable").
//? expect: 0
//? source: ...

const id: string = generate_uuid();
add_object("file", "TestObjectType1", { "a": [1, 2], "s": "x" }, id);
modify_object("file", "TestObjectType1", id, [["add_value", "a", 3]]);
assert(stringify(get_object("file", "TestObjectType1", id).a) === "[1,2,3]",
    "add_value: " + stringify(get_object("file", "TestObjectType1", id).a));
modify_object("file", "TestObjectType1", id, [["remove_value", "a", 1]]);
assert(stringify(get_object("file", "TestObjectType1", id).a) === "[2,3]",
    "remove_value: " + stringify(get_object("file", "TestObjectType1", id).a));
modify_object("file", "TestObjectType1", id, [["add_value", "s", "y"]]);
assert(stringify(get_object("file", "TestObjectType1", id).s) === "[\"x\",\"y\"]",
    "add_value to single: " + stringify(get_object("file", "TestObjectType1", id).s));
delete_object("file", "TestObjectType1", id);
return 0;
