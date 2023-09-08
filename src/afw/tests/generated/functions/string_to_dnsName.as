#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/string_to_dnsName.as
//? customPurpose: Part of core function tests
//? description: Tests for to_dnsName<string> function.
//? sourceType: script
//?
//? test: to_dnsName<string>-1
//? description: x
//? expect: dnsName("clemson:8080")
//? source: ...

to_dnsName<string>(
    string("clemson:8080")
)

//?
//? test: to_dnsName<string>-2
//? description: x
//? expect: dnsName("190.190.190.190:8080")
//? source: ...

to_dnsName<string>(
    string("190.190.190.190:8080")
)

//?
//? test: to_dnsName<string>-3
//? description: Invalid arguments test.
//? expect: error
//? source: to_dnsName<string>()
