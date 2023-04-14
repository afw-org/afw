#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/x500Name_bag.as
//? customPurpose: Part of core function tests
//? description: Tests for bag<x500Name> function.
//? sourceType: expression
//?
//? test: bag<x500Name>-1
//? description: None
//? expect: ...

bag<x500Name>(
    x500Name("cn=John Smith,ou=Finance,o=Acme"),
    x500Name("cn=John Smith,ou=Finance,o=Acme"),
    x500Name("cn=John Smith,ou=Finance,o=Acme")
)

//? source: ...

bag<x500Name>(
    x500Name("cn=John Smith,ou=Finance,o=Acme"),
    x500Name("cn=John Smith,ou=Finance,o=Acme"),
    x500Name("cn=John Smith,ou=Finance,o=Acme")
)

//?
//? test: bag<x500Name>-2
//? description: None
//? expect: ...

bag<x500Name>(
)

//? source: bag<x500Name>()
//?
//? test: bag<x500Name>-3
//? skip: true
//? description: Invalid arguments test.
//? expect: error
//? source: ...

bag<x500Name>(
    x500Name("x")
)

