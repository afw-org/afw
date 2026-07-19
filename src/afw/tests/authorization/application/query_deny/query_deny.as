#!/usr/bin/env -S afw --syntax test_script --conf ./afw.conf
//?
//? testScript: query_deny.as
//? customPurpose: Part of authorization tests
//? description: issue #90 - collection-level query deny before objects are retrieved
//? sourceType: script
//?
//? test: retrieve_query_denied
//? description: query deny blocks retrieve
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

//?
//? test: get_query_denied
//? description: query deny blocks get_object
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
