#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: retrieve_objects_to_callback.as
//? customPurpose: Part of model adaptor function tests
//? description: Test model adaptor retrieve_objects_to_callback.
//? sourceType: script
//?
//? test: model_test-1
//? description: Test retrieve_objects_to_callback.
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let count: integer = 0;
let obj: object;

function cb (theObj: object): boolean
{
    count = count + 1;
    obj = theObj;

    return false;
}

retrieve_objects_to_callback(
    cb,
    null,
    "model", 
    "MyObjectType1"
);

assert(count === 1, "count was " + string(count));

assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.MyTestString1 === "This is a test string.");

return 0;
