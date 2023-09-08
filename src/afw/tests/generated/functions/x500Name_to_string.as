#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/x500Name_to_string.as
//? customPurpose: Part of core function tests
//? description: Tests for to_string<x500Name> function.
//? sourceType: script
//?
//? test: to_string<x500Name>-1
//? description: x
//? expect: string("cn=John Smith,ou=Finance,o=Acme")
//? source: ...

to_string<x500Name>(
    x500Name("cn=John Smith,ou=Finance,o=Acme")
)

//?
//? test: to_string<x500Name>-2
//? description: x
//? expect: string("cn=,ou=Finance,o=Acme")
//? source: ...

to_string<x500Name>(
    x500Name("cn=,ou=Finance,o=Acme")
)

//?
//? test: to_string<x500Name>-3
//? skip: true
//? description: Invalid arguments test.
//? expect: error
//? source: ...

to_string<x500Name>(
    x500Name("x")
)

