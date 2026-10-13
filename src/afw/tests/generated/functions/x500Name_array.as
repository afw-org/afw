#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/x500Name_array.as
//? customPurpose: Part of core function tests
//? description: Tests for array<x500Name> function.
//? sourceType: script
//?
//? test: array<x500Name>-1
//? description: None
//? expect: ...

array<x500Name>(
    "cn=John Smith,ou=Finance,o=Acme"x500Name,
    "cn=John Smith,ou=Finance,o=Acme"x500Name,
    "cn=John Smith,ou=Finance,o=Acme"x500Name
)

//? source: ...

array<x500Name>(
    "cn=John Smith,ou=Finance,o=Acme"x500Name,
    "cn=John Smith,ou=Finance,o=Acme"x500Name,
    "cn=John Smith,ou=Finance,o=Acme"x500Name
)

//?
//? test: array<x500Name>-2
//? description: None
//? expect: ...

array<x500Name>(
)

//? source: array<x500Name>()
//?
//? test: array<x500Name>-3
//? skip: true
//? description: Invalid arguments test.
//? expect: error
//? source: ...

array<x500Name>(
    "cn=x"x500Name
)

