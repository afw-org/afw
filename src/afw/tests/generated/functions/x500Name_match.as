#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/x500Name_match.as
//? customPurpose: Part of core function tests
//? description: Tests for x500Name_match function.
//? sourceType: script
//?
//? test: x500Name_match-1
//? description: the last RDNs of value match
//? expect: boolean(true)
//? source: ...

x500Name_match(
    "ou=Finance,o=Acme"x500Name,
    "cn=John Smith,ou=Finance,o=Acme"x500Name
)

//?
//? test: x500Name_match-2
//? description: the same name matches
//? expect: boolean(true)
//? source: ...

x500Name_match(
    "cn=John Smith,ou=Finance,o=Acme"x500Name,
    "cn=John Smith,ou=Finance,o=Acme"x500Name
)

//?
//? test: x500Name_match-3
//? description: a different RDN does not match
//? expect: boolean(false)
//? source: ...

x500Name_match(
    "cn=John Smith,ou=Finances,o=Acme"x500Name,
    "cn=John Smith,ou=Finance,o=Acme"x500Name
)

//?
//? test: x500Name_match-4
//? description: RDNs compare as x500Name equality compares them
//? expect: boolean(true)
//? source: ...

x500Name_match(
    "O=Medico Corp,C=US"x500Name,
    "cn=John Smith,o=Medico Corp, c=US"x500Name
)

//?
//? test: x500Name_match-5
//? description: the first RDNs of value do not match
//? expect: boolean(false)
//? source: ...

x500Name_match(
    "cn=John Smith"x500Name,
    "cn=John Smith,o=Medico Corp, c=US"x500Name
)

//?
//? test: x500Name_match-6
//? description: a longer pattern does not match
//? expect: boolean(false)
//? source: ...

x500Name_match(
    "cn=John Smith,o=Medico Corp,c=US"x500Name,
    "o=Medico Corp,c=US"x500Name
)
