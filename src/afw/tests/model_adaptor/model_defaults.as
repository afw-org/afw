#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: model_defaults.as
//? customPurpose: Part of model adaptor category tests
//? description: Test model model_default_ function
//? sourceType: script
//?
//? test: model_default_add_object_action
//? description: Test action object
//? skip: false
//? expect: 0
//? source: ...

let result: object;

const uuid: string = generate_uuid();

// MyObjectType5 calls the model_default_add_object_action() and does asserts
result = add_object(
    "model",
    "MyObjectType5",
    { "MyTestString1": "This is a test string for test1." },
    uuid
);

return 0;