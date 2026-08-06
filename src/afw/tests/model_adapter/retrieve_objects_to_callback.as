#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: retrieve_objects_to_callback.as
//? customPurpose: Part of model adapter function tests
//? description: Test model adapter retrieve_objects_to_callback.
//? sourceType: script
//?
//? test: model_test-1
//? description: Test retrieve_objects_to_callback.
//? skip: false
//? expect: 0
//? source: ...

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

//?
//? test: retrieve_callback-mutable-face
//? description: ...
Issue #17: object passed to retrieve_objects_to_callback is a face; mutate
then retrieve again — next callback object must not keep face-only props.
//? skip: false
//? expect: 0
//? source: ...

let firstObj: object;
let secondObj: object;
let n: integer = 0;

function cb1(theObj: object): boolean {
    firstObj = theObj;
    firstObj.cbMarker = "from_cb";
    firstObj.MyTestString1 = "mutated";
    return false;
}

retrieve_objects_to_callback(cb1, null, "model", "MyObjectType1");
assert(firstObj !== undefined);
assert(firstObj.cbMarker === "from_cb");

function cb2(theObj: object): boolean {
    secondObj = theObj;
    n = n + 1;
    return false;
}

retrieve_objects_to_callback(cb2, null, "model", "MyObjectType1");
assert(n === 1);
assert(secondObj.MyTestString1 === "This is a test string.",
    "callback face mutation must not poison next retrieve callback");
assert(is_nullish(property_get(secondObj, "cbMarker", null)),
    "cbMarker must not appear on second callback object");

return 0;
