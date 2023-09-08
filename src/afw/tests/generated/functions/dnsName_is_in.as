#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dnsName_is_in.as
//? customPurpose: Part of core function tests
//? description: Tests for is_in<dnsName> function.
//? sourceType: script
//?
//? test: is_in<dnsName>-1
//? description: x
//? expect: boolean(false)
//? source: ...

is_in<dnsName>(
    dnsName("clemson"),
    bag<dnsName>(
        dnsName("clemson:8080"),
        dnsName("clemsen"),
        dnsName("google:8080")
    )
)

//?
//? test: is_in<dnsName>-2
//? description: x
//? expect: boolean(true)
//? source: ...

is_in<dnsName>(
    dnsName("clemson"),
    bag<dnsName>(
        dnsName("clemson:8080"),
        dnsName("clemson"),
        dnsName("google:8080")
    )
)

//?
//? test: is_in<dnsName>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

is_in<dnsName>(
    dnsName("x")
)

