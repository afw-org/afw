#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: model_location_native_tests.as
//? customPurpose: Part of lmdb tests
//? description: Test that an lmdb adapter configured with isModelLocation can host _AdaptiveModel_ objects directly, without a separate file adapter.
//? sourceType: script
//?
//? test: model_location_native_tests
//? description: Script to test lmdb adapter's support for isModelLocation.
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw


let modelTypeObject: object;
let result: object;

// isModelLocation should have registered _AdaptiveModel_ as read/write
// directly on the lmdb adapter.
modelTypeObject = get_object('lmdb', '_AdaptiveObjectType_', '_AdaptiveModel_');
assert(modelTypeObject.allowAdd == true, "isModelLocation should allow adding _AdaptiveModel_ objects on lmdb");
assert(modelTypeObject.allowChange == true, "isModelLocation should allow changing _AdaptiveModel_ objects on lmdb");

const modelId: string = "TestModelLmdbNative";

// Add a model definition directly to the lmdb adapter.
result = add_object('lmdb', '_AdaptiveModel_', {
    modelId: modelId,
    objectTypes: {
        MyObjectType1: {
            mappedObjectType: "TestObjectType1",
            propertyTypes: {
                MyTestString1: {
                    dataType: "string",
                    mappedPropertyName: "TestString1"
                }
            }
        }
    }
}, modelId);
assert(result.objectId == modelId, "objectId was not preserved when adding a model to an lmdb model location");

// Read it back.
const model: object = get_object('lmdb', '_AdaptiveModel_', modelId);
assert(model.objectTypes.MyObjectType1.mappedObjectType == "TestObjectType1",
    "stored model definition was not read back correctly");

delete_object('lmdb', '_AdaptiveModel_', modelId);

return 0;
