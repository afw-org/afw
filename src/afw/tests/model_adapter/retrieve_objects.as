#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: retrieve_objects.as
//? customPurpose: Part of model adapter function tests
//? description: Test model adapter retrieve_objects.
//? sourceType: script
//?
//? test: model_test-1
//? description: Test retrieve_objects.
//? skip: false
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("model", "MyObjectType1");

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.MyTestString1 === "This is a test string.", "MyTestString1 was " + obj.MyTestString1);

return 0;

//?
//? test: retrieve-select
//? description: retrieve with a non-empty select list against a model adapter
//? skip: false
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("model", "MyObjectType1", {
    select: ["MyTestString1"]
});
const obj: object = objects[0];
assert(obj !== undefined);
assert(obj.MyTestString1 === "This is a test string.",
    "MyTestString1 was " + obj.MyTestString1);

return 0;
