#!/usr/bin/env -S afw --syntax test_script --conf ./afw.conf
//?
//? testScript: permit.as
//? customPurpose: Part of authorization tests
//? description: Test permit decisions using application authorizationControl and models
//? sourceType: script
//?
//? test: get_object
//? description: Test get_object succeeds
//? expect: 0
//? source: ...
#!/usr/bin/env afw

// get the mapped object
loc obj = get_object("model", "MyObjectType1", "Test1");

return 0;