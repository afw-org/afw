#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/rfc822Name_match.as
//? customPurpose: Part of core function tests
//? description: Tests for rfc822Name_match function.
//? sourceType: script
//?
//? test: rfc822Name_match-1
//? description: a complete mailbox matches it with the domain part in any case
//? expect: boolean(true)
//? source: ...

rfc822Name_match(
    "localPart@DOMAINPART",
    "localPart@DOMAINPart"rfc822Name
)

//?
//? test: rfc822Name_match-2
//? description: the local part of a complete mailbox is case sensitive
//? expect: boolean(false)
//? source: ...

rfc822Name_match(
    "LOcalPart@DOMAINParT",
    "localPart@DOMAINPart"rfc822Name
)

//?
//? test: rfc822Name_match-3
//? description: a domain matches mailboxes at that domain in any case
//? expect: boolean(true)
//? source: ...

rfc822Name_match(
    "sun.com",
    "anderson@SUN.COM"rfc822Name
)

//?
//? test: rfc822Name_match-4
//? description: a domain without a leading dot does not match its subdomains
//? expect: boolean(false)
//? source: ...

rfc822Name_match(
    "sun.com",
    "anderson@bobo.sun.com"rfc822Name
)

//?
//? test: rfc822Name_match-5
//? description: a domain with a leading dot matches its subdomains
//? expect: boolean(true)
//? source: ...

rfc822Name_match(
    ".east.sun.com",
    "anderson@bobo.EAST.sun.com"rfc822Name
)

//?
//? test: rfc822Name_match-6
//? description: a domain with a leading dot does not match the domain itself
//? expect: boolean(false)
//? source: ...

rfc822Name_match(
    ".east.sun.com",
    "anderson@east.sun.com"rfc822Name
)

//?
//? test: rfc822Name_match-7
//? description: a domain with a leading dot matches whole labels only
//? expect: boolean(false)
//? source: ...

rfc822Name_match(
    ".sun.com",
    "anderson@bobosun.com"rfc822Name
)

//?
//? test: rfc822Name_match-8
//? description: the pattern is a string and the value an rfc822Name
//? expect: error
//? source: ...

rfc822Name_match(
    "anderson@sun.com"rfc822Name,
    "sun.com"
)
