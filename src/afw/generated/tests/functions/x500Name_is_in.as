#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/x500Name_is_in.as
//? customPurpose: Part of core function tests
//? description: Tests for is_in<x500Name> function.
//? sourceType: expression
//?
//? test: is_in<x500Name>-1
//? description: x
//? expect: boolean(true)
//? source: ...

is_in<x500Name>(
    x500Name("cn=John Smith,ou=Finance,o=Acme"),
    bag<x500Name>(
        x500Name("cn=John Smith,ou=Finance,o=Acme1"),
        x500Name("cn=John Smith,ou=Finance,o=Acme2"),
        x500Name("cn=John Smith,ou=Finance,o=Acme3"),
        x500Name("cn=John Smith,ou=Finance,o=Acme4"),
        x500Name("cn=John Smith,ou=Finance,o=Acme")
    )
)

//?
//? test: is_in<x500Name>-2
//? description: x
//? expect: boolean(false)
//? source: ...

is_in<x500Name>(
    x500Name("cn=John Smith,ou=Finance,o=Acme"),
    bag<x500Name>(
        x500Name("cn=John Smith,ou=Finance,o=Acme1"),
        x500Name("cn=John Smith,ou=Finance,o=Acme2"),
        x500Name("cn=John Smith,ou=Finance,o=Acme3"),
        x500Name("cn=John Smith,ou=Finance,o=Acme4"),
        x500Name("cn=John Smith,ou=Finance,o=Acme5")
    )
)

//?
//? test: is_in<x500Name>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

is_in<x500Name>(
    x500Name("x")
)

