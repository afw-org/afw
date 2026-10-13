#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/rfc822Name_subset.as
//? customPurpose: Part of core function tests
//? description: Tests for subset<rfc822Name> function.
//? sourceType: script
//?
//? test: subset<rfc822Name>-1
//? description: x
//? expect: boolean(true)
//? source: ...

subset<rfc822Name>(
    array<rfc822Name>(
        "anderson@sun.com"rfc822Name,
        "localpart@domainpart"rfc822Name
    ),
    array<rfc822Name>(
        "anderson@sun.com"rfc822Name,
        "localpart@domainpart"rfc822Name
    )
)

//?
//? test: subset<rfc822Name>-2
//? description: x
//? expect: boolean(false)
//? source: ...

subset<rfc822Name>(
    array<rfc822Name>(
        "anderson@sun.com"rfc822Name,
        "localpart@domainpart"rfc822Name
    ),
    array<rfc822Name>(
        "anderson@sun.com"rfc822Name,
        "bobo@sun.com"rfc822Name
    )
)

//?
//? test: subset<rfc822Name>-3
//? description: x
//? expect: boolean(true)
//? source: ...

subset<rfc822Name>(
    array<rfc822Name>(
        "anderson@sun.com"rfc822Name,
        "localpart@domainpart"rfc822Name
    ),
    array<rfc822Name>(
        "anderson@sun.com"rfc822Name,
        "bobo@sun.org"rfc822Name,
        "anderson@sun.edu"rfc822Name,
        "bobobo@sun.com"rfc822Name,
        "banderson@sun.com"rfc822Name,
        "bonobo@sun.com"rfc822Name,
        "localpart@domainpart"rfc822Name
    )
)

//?
//? test: subset<rfc822Name>-4
//? description: Invalid arguments test.
//? expect: error
//? source: ...

subset<rfc822Name>(
    array<rfc822Name>(
        "x@y"rfc822Name,
        "x@y"rfc822Name
    )
)

