#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/string_to_rfc822Name.as
//? customPurpose: Part of core function tests
//? description: Tests for to_rfc822Name<string> function.
//? sourceType: expression
//?
//? test: to_rfc822Name<string>-1
//? description: x
//? expect: rfc822Name("localPart@DOMAINPart")
//? source: ...

to_rfc822Name<string>(
    string("localPart@DOMAINPart")
)

//?
//? test: to_rfc822Name<string>-2
//? description: x
//? expect: rfc822Name("localPart@DOMAINPart.com")
//? source: ...

to_rfc822Name<string>(
    string("localPart@DOMAINPart.com")
)

//?
//? test: to_rfc822Name<string>-3
//? skip: true
//? description: Invalid arguments test.
//? expect: error
//? source: ...

to_rfc822Name<string>(
    string("x")
)

