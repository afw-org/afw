#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: get_object.as
//? customPurpose: Part of file adapter tests
//? description: Test file adapter get_object
//? sourceType: script
//?
//? test: file_get_object-1
//? description: Test file adapter get_object.
//? skip: false
//? expect: 0
//? source: ...

let obj: object = get_object("file", "TestObjectType1", "Test1");

// we should have one object to start with
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.TestString1 === "This is a test string.");

return 0;

//?
//? test: get_object_test-2
//? description: Test file adapter get_object with object that does not exist
//? skip: false
//? expect: 0
//? source: ...

// get an object that does not exist
assert(
    safe_evaluate(
        get_object("file", "TestObjectType1", "Test2"),
        "error"
    ) == "error",
    "get_object returned an object that does not exist!"
);

return 0;

//?
//? test: get_object_test-3
//? description: Test file adapter get_object with object options
//? skip: false
//? expect: 0
//? source: ...

let obj: object;

// use object options
obj = get_object("file", "TestObjectType1", "Test1", 
    { 
        "path": true, 
        "objectId": true,
        "objectType": true,
        "reconcilable": true,        
    }
);

//println(meta(meta(obj).objectId));
assert(meta(obj).path === anyURI("/file/TestObjectType1/Test1"));
assert(meta(obj).objectId === "Test1");
assert(meta(obj).objectType === "TestObjectType1");
assert(meta(obj).reconcilable !== undefined);

return 0;
