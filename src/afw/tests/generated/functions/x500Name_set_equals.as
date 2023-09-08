#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/x500Name_set_equals.as
//? customPurpose: Part of core function tests
//? description: Tests for set_equals<x500Name> function.
//? sourceType: script
//?
//? test: set_equals<x500Name>-1
//? description: x
//? expect: boolean(true)
//? source: ...

set_equals<x500Name>(
    bag<x500Name>(
        x500Name("cn=John Smith,ou=Finance,o=Acme4"),
        x500Name("cn=John Smith,ou=Finance,o=Acme4")
    ),
    bag<x500Name>(
        x500Name("cn=John Smith,ou=Finance,o=Acme4")
    )
)

//?
//? test: set_equals<x500Name>-2
//? description: x
//? expect: boolean(true)
//? source: ...

set_equals<x500Name>(
    bag<x500Name>(
        x500Name("cn=John Smith,ou=Finance,o=Acme1"),
        x500Name("cn=John Smith,ou=Finance,o=Acme2")
    ),
    bag<x500Name>(
        x500Name("cn=John Smith,ou=Finance,o=Acme1"),
        x500Name("cn=John Smith,ou=Finance,o=Acme2")
    )
)

//?
//? test: set_equals<x500Name>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

set_equals<x500Name>(
    bag<x500Name>(
        x500Name("x"),
        x500Name("x")
    )
)

