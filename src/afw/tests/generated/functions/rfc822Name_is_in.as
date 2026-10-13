#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/rfc822Name_is_in.as
//? customPurpose: Part of core function tests
//? description: Tests for is_in<rfc822Name> function.
//? sourceType: script
//?
//? test: is_in<rfc822Name>-1
//? description: x
//? expect: boolean(true)
//? source: ...

is_in<rfc822Name>(
    "localPart@DOMAINPart"rfc822Name,
    array<rfc822Name>(
        "localPart@DOMAINPart"rfc822Name,
        "x@y"rfc822Name
    )
)

//?
//? test: is_in<rfc822Name>-2
//? description: x
//? expect: boolean(false)
//? source: ...

is_in<rfc822Name>(
    "LocalPart@DOMAINPart"rfc822Name,
    array<rfc822Name>(
        "localPart@DOMAINPart"rfc822Name,
        "x@y"rfc822Name
    )
)

//?
//? test: is_in<rfc822Name>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

is_in<rfc822Name>(
    "x@y"rfc822Name
)

