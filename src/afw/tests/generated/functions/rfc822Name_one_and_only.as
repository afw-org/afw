#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/rfc822Name_one_and_only.as
//? customPurpose: Part of core function tests
//? description: Tests for one_and_only<rfc822Name> function.
//? sourceType: script
//?
//? test: one_and_only<rfc822Name>-1
//? description: x
//? expect: "localPart@DOMAINPart"rfc822Name
//? source: ...

one_and_only<rfc822Name>(
    array<rfc822Name>(
        "localPart@DOMAINPart"rfc822Name
    )
)

//?
//? test: one_and_only<rfc822Name>-2
//? description: x
//? expect: "localPart@DOMAINPart"rfc822Name
//? source: ...

one_and_only<rfc822Name>(
    array<rfc822Name>(
        "localPart@DOMAINPart"rfc822Name
    )
)

//?
//? test: one_and_only<rfc822Name>-3
//? description: Invalid arguments test.
//? expect: error
//? source: ...

one_and_only<rfc822Name>(
    array<rfc822Name>(
        "x@y"rfc822Name,
        "x@y"rfc822Name
    )
)

