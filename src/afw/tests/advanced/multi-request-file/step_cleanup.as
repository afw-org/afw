#!/usr/bin/env afw --syntax test_script
//?
//? testScript: step_cleanup.as
//? description: Delete Demo/alpha left by prior multi-request steps
//? sourceType: script
//?
//? test: delete_alpha
//? description: remove the multi-request demo object
//? expect: null
//? source: ...

delete_object("file", "Demo", "alpha");
return null;
