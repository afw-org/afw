#!/usr/bin/env -S afw --syntax test_script --conf ./afw.conf
//?
//? testScript: deny_with_e.as
//? customPurpose: Part of authorization tests
//? description: Test deny decisions with e
//? sourceType: script
//?
//? test: get_object
//? description: Test get_object is denied
//? expect: "query"
//? source: ...

try {
    const o: object = get_object("afw", "_AdaptiveAdapter_", "afw");
}
catch (e) {
    return e.data.actionId;
}
