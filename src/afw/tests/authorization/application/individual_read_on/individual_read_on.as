#!/usr/bin/env -S afw --syntax test_script --conf ./afw.conf
//?
//? testScript: individual_read_on.as
//? customPurpose: Part of authorization tests
//? description: issue #90 - checkIndividualObjectReadAccess true; per-object read deny enforced
//? sourceType: script
//?
//? test: get_object_denied_on_read
//? description: With flag true, get_object runs read check and deny throws
//? expect: 0
//? source: ...

let caught = false;
try {
    get_object("data", "SampleType", "A");
} catch (e) {
    assert(e.id === "denied");
    caught = true;
}
assert(caught);
return 0;

//?
//? test: retrieve_objects_denied_on_read
//? description: With flag true, retrieve fails when first object hits read deny
//? expect: 0
//? source: ...

let caught = false;
try {
    retrieve_objects("data", "SampleType");
} catch (e) {
    assert(e.id === "denied");
    caught = true;
}
assert(caught);
return 0;
