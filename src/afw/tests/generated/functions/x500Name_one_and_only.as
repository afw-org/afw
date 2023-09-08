#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/x500Name_one_and_only.as
//? customPurpose: Part of core function tests
//? description: Tests for one_and_only<x500Name> function.
//? sourceType: script
//?
//? test: one_and_only<x500Name>-1
//? description: x
//? expect: x500Name("cn=John Smith,ou=Finance,o=Acme4")
//? source: ...

one_and_only<x500Name>(
    bag<x500Name>(
        x500Name("cn=John Smith,ou=Finance,o=Acme4")
    )
)

//?
//? test: one_and_only<x500Name>-2
//? description: Invalid arguments test.
//? expect: error
//? source: ...

one_and_only<x500Name>(
    bag<x500Name>(
        x500Name("x"),
        x500Name("x")
    )
)

