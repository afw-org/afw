#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/ipAddress_array.as
//? customPurpose: Part of core function tests
//? description: Tests for array<ipAddress> function.
//? sourceType: script
//?
//? test: array<ipAddress>-1
//? description: array does not remove duplicates
//? expect: ...

array<ipAddress>(
    "204.0.0.224"ipAddress,
    "204.0.0.224"ipAddress,
    "204.0.0.225"ipAddress,
    "204.0.0.226"ipAddress
)

//? source: ...

array<ipAddress>(
    "204.0.0.224"ipAddress,
    "204.0.0.224"ipAddress,
    "204.0.0.225"ipAddress,
    "204.0.0.226"ipAddress
)

//?
//? test: array<ipAddress>-2
//? skip: true
//? description: Invalid arguments test.
//? expect: error
//? source: ...

array<ipAddress>(
    "x"ipAddress
)

