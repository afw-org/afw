#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: datatypes/ipAddress.as
//? customPurpose: Part of core dataType tests
//? description: Tests for ipAddress dataType.
//? sourceType: expression
//?
//? test: ipAddress-1
//? description: Test 1 for ipAddress dataType.
//? expect: boolean(true)
//? source: ...

eq<ipAddress>(
    ipAddress("194.153.205.26"),
    ipAddress("194.153.205.26")
)

//?
//? test: ipAddress-2
//? description: Test 2 for ipAddress dataType.
//? expect: boolean(true)
//? source: ...

eq<ipAddress>(
    ipAddress("194.153.205.26:22"),
    ipAddress("194.153.205.26:22")
)

//?
//? test: ipAddress-3
//? description: Test 3 for ipAddress dataType.
//? expect: boolean(true)
//? source: ...

eq<ipAddress>(
    ipAddress("194.28.0.0/16"),
    ipAddress("194.28.0.0/16")
)

//?
//? test: ipAddress-4
//? description: Test 4 for ipAddress dataType.
//? expect: boolean(true)
//? source: ...

eq<ipAddress>(
    ipAddress("2001:0f68:0000:0000:0000:0000:1986:69af"),
    ipAddress("2001:0f68:0000:0000:0000:0000:1986:69af")
)

//?
//? test: ipAddress-5
//? description: Test 5 for ipAddress dataType.
//? expect: boolean(true)
//? source: ...

eq<ipAddress>(
    ipAddress("2001:f68::1986:69af"),
    ipAddress("2001:f68::1986:69af")
)

//?
//? test: ipAddress-6
//? description: Test 6 for ipAddress dataType.
//? expect: boolean(true)
//? source: ...

eq<ipAddress>(
    ipAddress("[2001:f68::1986:69af]:80-90"),
    ipAddress("[2001:f68::1986:69af]:80-90")
)

//?
//? test: ipAddress-7
//? description: Test 7 for ipAddress dataType.
//? expect: boolean(true)
//? source: ...

eq<ipAddress>(
    ipAddress("2001:db8:1234:5678::9/64"),
    ipAddress("2001:db8:1234:5678::9/64")
)

