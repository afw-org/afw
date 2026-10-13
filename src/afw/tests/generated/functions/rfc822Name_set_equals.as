#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/rfc822Name_set_equals.as
//? customPurpose: Part of core function tests
//? description: Tests for set_equals<rfc822Name> function.
//? sourceType: script
//?
//? test: set_equals<rfc822Name>-1
//? description: x
//? expect: boolean(true)
//? source: ...

set_equals<rfc822Name>(
    array<rfc822Name>(
        "localPart@DOMAINPart"rfc822Name,
        "localPart@DOMAINPart2"rfc822Name
    ),
    array<rfc822Name>(
        "localPart@DOMAINPart"rfc822Name,
        "localPart@DOMAINPart2"rfc822Name
    )
)

//?
//? test: set_equals<rfc822Name>-2
//? description: the local part is case sensitive
//? expect: boolean(false)
//? source: ...

set_equals<rfc822Name>(
    array<rfc822Name>(
        "localPart@DOMAINPart"rfc822Name,
        "localPart@DOMAINPart2"rfc822Name
    ),
    array<rfc822Name>(
        "localPart@DOMAINPart"rfc822Name,
        "LocalPart@DOMAINPart2"rfc822Name
    )
)

//?
//? test: set_equals<rfc822Name>-domain-part
//? description: the domain part ignores case
//? expect: boolean(true)
//? source: ...

set_equals<rfc822Name>(
    array<rfc822Name>(
        "localPart@DOMAINPart"rfc822Name,
        "localPart@DOMAINPart2"rfc822Name
    ),
    array<rfc822Name>(
        "localPart@DOMAINPart"rfc822Name,
        "localPart@DOMAINParT2"rfc822Name
    )
)

//?
//? test: set_equals<rfc822Name>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

set_equals<rfc822Name>(
    array<rfc822Name>(
        "x@y"rfc822Name,
        "x@y"rfc822Name
    )
)

