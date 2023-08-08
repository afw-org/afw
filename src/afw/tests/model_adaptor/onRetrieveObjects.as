#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: onRetrieveObjects.as
//? customPurpose: Part of model tests
//? description: Test model onRetrieveObjects function
//? sourceType: script
//?
//? test: Test useDefaultProcessing
//? description: Test using current::useDefaultProcessing
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const objects: array = retrieve_objects("model", "MyObjectType2");

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.MyTestString1 === "This is a test string.");

return 0;

//? test: Test Change Object Type
//? description: Test onRetrieveObject by overriding the object type
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const objects: array = retrieve_objects("model", "MyObjectType3");

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.MyTestString1 === "This is a test string.");

return 0;

//? test: Test Change Query Criteria
//? description: Test onRetrieveObject by overriding the queryCriteria
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const objects: array = retrieve_objects("model", "MyObjectType4");

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.MyTestString1 === "This is a test string.");

return 0;