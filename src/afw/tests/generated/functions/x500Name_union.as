#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/x500Name_union.as
//? customPurpose: Part of core function tests
//? description: Tests for union<x500Name> function.
//? sourceType: script
//?
//? test: union<x500Name>-1
//? description: x
//? expect: ...

array<x500Name>(
    "cn=John Smith,ou=Finance,o=Acme1"x500Name,
    "cn=John Smith,ou=Finance,o=Acme2"x500Name
)

//? source: ...

union<x500Name>(
    array<x500Name>(
        "cn=John Smith,ou=Finance,o=Acme1"x500Name,
        "cn=John Smith,ou=Finance,o=Acme2"x500Name
    ),
    array<x500Name>(
        "cn=John Smith,ou=Finance,o=Acme1"x500Name,
        "cn=John Smith,ou=Finance,o=Acme2"x500Name
    )
)

//?
//? test: union<x500Name>-2
//? description: x
//? expect: ...

array<x500Name>(
    "cn=John Smith,ou=Finance,o=Acme1"x500Name,
    "cn=John Smith,ou=Finance,o=Acme2"x500Name,
    "cn=John Smith,ou=Finance,o=Acme3"x500Name,
    "cn=John Smith,ou=Finance,o=Acme4"x500Name
)

//? source: ...

union<x500Name>(
    array<x500Name>(
        "cn=John Smith,ou=Finance,o=Acme1"x500Name,
        "cn=John Smith,ou=Finance,o=Acme2"x500Name
    ),
    array<x500Name>(
        "cn=John Smith,ou=Finance,o=Acme3"x500Name,
        "cn=John Smith,ou=Finance,o=Acme4"x500Name
    )
)

//?
//? test: union<x500Name>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

union<x500Name>(
    array<x500Name>(
        "x"x500Name,
        "x"x500Name
    )
)

