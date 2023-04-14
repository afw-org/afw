#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/x500Name_intersection.as
//? customPurpose: Part of core function tests
//? description: Tests for intersection<x500Name> function.
//? sourceType: expression
//?
//? test: intersection<x500Name>-1
//? description: x
//? expect: ...

bag<x500Name>(
    x500Name("cn=John Smith,ou=Finance,o=Acme2")
)

//? source: ...

intersection<x500Name>(
    bag<x500Name>(
        x500Name("cn=John Smith,ou=Finance,o=Acme1"),
        x500Name("cn=John Smith,ou=Finance,o=Acme2")
    ),
    bag<x500Name>(
        x500Name("cn=John Smith,ou=Finance,o=Acme2"),
        x500Name("cn=John Smith,ou=Finance,o=Acme3")
    )
)

//?
//? test: intersection<x500Name>-2
//? description: x
//? expect: ...

bag<x500Name>(
)

//? source: ...

intersection<x500Name>(
    bag<x500Name>(
        x500Name("cn=John Smith,ou=Finance,o=Acme1"),
        x500Name("cn=John Smith,ou=Finance,o=Acme2")
    ),
    bag<x500Name>(
        x500Name("cn=John Smith,ou=Finance,o=Acme3"),
        x500Name("cn=John Smith,ou=Finance,o=Acme4")
    )
)

//?
//? test: intersection<x500Name>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

intersection<x500Name>(
    bag<x500Name>(
        x500Name("x"),
        x500Name("x")
    )
)

