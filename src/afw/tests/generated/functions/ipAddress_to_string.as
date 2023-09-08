#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/ipAddress_to_string.as
//? customPurpose: Part of core function tests
//? description: Tests for to_string<ipAddress> function.
//? sourceType: script
//?
//? test: to_string<ipAddress>-1
//? description: 204.11.3.4
//? expect: string("204.11.3.4")
//? source: ...

to_string<ipAddress>(
    ipAddress("204.11.3.4")
)

//?
//? test: to_string<ipAddress>-2
//? description: 204.0.0.255
//? expect: string("204.0.0.255")
//? source: ...

to_string<ipAddress>(
    ipAddress("204.0.0.255")
)

//?
//? test: to_string<ipAddress>-3
//? description: Invalid arguments test.
//? expect: error
//? source: to_string<ipAddress>()
