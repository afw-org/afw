#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: onGetObject.as
//? customPurpose: Part of model tests
//? description: Test model onGetObject function
//? sourceType: script
//?
//? test: Test useDefaultProcessing
//? description: Test using current::useDefaultProcessing
//? expect: 0
//? source: ...

let obj: object = get_object("model", "MyObjectType1", "Test1");

// we should have one object to start with
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.MyTestString1 === "This is a test string.");

return 0;


//? test: Test onGetObject Qualified Variables
//? description: Test onGetObject qualified variables
//? expect: 0
//? source: ...

const obj = get_object(
    "model", 
    "TestQualifiedVariables",
    "abc",
    { "objectId": true },
    { "func": "get_object" }
);

assert(obj.x === true);

return 0;
