#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/x500Name_eq.as
//? customPurpose: Part of core function tests
//? description: Tests for eq<x500Name> function.
//? sourceType: script
//?
//? test: eq<x500Name>-1
//? description: x
//? expect: boolean(true)
//? source: ...

eq<x500Name>(
    x500Name("cn=John Smith,ou=Finance,o=Acme"),
    x500Name("cn=John Smith,ou=Finance,o=Acme")
)

//?
//? test: eq<x500Name>-2
//? description: x
//? expect: boolean(false)
//? source: ...

eq<x500Name>(
    x500Name("cn=John Smith,ou=Finance,o=Acme"),
    x500Name("cn=Johny Smith,ou=Finance,o=Acme")
)

//?
//? test: eq<x500Name>-3
//? description: Invalid arguments test.
//? expect: error
//? source: eq<x500Name>()
