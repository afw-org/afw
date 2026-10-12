#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/ipAddress_is_in.as
//? customPurpose: Part of core function tests
//? description: Tests for is_in<ipAddress> function.
//? sourceType: script
//?
//? test: is_in<ipAddress>-1
//? description: x
//? expect: boolean(true)
//? source: ...

is_in<ipAddress>(
    "255.255.255.255"ipAddress,
    bag<ipAddress>(
        "255.255.255.250"ipAddress,
        "255.255.255.251"ipAddress,
        "255.255.255.252"ipAddress,
        "255.255.255.253"ipAddress,
        "255.255.255.254"ipAddress,
        "255.255.255.255"ipAddress
    )
)

//?
//? test: is_in<ipAddress>-2
//? description: Invalid arguments test.
//? expect: error
//? source: ...

is_in<ipAddress>(
    "x"ipAddress
)

