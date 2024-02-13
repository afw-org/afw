#!/usr/bin/env -S afw --syntax test_script --conf ./afw.conf
//?
//? testScript: recurse.as
//? customPurpose: Part of authorization tests
//? description: Test authorization script that may cause recursive authorization calls.
//? sourceType: script
//?
//? test: get_object
//? description: Test get_object succeeds
//? expect: 0
//? source: ...

const o: object = get_object("afw", "_AdaptiveAdaptor_", "afw");

return 0;