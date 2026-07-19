#!/usr/bin/env -S afw --syntax test_script --conf ./afw.conf
//?
//? testScript: individual_read_property.as
//? customPurpose: Part of authorization tests
//? description: issue #90 - per-object read uses object body (secret property)
//? sourceType: script
//?
//? test: get_object_A_denied_by_secret
//? description: Object A has secret s1; read policy denies it
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
//? test: get_object_B_permitted
//? description: Object B has secret s2; read policy permits it
//? expect: 0
//? source: ...

let obj = get_object("data", "SampleType", "B");
assert(obj.name === "B");
assert(obj.secret === "s2");
return 0;

//?
//? test: retrieve_fails_when_denied_object_encountered
//? description: retrieve fails (enforced deny) when it hits object A with secret s1
//? expect: 0
//? source: ...

/*
 * Per-object read deny is enforced via afw_authorization_check(true, ...), so
 * a deny throws rather than soft-filtering that object out of the list.
 */
let caught = false;
try {
    retrieve_objects("data", "SampleType");
} catch (e) {
    assert(e.id === "denied");
    caught = true;
}
assert(caught);
return 0;
