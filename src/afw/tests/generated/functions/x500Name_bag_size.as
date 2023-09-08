#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/x500Name_bag_size.as
//? customPurpose: Part of core function tests
//? description: Tests for bag_size<x500Name> function.
//? sourceType: script
//?
//? test: bag_size<x500Name>-1
//? description: x
//? expect: integer(6)
//? source: ...

bag_size<x500Name>(
    bag<x500Name>(
        x500Name("cn=John Smith,ou=Finance,o=Acme"),
        x500Name("cn=John Smith,ou=Finance1,o=Acme"),
        x500Name("cn=John Smith,ou=Finance2,o=Acme"),
        x500Name("cn=John Smith,ou=Finance4,o=Acme"),
        x500Name("cn=John Smith,ou=Finance3,o=Acme"),
        x500Name("cn=John Smith,ou=Finance5,o=Acme")
    )
)

//?
//? test: bag_size<x500Name>-2
//? description: x
//? expect: integer(0)
//? source: ...

bag_size<x500Name>(
    bag<x500Name>(
    )
)

//?
//? test: bag_size<x500Name>-3
//? description: Invalid arguments test.
//? expect: error
//? source: bag_size<x500Name>()
