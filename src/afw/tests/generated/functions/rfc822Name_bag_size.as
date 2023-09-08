#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/rfc822Name_bag_size.as
//? customPurpose: Part of core function tests
//? description: Tests for bag_size<rfc822Name> function.
//? sourceType: script
//?
//? test: bag_size<rfc822Name>-1
//? description: x
//? expect: integer(4)
//? source: ...

bag_size<rfc822Name>(
    bag<rfc822Name>(
        rfc822Name("LOCALPart@DomainPart.com"),
        rfc822Name("localPart@DOMAINPart"),
        rfc822Name("differentlocalPart@DOMAINPart"),
        rfc822Name("localPart@differentDOMAINPart")
    )
)

//?
//? test: bag_size<rfc822Name>-2
//? description: x
//? expect: integer(2)
//? source: ...

bag_size<rfc822Name>(
    bag<rfc822Name>(
        rfc822Name("LOCALPart@DomainPart"),
        rfc822Name("localPart@DOMAINPart")
    )
)

//?
//? test: bag_size<rfc822Name>-3
//? description: Invalid arguments test.
//? expect: error
//? source: bag_size<rfc822Name>()
