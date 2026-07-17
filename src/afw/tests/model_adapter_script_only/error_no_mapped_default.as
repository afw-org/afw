#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: error_no_mapped_default.as
//? customPurpose: Part of model script-only tests
//? description: issue #109 default processing without mappedAdapterId errors
//? sourceType: script
//?
//? test: get_missing_hook
//? description: issue #109 get_object with no onGetObject and no mappedAdapterId
//? expect: error
//? source: ...

get_object("scriptModel", "MissingHook", "any");


//? test: retrieve_missing_hook
//? description: issue #109 retrieve_objects with no onRetrieveObjects and no mappedAdapterId
//? expect: error
//? source: ...

retrieve_objects("scriptModel", "MissingHook");


//? test: add_missing_hook
//? description: issue #109 add_object with no onAddObject and no mappedAdapterId
//? expect: error
//? source: ...

add_object("scriptModel", "MissingHook", { "x": true }, "id1");
