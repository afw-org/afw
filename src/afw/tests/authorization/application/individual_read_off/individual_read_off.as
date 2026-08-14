#!/usr/bin/env -S afw --syntax test_script --conf ./afw.conf
//?
//? testScript: individual_read_off.as
//? customPurpose: Part of authorization tests
//? description: issue #90 - checkIndividualObjectReadAccess false; read deny must not apply
//? sourceType: script
//?
//? test: get_object_succeeds_despite_read_deny
//? description: With flag false, get_object only does query; read deny policy is not run
//? expect: 0
//? source: ...

let obj = get_object("data", "SampleType", "A");
assert(obj !== undefined);
assert(obj.name === "A");
return 0;

//?
//? test: retrieve_objects_succeeds_despite_read_deny
//? description: With flag false, retrieve only does query; objects returned even if read would deny
//? expect: 0
//? source: ...

let list = retrieve_objects("data", "SampleType");
assert(length(list) === 2);
return 0;

//?
//? test: get_object_B
//? description: Second sample object also readable when flag is false
//? expect: 0
//? source: ...

let obj = get_object("data", "SampleType", "B");
assert(obj.name === "B");
return 0;
