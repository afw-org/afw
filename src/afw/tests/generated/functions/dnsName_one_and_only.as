#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dnsName_one_and_only.as
//? customPurpose: Part of core function tests
//? description: Tests for one_and_only<dnsName> function.
//? sourceType: expression
//?
//? test: one_and_only<dnsName>-1
//? description: x
//? expect: dnsName("clemson:8080")
//? source: ...

one_and_only<dnsName>(
    bag<dnsName>(
        dnsName("clemson:8080")
    )
)

//?
//? test: one_and_only<dnsName>-2
//? description: Invalid arguments test.
//? expect: error
//? source: ...

one_and_only<dnsName>(
    bag<dnsName>(
        dnsName("clemson:8080"),
        dnsName("clemson")
    )
)

