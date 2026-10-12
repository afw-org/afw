#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dnsName_one_and_only.as
//? customPurpose: Part of core function tests
//? description: Tests for one_and_only<dnsName> function.
//? sourceType: script
//?
//? test: one_and_only<dnsName>-1
//? description: x
//? expect: "clemson:8080"dnsName
//? source: ...

one_and_only<dnsName>(
    array<dnsName>(
        "clemson:8080"dnsName
    )
)

//?
//? test: one_and_only<dnsName>-2
//? description: Invalid arguments test.
//? expect: error
//? source: ...

one_and_only<dnsName>(
    array<dnsName>(
        "clemson:8080"dnsName,
        "clemson"dnsName
    )
)

