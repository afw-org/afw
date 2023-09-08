#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/ipAddress_one_and_only.as
//? customPurpose: Part of core function tests
//? description: Tests for one_and_only<ipAddress> function.
//? sourceType: script
//?
//? test: one_and_only<ipAddress>-1
//? description: x
//? expect: ipAddress("255.255.255.255")
//? source: ...

one_and_only<ipAddress>(
    bag<ipAddress>(
        ipAddress("255.255.255.255")
    )
)

//?
//? test: one_and_only<ipAddress>-2
//? description: Invalid arguments test.
//? expect: error
//? source: ...

one_and_only<ipAddress>(
    bag<ipAddress>(
        ipAddress("255.255.255.255"),
        ipAddress("0.0.0.1")
    )
)

