#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/rfc822Name_match.as
//? customPurpose: Part of core function tests
//? description: Tests for match<rfc822Name> function.
//? sourceType: expression
//?
//? test: match<rfc822Name>-1
//? skip: true
//? description: domain part is not case sensitive
//? expect: boolean(true)
//? source: ...

match<rfc822Name>(
    rfc822Name("localPart@DOMAINPart"),
    string("localPart@DOMAINPART")
)

//?
//? test: match<rfc822Name>-2
//? skip: true
//? description: local part is case sensitive
//? expect: boolean(false)
//? source: ...

match<rfc822Name>(
    rfc822Name("localPart@DOMAINPart"),
    string("LOcalPart@DOMAINParT")
)

//?
//? test: match<rfc822Name>-3
//? skip: true
//? description: domain name is case insensitive
//? expect: boolean(true)
//? source: ...

match<rfc822Name>(
    rfc822Name("sun.com"),
    string("anderson@SUN.COM")
)

//?
//? test: match<rfc822Name>-4
//? skip: true
//? description: extra part in domain name without leading dot is false
//? expect: boolean(false)
//? source: ...

match<rfc822Name>(
    rfc822Name("sun.com"),
    string("anderson@bobo.sun.com")
)

//?
//? test: match<rfc822Name>-5
//? skip: true
//? description: extra part in domain name with leading dot is true
//? expect: boolean(true)
//? source: ...

match<rfc822Name>(
    rfc822Name(".bobo.sun.com"),
    string("anderson@abc.bobo.sun.com")
)

