#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: datatypes/dnsName.as
//? customPurpose: Part of core dataType tests
//? description: Tests for dnsName dataType.
//? sourceType: script
//?
//? test: dnsName-1
//? description: Test 1 for dnsName dataType.
//? expect: boolean(true)
//? source: ...

eq<dnsName>(
    dnsName("example.com"),
    dnsName("example.com")
)

//?
//? test: dnsName-2
//? description: Test 2 for dnsName dataType.
//? expect: boolean(true)
//? source: ...

eq<dnsName>(
    dnsName("*.example.com"),
    dnsName("*.example.com")
)

//?
//? test: dnsName-3
//? description: Test 3 for dnsName dataType.
//? expect: boolean(true)
//? source: ...

eq<dnsName>(
    dnsName("www.example.com"),
    dnsName("www.example.com")
)

//?
//? test: dnsName-4
//? description: Test 4 for dnsName dataType.
//? expect: boolean(true)
//? source: ...

eq<dnsName>(
    dnsName("www.example.com:80"),
    dnsName("www.example.com:80")
)

