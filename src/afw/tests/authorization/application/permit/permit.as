#!/usr/bin/env -S afw --syntax test_script --conf ./afw.conf
//?
//? testScript: permit.as
//? customPurpose: Part of authorization tests
//? description: Test permit decisions using application authorizationControl
//? sourceType: script
//?
//? test: get_object
//? description: Test get_object succeeds
//? expect: 0
//? source: ...

const o: object = get_object("afw", "_AdaptiveAdapter_", "afw");

return 0;