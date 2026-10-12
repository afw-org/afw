#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dnsName_to_string.as
//? customPurpose: Part of core function tests
//? description: Tests for to_string<dnsName> function.
//? sourceType: script
//?
//? test: to_string<dnsName>-1
//? description: example.org
//? expect: string("example.org")
//? source: ...

to_string<dnsName>(
    "example.org"dnsName
)

//?
//? test: to_string<dnsName>-2
//? description: microsoft.com
//? expect: string("microsoft.com")
//? source: ...

to_string<dnsName>(
    "microsoft.com"dnsName
)

//?
//? test: to_string<dnsName>-3
//? description: microsoft.com
//? expect: error
//? source: to_string<dnsName>()
