#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/ipAddress_bag.as
//? customPurpose: Part of core function tests
//? description: Tests for bag<ipAddress> function.
//? sourceType: expression
//?
//? test: bag<ipAddress>-1
//? description: bag does not remove duplicates
//? expect: ...

bag<ipAddress>(
    ipAddress("204.0.0.224"),
    ipAddress("204.0.0.224"),
    ipAddress("204.0.0.225"),
    ipAddress("204.0.0.226")
)

//? source: ...

bag<ipAddress>(
    ipAddress("204.0.0.224"),
    ipAddress("204.0.0.224"),
    ipAddress("204.0.0.225"),
    ipAddress("204.0.0.226")
)

//?
//? test: bag<ipAddress>-2
//? skip: true
//? description: Invalid arguments test.
//? expect: error
//? source: ...

bag<ipAddress>(
    ipAddress("x")
)

