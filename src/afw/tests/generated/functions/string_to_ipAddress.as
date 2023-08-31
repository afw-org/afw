#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/string_to_ipAddress.as
//? customPurpose: Part of core function tests
//? description: Tests for to_ipAddress<string> function.
//? sourceType: expression
//?
//? test: to_ipAddress<string>-1
//? description: x
//? expect: ipAddress("204.11.3.224")
//? source: ...

to_ipAddress<string>(
    string("204.11.3.224")
)

//?
//? test: to_ipAddress<string>-2
//? description: x
//? expect: ipAddress("204.0.0.224")
//? source: ...

to_ipAddress<string>(
    string("204.0.0.224")
)

//?
//? test: to_ipAddress<string>-3
//? skip: true
//? description: Invalid arguments test.
//? expect: error
//? source: ...

to_ipAddress<string>(
    string("x")
)

