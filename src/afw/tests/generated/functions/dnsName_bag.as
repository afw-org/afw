#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dnsName_bag.as
//? customPurpose: Part of core function tests
//? description: Tests for bag<dnsName> function.
//? sourceType: script
//?
//? test: bag<dnsName>-1
//? description: x
//? expect: ...

bag<dnsName>(
    "clemson:8080"dnsName,
    "google"dnsName,
    "190.190.190.190:8080"dnsName
)

//? source: ...

bag<dnsName>(
    "clemson:8080"dnsName,
    "google"dnsName,
    "190.190.190.190:8080"dnsName
)

//?
//? test: bag<dnsName>-2
//? skip: true
//? description: Invalid arguments test.
//? expect: error
//? source: bag<dnsName>()
