#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/string_to_x500Name.as
//? customPurpose: Part of core function tests
//? description: Tests for to_x500Name<string> function.
//? sourceType: expression
//?
//? test: to_x500Name<string>-1
//? description: Convert string to x500Name
//? expect: x500Name("cn=John Smith,ou=Finance,o=Acme")
//? source: ...

to_x500Name<string>(
    string("cn=John Smith,ou=Finance,o=Acme")
)

//?
//? test: to_x500Name<string>-2
//? description: Invalid arguments test.
//? expect: error
//? source: ...

to_x500Name<string>(
    string("x"),
    string("x")
)

