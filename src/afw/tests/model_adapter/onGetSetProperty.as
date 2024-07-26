#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: onGetSetProperty.as
//? customPurpose: Part of model adapter tests
//? description: Test model adapter onGetProperty and onSetProperty.
//? sourceType: script
//?
//? test: onGetSetProperty-1
//? description: Test model adapter onSetProperty.
//? skip: false
//? expect: 0
//? source: ...

let result: object;

const uuid: string = generate_uuid();

// create a simple object
result = add_object(
    "model",
    "MyObjectType6",
    { "TestOnPropertyList": "x" },
    uuid
);
assert((result.objectId == uuid), "objectId was not created with uuid properly");

// now get it back
let obj = get_object("model", "MyObjectType6", uuid);
let x = obj.TestOnPropertyList;
assert((length(x) === 1), "Expected x to have length of 1, instead length = " + string(length(x)));
assert((x[0] === "x"), "Expected 'x', got " + string(x));

// read it back a second time
obj = get_object("model", "MyObjectType6", uuid);
x = obj.TestOnPropertyList;
assert((length(x) === 1), "Expected x to have length of 1, instead length = " + string(length(x)));
assert((x[0] === "x"), "Expected 'x', got " + string(x));

return 0;