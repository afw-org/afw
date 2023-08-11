#!/usr/bin/env -S afw --syntax test_script --conf ./afw.conf
//?
//? testScript: authorization_check_function.as
//? customPurpose: Part of authorization tests
//? description: ...
Test authorization_check built-in function. The associated afw.conf has a
authorizationCheck that only makes a 'permit' decision on resourceId
'/afw/_AdaptiveAdaptor_/afw'.
//? sourceType: script
//?
//? test: authorization_check_permit
//? description: Test authorization_check decisionId permit.
//? expect: "permit"
//? source: ...
#!/usr/bin/env afw

let result;

try {
    result = authorization_check(
        'aRequestId',                   // requestId
        '/afw/_AdaptiveAdaptor_/afw',   // resourceId
        'maluba',                       // actionId
        ,                               // object
        true);                          // enforce
}
catch (e) {
    return e.data.actionId;
}

return result.decisionId;

//?
//? test: authorization_check_deny_enforced
//? description: Test authorization_check decisionId deny enforced
//? expect: "maluba"
//? source: ...
#!/usr/bin/env afw

let result;

try {
    result = authorization_check(
        'aRequestId',                      // requestId
        '/afw/_AdaptiveAdaptor_/secret',   // resourceId
        'maluba',                          // actionId
        ,                                  // object
        true);                             // enforce
}
catch (e) {
    return e.data.actionId;
}

return result.decisionId;


//?
//? test: authorization_check_deny_enforced_false
//? description: Test authorization_check decisionId deny enforced false
//? expect: "deny"
//? source: ...
#!/usr/bin/env afw

let result;

try {
    result = authorization_check(
        'aRequestId',                      // requestId
        '/afw/_AdaptiveAdaptor_/secret',   // resourceId
        'maluba',                          // actionId
        ,                                  // object
        false);                            // enforce
}
catch (e) {
    return e.data.actionId;
}

return result.decisionId;



//?
//? test: authorization_check_deny_enforced_missing
//? description: Test authorization_check decisionId deny enforced missing
//? expect: "deny"
//? source: ...
#!/usr/bin/env afw

let result;

try {
    result = authorization_check(
        'aRequestId',                      // requestId
        '/afw/_AdaptiveAdaptor_/secret',   // resourceId
        'maluba');                         // actionId
}
catch (e) {
    return e.data.actionId;
}

return result.decisionId;

