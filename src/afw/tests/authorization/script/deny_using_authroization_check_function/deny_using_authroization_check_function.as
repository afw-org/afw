#!/usr/bin/env -S afw --syntax test_script --conf ./afw.conf
//?
//? testScript: deny_using_authroization_check_function.as
//? customPurpose: Part of authorization tests
//? description: Test deny decisions using authorization_check()
//? sourceType: script
//?
//? test: get_object
//? description: Test authorization_check is denied
//? expect: "maluba"
//? source: ...
#!/usr/bin/env afw

try {
    const result = authorization_check(
        'aRequestId',                   // requestId
        '/afw/_AdaptiveAdaptor_/afw',   // resourceId
        'maluba',                       // actionId
        ,                               // object
        true);                          // enforce
}
catch (e) {
    return e.data.actionId;
}
