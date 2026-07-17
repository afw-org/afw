#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: get_retrieve.as
//? customPurpose: Part of model script-only tests
//? description: issue #109 pure-script get/retrieve without mappedAdapterId
//? sourceType: script
//?
//? test: get_object_synthetic
//? description: issue #109 get_object via onGetObject with no mappedAdapterId
//? expect: 0
//? source: ...

const obj = get_object("scriptModel", "SyntheticRead", "alpha");
assert(obj !== undefined);
assert(obj.x === true);
assert(obj.name === "alpha");

return 0;


//? test: retrieve_objects_synthetic
//? description: issue #109 retrieve_objects via onRetrieveObjects with no mappedAdapterId
//? expect: 0
//? source: ...

const objects = retrieve_objects("scriptModel", "SyntheticRead");
assert(length(objects) == 2);
assert(objects[0].name === "one");
assert(objects[1].name === "two");

return 0;


//? test: object_types_from_model
//? description: issue #109 _AdaptiveObjectType_ still served from model without mapped
//? expect: 0
//? source: ...

const ot = get_object("scriptModel", "_AdaptiveObjectType_", "SyntheticRead");
assert(ot !== undefined);

return 0;
