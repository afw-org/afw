#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dnsName_array.as
//? customPurpose: Part of core function tests
//? description: Tests for array<dnsName> function.
//? sourceType: script
//?
//? test: array<dnsName>-1
//? description: x
//? expect: ...

array<dnsName>(
    "clemson:8080"dnsName,
    "google"dnsName,
    "190.190.190.190:8080"dnsName
)

//? source: ...

array<dnsName>(
    "clemson:8080"dnsName,
    "google"dnsName,
    "190.190.190.190:8080"dnsName
)

//?
//? test: array<dnsName>-2
//? skip: true
//? description: Invalid arguments test.
//? expect: error
//? source: array<dnsName>()
