#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/x500Name_subset.as
//? customPurpose: Part of core function tests
//? description: Tests for subset<x500Name> function.
//? sourceType: expression
//?
//? test: subset<x500Name>-1
//? description: x
//? expect: boolean(true)
//? source: ...

subset<x500Name>(
    bag<x500Name>(
        x500Name("cn=John Smith,ou=Finance,o=Acme3"),
        x500Name("cn=John Smith,ou=Finance,o=Acme4")
    ),
    bag<x500Name>(
        x500Name("cn=John Smith,ou=Finance,o=Acme1"),
        x500Name("cn=John Smith,ou=Finance,o=Acme2"),
        x500Name("cn=John Smith,ou=Finance,o=Acme3"),
        x500Name("cn=John Smith,ou=Finance,o=Acme4"),
        x500Name("cn=John Smith,ou=Finance,o=Acme5"),
        x500Name("cn=John Smith,ou=Finance,o=Acme6")
    )
)

//?
//? test: subset<x500Name>-2
//? description: x
//? expect: boolean(true)
//? source: ...

subset<x500Name>(
    bag<x500Name>(
        x500Name("cn=John Smith,ou=Finance,o=Acme4"),
        x500Name("cn=John Smith,ou=Finance,o=Acme4")
    ),
    bag<x500Name>(
        x500Name("cn=John Smith,ou=Finance,o=Acme1"),
        x500Name("cn=John Smith,ou=Finance,o=Acme2"),
        x500Name("cn=John Smith,ou=Finance,o=Acme3"),
        x500Name("cn=John Smith,ou=Finance,o=Acme4"),
        x500Name("cn=John Smith,ou=Finance,o=Acme5"),
        x500Name("cn=John Smith,ou=Finance,o=Acme6")
    )
)

//?
//? test: subset<x500Name>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

subset<x500Name>(
    bag<x500Name>(
        x500Name("x"),
        x500Name("x")
    )
)

