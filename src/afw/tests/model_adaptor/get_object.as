#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: get_object.as
//? customPurpose: Part of model adaptor function tests
//? description: Test model adaptor get_object.
//? sourceType: script
//?
//? test: model_get_object-1
//? description: Test get_object with an object that exists.
//? skip: false
//? expect: 0
//? source: ...

// get the mapped object
let obj: object = get_object("model", "MyObjectType1", "Test1");

// we should have one object to start with
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.MyTestString1 === "This is a test string.");

return 0;

//?
//? test: model_get_object-2
//? description: Test get_object with an object that does not exist.
//? skip: false
//? expect: 0
//? source: ...

// get an object that does not exist
assert(
    safe_evaluate(
        get_object("model", "MyObjectType1", "Test2"),
        "error"
    ) == "error",
    "get_object returned an object that does not exist!"
);

return 0;

//?
//? test: model_get_object-3
//? description: Test get_object with a object options.
//? skip: false
//? expect: 0
//? source: ...

let obj: object;

obj = get_object("model", "MyObjectType1", "Test1", 
    { 
        "path": true, 
        "objectId": true,
        "objectType": true,
        "reconcilable": true,        
    }
);

assert(meta(obj).path === anyURI("/model/MyObjectType1/Test1"));
assert(meta(obj).objectId === "Test1");
assert(meta(obj).objectType === "MyObjectType1");
assert(meta(obj).reconcilable === "{\"_meta_\":{\"path\":\"/model/MyObjectType1/Test1\"},\"MyTestString1\":\"This is a test string.\"}");

return 0;