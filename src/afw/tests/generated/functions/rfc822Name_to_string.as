#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/rfc822Name_to_string.as
//? customPurpose: Part of core function tests
//? description: Tests for to_string<rfc822Name> function.
//? sourceType: script
//?
//? test: to_string<rfc822Name>-1
//? description: x
//? expect: string("anderson@sun.com")
//? source: ...

to_string<rfc822Name>(
    "anderson@sun.com"rfc822Name
)

//?
//? test: to_string<rfc822Name>-2
//? description: x
//? expect: string("localpart@domainpart")
//? source: ...

to_string<rfc822Name>(
    "localpart@domainpart"rfc822Name
)

//?
//? test: to_string<rfc822Name>-3
//? description: Invalid arguments test.
//? expect: error
//? source: to_string<rfc822Name>()
