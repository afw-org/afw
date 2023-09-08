#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/x500Name_at_least_one_member_of.as
//? customPurpose: Part of core function tests
//? description: Tests for at_least_one_member_of<x500Name> function.
//? sourceType: script
//?
//? test: at_least_one_member_of<x500Name>-1
//? description: x
//? expect: boolean(true)
//? source: ...

at_least_one_member_of<x500Name>(
    bag<x500Name>(
        x500Name("cn=John Smith,ou=Finance,o=Acme"),
        x500Name("cn=Johnny Smith,ou=Finance,o=Acme"),
        x500Name("cn=Jon Smith,ou=Finance,o=Acme"),
        x500Name("cn=Jonny Smith,ou=Finance,o=Acme")
    ),
    bag<x500Name>(
        x500Name("cn=Jonny Smith,ou=Finance,o=Acme")
    )
)

//?
//? test: at_least_one_member_of<x500Name>-2
//? description: x
//? expect: boolean(false)
//? source: ...

at_least_one_member_of<x500Name>(
    bag<x500Name>(
        x500Name("cn=John Smith,ou=Finance,o=Acme"),
        x500Name("cn=Johnny Smith,ou=Finance,o=Acme"),
        x500Name("cn=Jon Smith,ou=Finance,o=Acme"),
        x500Name("cn=Jonny Smith,ou=Finance,o=Acme")
    ),
    bag<x500Name>(
        x500Name("cn=Lonny Smith,ou=Finance,o=Acme")
    )
)

//?
//? test: at_least_one_member_of<x500Name>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

at_least_one_member_of<x500Name>(
    bag<x500Name>(
        x500Name("x"),
        x500Name("x")
    )
)

