#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/x500Name_match.as
//? customPurpose: Part of core function tests
//? description: Tests for match<x500Name> function.
//? sourceType: script
//?
//? test: match<x500Name>-1
//? skip: true
//? description: should work
//? expect: boolean(true)
//? source: ...

match<x500Name>(
    x500Name("ou=Finance,o=Acme"),
    x500Name("cn=John Smith,ou=Finance,o=Acme")
)

//?
//? test: match<x500Name>-2
//? skip: true
//? description: same
//? expect: boolean(true)
//? source: ...

match<x500Name>(
    x500Name("cn=John Smith,ou=Finance,o=Acme"),
    x500Name("cn=John Smith,ou=Finance,o=Acme")
)

//?
//? test: match<x500Name>-3
//? skip: true
//? description: not the same
//? expect: boolean(false)
//? source: ...

match<x500Name>(
    x500Name("cn=John Smith,ou=Finances,o=Acme"),
    x500Name("cn=John Smith,ou=Finance,o=Acme")
)

