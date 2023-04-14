#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/dnsName_bag_size.as
//? customPurpose: Part of core function tests
//? description: Tests for bag_size<dnsName> function.
//? sourceType: expression
//?
//? test: bag_size<dnsName>-1
//? description: x
//? expect: integer(3)
//? source: ...

bag_size<dnsName>(
    bag<dnsName>(
        dnsName("clemson:8080"),
        dnsName("google"),
        dnsName("190.190.190.190:8080")
    )
)

//?
//? test: bag_size<dnsName>-2
//? description: x
//? expect: integer(0)
//? source: ...

bag_size<dnsName>(
    bag<dnsName>(
    )
)

//?
//? test: bag_size<dnsName>-3
//? description: Invalid arguments test.
//? expect: error
//? source: bag_size<dnsName>()
