#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/ipAddress_bag_size.as
//? customPurpose: Part of core function tests
//? description: Tests for bag_size<ipAddress> function.
//? sourceType: expression
//?
//? test: bag_size<ipAddress>-1
//? description: x
//? expect: integer(3)
//? source: ...

bag_size<ipAddress>(
    bag<ipAddress>(
        ipAddress("204.11.3.4"),
        ipAddress("204.11.3.4"),
        ipAddress("204.11.3.224")
    )
)

//?
//? test: bag_size<ipAddress>-2
//? description: x
//? expect: integer(3)
//? source: ...

bag_size<ipAddress>(
    bag<ipAddress>(
        ipAddress("204.11.3.4"),
        ipAddress("204.11.3.5"),
        ipAddress("204.11.3.224")
    )
)

//?
//? test: bag_size<ipAddress>-3
//? description: Invalid arguments test.
//? expect: error
//? source: bag_size<ipAddress>()
