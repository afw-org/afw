#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: error_use_default_processing.as
//? customPurpose: Part of model script-only tests
//? description: issue #109 useDefaultProcessing without mappedAdapterId errors
//? sourceType: script
//?
//? test: get_use_default_processing
//? description: issue #109 onGetObject returns useDefaultProcessing with no mappedAdapterId
//? expect: error
//? source: ...

get_object("scriptModel", "UseDefault", "any");


//? test: retrieve_use_default_processing
//? description: issue #109 onRetrieveObjects returns useDefaultProcessing with no mappedAdapterId
//? expect: error
//? source: ...

retrieve_objects("scriptModel", "UseDefault");
