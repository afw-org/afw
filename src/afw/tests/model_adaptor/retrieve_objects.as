#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: retrieve_objects.as
//? customPurpose: Part of model adaptor function tests
//? description: Test model adaptor retrieve_objects.
//? sourceType: script
//?
//? test: model_test-1
//? description: Test retrieve_objects.
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const objects: array = retrieve_objects("model", "MyObjectType1");

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.MyTestString1 === "This is a test string.", "MyTestString1 was " + obj.MyTestString1);

return 0;
